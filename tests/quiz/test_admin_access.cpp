#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>

#include "quiz/AdminAccessController.h"
#include "quiz/UserManager.h"
#include "quiz/QuizDatabase.h"

class AdminAccessTest : public QObject
{
    Q_OBJECT

private:
    // ── Helpers ──────────────────────────────────────────────────────────────

    static void initInMemoryDb()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                                    QuizDatabase::CONNECTION_NAME);
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery q(db);
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "  id            INTEGER  PRIMARY KEY AUTOINCREMENT,"
            "  username      TEXT     NOT NULL UNIQUE COLLATE NOCASE,"
            "  display_name  TEXT     NOT NULL,"
            "  password_hash TEXT     NOT NULL,"
            "  salt          TEXT     NOT NULL,"
            "  avatar_color  TEXT     DEFAULT '#007ACC',"
            "  avatar_path   TEXT     DEFAULT NULL,"
            "  created_at    DATETIME DEFAULT CURRENT_TIMESTAMP,"
            "  last_login    DATETIME,"
            "  is_admin      INTEGER  DEFAULT 0"
            ")"
        ));
    }

    void cleanupUsers()
    {
        UserManager::instance().logout();
        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.exec("DELETE FROM users WHERE username LIKE 'test_%'");
    }

private slots:
    void initTestCase()
    {
        initInMemoryDb();
    }

    void cleanupTestCase()
    {
        UserManager::instance().logout();
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }

    void cleanup()
    {
        cleanupUsers();
    }

    // 1. Not logged in => NotLoggedIn
    void testNotLoggedIn()
    {
        UserManager::instance().logout();

        const AdminAccessResult result =
            AdminAccessController::instance().verifyAccess(nullptr, false);

        QCOMPARE(result, AdminAccessResult::NotLoggedIn);
    }

    // 2. Logged-in non-admin => NotAdmin
    void testLoggedInNonAdmin()
    {
        QVERIFY(UserManager::instance().registerUser("test_regular", "Regular User", "pass1234", false));
        QVERIFY(UserManager::instance().login("test_regular", "pass1234"));

        const AdminAccessResult result =
            AdminAccessController::instance().verifyAccess(nullptr, false);

        QCOMPARE(result, AdminAccessResult::NotAdmin);
    }

    // 3. Logged-in admin, requireReleasePassword=false => Granted
    void testAdminGrantedDebugPath()
    {
        QVERIFY(UserManager::instance().registerUser("test_admin", "Admin User", "adminpass", true));
        QVERIFY(UserManager::instance().login("test_admin", "adminpass"));

        const AdminAccessResult result =
            AdminAccessController::instance().verifyAccess(nullptr, false);

        QCOMPARE(result, AdminAccessResult::Granted);
    }

    // 4. Release-password-required path: dialog cancelled => PasswordRejected
    void testReleasePasswordRejectedOnCancel()
    {
        // Set a known hash so the empty-hash guard does not return Error
        const QString knownHash = QString::fromLatin1(
            QCryptographicHash::hash(QByteArrayLiteral("correct_password"),
                                     QCryptographicHash::Sha256).toHex()
        );
        AdminAccessController::instance().setReleaseAdminPasswordHash(knownHash);

        QVERIFY(UserManager::instance().registerUser("test_admin2", "Admin2", "adminpass2", true));
        QVERIFY(UserManager::instance().login("test_admin2", "adminpass2"));

        // Schedule dialog dismissal. QInputDialog::exec() blocks the test thread
        // in its own event loop, so a QTimer::singleShot posted before exec() is
        // the idiomatic Qt way to dismiss a synchronous modal dialog in tests.
        // 50 ms is sufficient for the offscreen platform to process the event.
        QTimer::singleShot(50, []() {
            QWidget* modal = QApplication::activeModalWidget();
            if (modal) {
                QTest::keyClick(modal, Qt::Key_Escape);
            }
        });

        const AdminAccessResult result =
            AdminAccessController::instance().verifyAccess(nullptr, true);

        QCOMPARE(result, AdminAccessResult::PasswordRejected);

        // Reset hash so later tests are not affected
        AdminAccessController::instance().setReleaseAdminPasswordHash(QString());
    }
};

QTEST_MAIN(AdminAccessTest)
#include "test_admin_access.moc"
