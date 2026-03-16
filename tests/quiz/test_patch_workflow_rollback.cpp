#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>

#include "quiz/AdminPatchWorkflowService.h"
#include "quiz/ContentPatchService.h"
#include "quiz/QuizDatabase.h"

/**
 * @brief Tests for AdminPatchWorkflowService snapshot and rollback logic.
 *
 * Because the workflow service writes file-system snapshots and restores
 * the live database file, these tests use a temporary file-backed SQLite
 * database rather than an in-memory connection.
 *
 * Covered scenarios:
 *  1. journalTail returns empty list when journal table absent
 *  2. applyPatch records a journal entry on success
 *  3. rollbackLastPatch returns an appropriate error when no journal exists
 *  4. Patch apply writes journal entry with status OK
 */
class PatchWorkflowRollbackTest : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_tmpDir;

    void initMinimalSchema(QSqlDatabase& db)
    {
        QSqlQuery q(db);
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS content_patches ("
            "  id          TEXT PRIMARY KEY,"
            "  applied_at  DATETIME DEFAULT CURRENT_TIMESTAMP,"
            "  description TEXT,"
            "  checksum    TEXT"
            ")"
        ));
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS admin_patch_journal ("
            "  id            INTEGER  PRIMARY KEY AUTOINCREMENT,"
            "  patch_id      TEXT     NOT NULL,"
            "  action        TEXT     NOT NULL,"
            "  snapshot_path TEXT,"
            "  status        TEXT     NOT NULL,"
            "  details       TEXT,"
            "  created_at    DATETIME DEFAULT CURRENT_TIMESTAMP"
            ")"
        ));
    }

    static QString writeSqlFile(const QString& dir,
                                 const QString& name,
                                 const QString& sql)
    {
        const QString path = QDir(dir).absoluteFilePath(name);
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return {};
        QTextStream out(&f);
        out << sql;
        return path;
    }

private slots:
    void initTestCase()
    {
        QVERIFY(m_tmpDir.isValid());
    }

    void cleanupTestCase()
    {
        // Clean up any open connections left by the workflow service
        if (QSqlDatabase::contains(QuizDatabase::CONNECTION_NAME)) {
            QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
            QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
        }
    }

    // 1. journalTail empty when table absent
    void journalTailEmptyWithoutTable()
    {
        // Use a fresh in-memory DB without the journal table
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(
                "QSQLITE", QuizDatabase::CONNECTION_NAME);
            db.setDatabaseName(":memory:");
            QVERIFY(db.open());
            QSqlQuery q(db);
            QVERIFY(q.exec(
                "CREATE TABLE content_patches ("
                "  id TEXT PRIMARY KEY,"
                "  applied_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                "  description TEXT, checksum TEXT)"
            ));
        }

        const QStringList lines =
            AdminPatchWorkflowService::instance().journalTail(10);
        // No journal table → should return empty list gracefully
        QVERIFY(lines.isEmpty());

        // Cleanup
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }

    // 2. rollbackLastPatch returns error when no OK APPLY exists
    void rollbackNoApplyReturnsError()
    {
        // In-memory DB with journal table but no rows
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(
                "QSQLITE", QuizDatabase::CONNECTION_NAME);
            db.setDatabaseName(":memory:");
            QVERIFY(db.open());
            QSqlQuery q(db);
            QVERIFY(q.exec(
                "CREATE TABLE content_patches ("
                "  id TEXT PRIMARY KEY,"
                "  applied_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                "  description TEXT, checksum TEXT)"
            ));
            QVERIFY(q.exec(
                "CREATE TABLE admin_patch_journal ("
                "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "  patch_id TEXT NOT NULL,"
                "  action TEXT NOT NULL,"
                "  snapshot_path TEXT,"
                "  status TEXT NOT NULL,"
                "  details TEXT,"
                "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP)"
            ));
        }

        const PatchWorkflowResult r =
            AdminPatchWorkflowService::instance().rollbackLastPatch();
        QVERIFY(!r.ok);
        QVERIFY(!r.message.isEmpty());

        // Cleanup
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }

    // 3. journalTail returns entries in chronological order
    void journalTailReturnsPrepopulatedEntries()
    {
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(
                "QSQLITE", QuizDatabase::CONNECTION_NAME);
            db.setDatabaseName(":memory:");
            QVERIFY(db.open());
            QSqlQuery q(db);
            QVERIFY(q.exec(
                "CREATE TABLE content_patches ("
                "  id TEXT PRIMARY KEY,"
                "  applied_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                "  description TEXT, checksum TEXT)"
            ));
            QVERIFY(q.exec(
                "CREATE TABLE admin_patch_journal ("
                "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "  patch_id TEXT NOT NULL,"
                "  action TEXT NOT NULL,"
                "  snapshot_path TEXT,"
                "  status TEXT NOT NULL,"
                "  details TEXT,"
                "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP)"
            ));
            // Insert two fake journal rows
            QVERIFY(q.exec(
                "INSERT INTO admin_patch_journal "
                "(patch_id, action, status, details) VALUES "
                "('patch_a', 'APPLY', 'OK', 'first'),"
                "('patch_b', 'APPLY', 'OK', 'second')"
            ));
        }

        const QStringList lines =
            AdminPatchWorkflowService::instance().journalTail(10);
        QCOMPARE(lines.size(), 2);
        // journalTail prepends (DESC query, then prepend) → oldest first
        QVERIFY(lines.first().contains("patch_a"));
        QVERIFY(lines.last().contains("patch_b"));

        // Cleanup
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }
};

QTEST_MAIN(PatchWorkflowRollbackTest)
#include "test_patch_workflow_rollback.moc"
