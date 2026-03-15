#include <QtTest/QtTest>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFile>
#include <QTextStream>

#include "QuizAdminCli.h"
#include "quiz/QuizDatabase.h"

// ─────────────────────────────────────────────────────────────────────────────
// Helper: build a minimal in-memory database for CLI tests
// ─────────────────────────────────────────────────────────────────────────────
static void initInMemoryDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                                QuizDatabase::CONNECTION_NAME);
    db.setDatabaseName(":memory:");
    if (!db.open()) return;

    QSqlQuery q(db);
    q.exec("CREATE TABLE IF NOT EXISTS schema_version (version INTEGER, description TEXT)");
    q.exec("INSERT INTO schema_version VALUES (3, 'test')");
    q.exec("CREATE TABLE IF NOT EXISTS topics (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, slug TEXT UNIQUE)");
    q.exec("CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)");
    q.exec("CREATE TABLE IF NOT EXISTS quizzes (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, is_active INTEGER DEFAULT 1)");
    q.exec("CREATE TABLE IF NOT EXISTS questions (id INTEGER PRIMARY KEY AUTOINCREMENT, quiz_id INTEGER, type TEXT, is_active INTEGER DEFAULT 1)");
    q.exec("CREATE TABLE IF NOT EXISTS options (id INTEGER PRIMARY KEY AUTOINCREMENT, question_id INTEGER, is_correct INTEGER DEFAULT 0)");
    q.exec("CREATE TABLE IF NOT EXISTS question_tags (question_id INTEGER, tag_id INTEGER, PRIMARY KEY(question_id, tag_id))");
    q.exec("CREATE TABLE IF NOT EXISTS quiz_tags (quiz_id INTEGER, tag_id INTEGER, PRIMARY KEY(quiz_id, tag_id))");
    q.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, display_name TEXT, password_hash TEXT, salt TEXT, is_admin INTEGER DEFAULT 0)");
    q.exec("CREATE TABLE IF NOT EXISTS quiz_sessions (id INTEGER PRIMARY KEY AUTOINCREMENT, is_complete INTEGER DEFAULT 0)");
    q.exec("CREATE TABLE IF NOT EXISTS content_patches (id TEXT PRIMARY KEY, applied_at DATETIME DEFAULT CURRENT_TIMESTAMP, description TEXT, checksum TEXT)");

    // Seed a topic and a tag so export has something to dump
    q.exec("INSERT INTO topics (title, slug) VALUES ('C++ Basics', 'cpp-basics')");
    q.exec("INSERT INTO tags (name) VALUES ('oop')");
}

// ─────────────────────────────────────────────────────────────────────────────
// Helper: build a temp DB file (used by openDatabase path)
// ─────────────────────────────────────────────────────────────────────────────
static QString buildTempDbFile()
{
    // Close any existing connection first
    if (QSqlDatabase::contains(QuizDatabase::CONNECTION_NAME)) {
        QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
        QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
    }

    QTemporaryFile tmp;
    tmp.setAutoRemove(false);
    if (!tmp.open()) return QString();
    const QString path = tmp.fileName();
    tmp.close();

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "cli_setup_conn");
        db.setDatabaseName(path);
        if (!db.open()) return QString();
        QSqlQuery q(db);
        q.exec("CREATE TABLE IF NOT EXISTS schema_version (version INTEGER, description TEXT)");
        q.exec("INSERT INTO schema_version VALUES (3, 'test')");
        q.exec("CREATE TABLE IF NOT EXISTS topics (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, slug TEXT UNIQUE)");
        q.exec("CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)");
        q.exec("CREATE TABLE IF NOT EXISTS quizzes (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, is_active INTEGER DEFAULT 1)");
        q.exec("CREATE TABLE IF NOT EXISTS questions (id INTEGER PRIMARY KEY AUTOINCREMENT, quiz_id INTEGER, type TEXT, is_active INTEGER DEFAULT 1)");
        q.exec("CREATE TABLE IF NOT EXISTS options (id INTEGER PRIMARY KEY AUTOINCREMENT, question_id INTEGER, is_correct INTEGER DEFAULT 0)");
        q.exec("CREATE TABLE IF NOT EXISTS question_tags (question_id INTEGER, tag_id INTEGER, PRIMARY KEY(question_id, tag_id))");
        q.exec("CREATE TABLE IF NOT EXISTS quiz_tags (quiz_id INTEGER, tag_id INTEGER, PRIMARY KEY(quiz_id, tag_id))");
        q.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, display_name TEXT, password_hash TEXT, salt TEXT, is_admin INTEGER DEFAULT 0)");
        q.exec("CREATE TABLE IF NOT EXISTS quiz_sessions (id INTEGER PRIMARY KEY AUTOINCREMENT, is_complete INTEGER DEFAULT 0)");
        q.exec("CREATE TABLE IF NOT EXISTS content_patches (id TEXT PRIMARY KEY, applied_at DATETIME DEFAULT CURRENT_TIMESTAMP, description TEXT, checksum TEXT)");
        q.exec("INSERT INTO topics (title, slug) VALUES ('C++ Basics', 'cpp-basics')");
        db.close();
    }
    QSqlDatabase::removeDatabase("cli_setup_conn");
    return path;
}

