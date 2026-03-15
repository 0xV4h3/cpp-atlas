#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>

#include "quiz/ContentPatchService.h"
#include "quiz/QuizDatabase.h"

class ContentPatchServiceTest : public QObject
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
        // Minimal schema: only the table(s) ContentPatchService uses.
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS content_patches ("
            "  id          TEXT PRIMARY KEY,"
            "  applied_at  DATETIME DEFAULT CURRENT_TIMESTAMP,"
            "  description TEXT,"
            "  checksum    TEXT"
            ")"
        ));
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS topics ("
            "  id    INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  title TEXT NOT NULL,"
            "  slug  TEXT NOT NULL UNIQUE"
            ")"
        ));
    }

    static QString writeSqlFile(const QTemporaryDir& dir, const QString& name,
                                const QString& sql)
    {
        const QString path = dir.filePath(name);
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return QString();
        QTextStream out(&f);
        out << sql;
        return path;
    }

private slots:
    void initTestCase()
    {
        initInMemoryDb();
    }

    void cleanupTestCase()
    {
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }

    // ── discoverPatches ───────────────────────────────────────────────────────

    void testDiscoverPatchesEmptyDir()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());
        QVERIFY(patches.isEmpty());
    }

    void testDiscoverPatchesSorted()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        writeSqlFile(dir, "003_c.sql", "SELECT 1;");
        writeSqlFile(dir, "001_a.sql", "SELECT 1;");
        writeSqlFile(dir, "002_b.sql", "SELECT 1;");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());

        QCOMPARE(patches.size(), 3);
        QCOMPARE(patches[0].id, QString("001_a"));
        QCOMPARE(patches[1].id, QString("002_b"));
        QCOMPARE(patches[2].id, QString("003_c"));
    }

    void testDiscoverPatchesIgnoresNonSql()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        writeSqlFile(dir, "001_valid.sql", "SELECT 1;");
        writeSqlFile(dir, "readme.txt", "ignore me");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());
        QCOMPARE(patches.size(), 1);
        QCOMPARE(patches[0].id, QString("001_valid"));
    }

    void testDiscoverPatchesChecksumNotEmpty()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        writeSqlFile(dir, "001_p.sql", "SELECT 1;");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());
        QCOMPARE(patches.size(), 1);
        QVERIFY(!patches[0].checksum.isEmpty());
    }

    // ── isPatchApplied / applyPendingPatches ──────────────────────────────────

    void testIsPatchAppliedReturnsFalseForUnknown()
    {
        ContentPatchService svc;
        QVERIFY(!svc.isPatchApplied("nonexistent_patch"));
    }

    void testApplyPendingPatchesSuccess()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        // A simple patch that inserts a topic row
        writeSqlFile(dir, "001_topic.sql",
                     "INSERT INTO topics (title, slug) VALUES ('TestTopic', 'test-topic');");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());
        QCOMPARE(patches.size(), 1);

        QString err;
        QVERIFY(svc.applyPendingPatches(patches, &err));
        QVERIFY(err.isEmpty());

        // Patch should now be recorded
        QVERIFY(svc.isPatchApplied("001_topic"));
    }

    void testApplyPendingPatchesSkipsAlreadyApplied()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        writeSqlFile(dir, "001_topic.sql",
                     "INSERT INTO topics (title, slug) VALUES ('TestTopic2', 'test-topic2');");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());

        // First apply
        QString err;
        QVERIFY(svc.applyPendingPatches(patches, &err));

        // Second apply should succeed without error (patch skipped)
        QVERIFY(svc.applyPendingPatches(patches, &err));
        QVERIFY(err.isEmpty());

        // Only one row in content_patches for this patch
        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.prepare("SELECT COUNT(*) FROM content_patches WHERE id = :id");
        q.bindValue(":id", "001_topic");
        QVERIFY(q.exec() && q.next());
        QCOMPARE(q.value(0).toInt(), 1);
    }

    void testApplyPendingPatchesFailsOnBadSql()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        writeSqlFile(dir, "099_bad.sql", "THIS IS NOT VALID SQL @@@@;");

        ContentPatchService svc;
        const auto patches = svc.discoverPatches(dir.path());

        QString err;
        const bool ok = svc.applyPendingPatches(patches, &err);
        QVERIFY(!ok);
        QVERIFY(!err.isEmpty());

        // Patch must NOT be recorded (rollback worked)
        QVERIFY(!svc.isPatchApplied("099_bad"));
    }

    // ── runSqlFile (public) ───────────────────────────────────────────────────

    void testRunSqlFileStrictFailsOnError()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString path = writeSqlFile(dir, "bad.sql", "BAD SQL;");
        QVERIFY(!path.isEmpty());

        const bool ok = QuizDatabase::instance().runSqlFile(path, /*strict=*/true);
        QVERIFY(!ok);
    }

    void testRunSqlFileNonStrictSucceedsOnError()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString path = writeSqlFile(dir, "warn.sql", "BAD SQL;");
        QVERIFY(!path.isEmpty());

        // Non-strict: errors are warnings, function returns true
        const bool ok = QuizDatabase::instance().runSqlFile(path, /*strict=*/false);
        QVERIFY(ok);
    }
};

QTEST_MAIN(ContentPatchServiceTest)
#include "test_content_patch_service.moc"