class QuizAdminCliTest : public QObject
{
    Q_OBJECT

private slots:
    void cleanupTestCase()
    {
        if (QSqlDatabase::contains(QuizDatabase::CONNECTION_NAME)) {
            QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
            QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
        }
    }

    void cleanup()
    {
        // Remove the CLI connection after each test so the next test can re-open
        if (QSqlDatabase::contains(QuizDatabase::CONNECTION_NAME)) {
            QSqlDatabase::database(QuizDatabase::CONNECTION_NAME).close();
            QSqlDatabase::removeDatabase(QuizDatabase::CONNECTION_NAME);
        }
    }

    // ── Arg-parse: no command → exit 1 ───────────────────────────────────────

    void testNoCommandReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── Arg-parse: unknown command → exit 1 ──────────────────────────────────

    void testUnknownCommandReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "notacommand"});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── Arg-parse: --help exits 0 ─────────────────────────────────────────────

    void testHelpExitsZero()
    {
        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--help"});
        QCOMPARE(rc, 0);
    }

    // ── stats ─────────────────────────────────────────────────────────────────

    void testStatsExitsZero()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "stats"});
        QCOMPARE(rc, 0);
        QFile::remove(dbPath);
    }

    // ── validate: missing --content-dir → exit 1 ─────────────────────────────

    void testValidateMissingDirReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "validate"});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── validate: empty dir → exit 0 ─────────────────────────────────────────

    void testValidateEmptyDirExitsZero()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QTemporaryDir patchDir;
        QVERIFY(patchDir.isValid());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "validate",
                                "--content-dir", patchDir.path()});
        QCOMPARE(rc, 0);
        QFile::remove(dbPath);
    }

    // ── validate: non-existent directory → exit 1 ────────────────────────────

    void testValidateNonExistentDirReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        // Use a path that is guaranteed not to exist
        const QString nonExistentDir = QDir::tempPath() + "/cppatlas_no_such_dir_xyzzy_12345";
        QDir().rmdir(nonExistentDir); // ensure it really doesn't exist

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "validate",
                                "--content-dir", nonExistentDir});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── apply-content: missing --content-dir → exit 1 ────────────────────────

    void testApplyContentMissingDirReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "apply-content"});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── apply-content: applies a valid patch ─────────────────────────────────

    void testApplyContentSuccess()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QTemporaryDir patchDir;
        QVERIFY(patchDir.isValid());

        // Write a valid patch
        {
            QFile f(patchDir.filePath("001_add_tag.sql"));
            QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Text));
            QTextStream out(&f);
            out << "INSERT INTO tags (name) VALUES ('concurrency');\n";
        }

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "apply-content",
                                "--content-dir", patchDir.path()});
        QCOMPARE(rc, 0);
        QFile::remove(dbPath);
    }

    // ── apply-content: bad SQL patch → exit 2 ────────────────────────────────

    void testApplyContentBadSqlReturnsTwo()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QTemporaryDir patchDir;
        QVERIFY(patchDir.isValid());

        {
            QFile f(patchDir.filePath("001_bad.sql"));
            QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Text));
            QTextStream out(&f);
            out << "THIS IS NOT VALID SQL @@@@;\n";
        }

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "apply-content",
                                "--content-dir", patchDir.path()});
        QCOMPARE(rc, 2);
        QFile::remove(dbPath);
    }

    // ── export: missing --out → exit 1 ───────────────────────────────────────

    void testExportMissingOutReturnsOne()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "export"});
        QCOMPARE(rc, 1);
        QFile::remove(dbPath);
    }

    // ── export: writes a non-empty SQL dump ───────────────────────────────────

    void testExportWritesDump()
    {
        const QString dbPath = buildTempDbFile();
        QVERIFY(!dbPath.isEmpty());

        QTemporaryFile outFile;
        QVERIFY(outFile.open());
        const QString outPath = outFile.fileName();
        outFile.close();

        QuizAdminCli cli;
        const int rc = cli.run({"quiz_admin", "--db", dbPath, "export", "--out", outPath});
        QCOMPARE(rc, 0);
        QVERIFY(QFile::exists(outPath));
        QVERIFY(QFileInfo(outPath).size() > 0);
        QFile::remove(dbPath);
        QFile::remove(outPath);
    }
};

QTEST_MAIN(QuizAdminCliTest)
#include "test_quiz_admin_cli.moc"
