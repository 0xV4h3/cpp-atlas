#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "quiz/AdminContentService.h"
#include "quiz/QuizDatabase.h"

class AdminContentServiceTest : public QObject
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
            "CREATE TABLE IF NOT EXISTS quizzes ("
            "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  title       TEXT NOT NULL,"
            "  description TEXT,"
            "  topic_id    INTEGER,"
            "  difficulty  INTEGER DEFAULT 1,"
            "  time_limit  INTEGER DEFAULT 0,"
            "  is_timed    INTEGER DEFAULT 0,"
            "  type        TEXT    DEFAULT 'standard',"
            "  is_active   INTEGER DEFAULT 1"
            ")"
        ));
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS questions ("
            "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  quiz_id     INTEGER,"
            "  topic_id    INTEGER,"
            "  type        TEXT NOT NULL,"
            "  content     TEXT NOT NULL,"
            "  code_snippet TEXT DEFAULT '',"
            "  explanation TEXT DEFAULT '',"
            "  difficulty  INTEGER DEFAULT 1,"
            "  time_limit  INTEGER DEFAULT 0,"
            "  points      INTEGER DEFAULT 10,"
            "  order_index INTEGER DEFAULT 0,"
            "  hint        TEXT DEFAULT '',"
            "  ref_url     TEXT DEFAULT '',"
            "  is_active   INTEGER DEFAULT 1"
            ")"
        ));
        QVERIFY(q.exec(
            "CREATE TABLE IF NOT EXISTS options ("
            "  id           INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  question_id  INTEGER NOT NULL,"
            "  content      TEXT NOT NULL,"
            "  code_snippet TEXT DEFAULT '',"
            "  is_correct   INTEGER DEFAULT 0,"
            "  order_index  INTEGER DEFAULT 0"
            ")"
        ));
        // admin_deletion_log is optional; use IF NOT EXISTS
        q.exec(
            "CREATE TABLE IF NOT EXISTS admin_deletion_log ("
            "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  entity_type TEXT,"
            "  entity_id   INTEGER,"
            "  reason      TEXT,"
            "  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP"
            ")"
        );
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

    // ── Validation ───────────────────────────────────────────────────────────

    void validateEmptyContent()
    {
        QVariantMap p;
        p["type"]       = "mcq";
        p["content"]    = "   ";
        p["difficulty"] = 2;
        const AdminOpResult r = AdminContentService::instance().validateBeforeSave(p);
        QVERIFY(!r.ok);
        QVERIFY(r.message.contains("content"));
    }

    void validateDifficultyTooLow()
    {
        QVariantMap p;
        p["difficulty"] = 0;
        const AdminOpResult r = AdminContentService::instance().validateBeforeSave(p);
        QVERIFY(!r.ok);
        QVERIFY(r.message.contains("difficulty"));
    }

    void validateDifficultyTooHigh()
    {
        QVariantMap p;
        p["difficulty"] = 5;
        const AdminOpResult r = AdminContentService::instance().validateBeforeSave(p);
        QVERIFY(!r.ok);
        QVERIFY(r.message.contains("difficulty"));
    }

    void validateDifficultyValid()
    {
        QVariantMap p;
        p["difficulty"] = 3;
        const AdminOpResult r = AdminContentService::instance().validateBeforeSave(p);
        QVERIFY(r.ok);
    }

    void validateUnknownType()
    {
        QVariantMap p;
        p["type"] = "banana";
        const AdminOpResult r = AdminContentService::instance().validateBeforeSave(p);
        QVERIFY(!r.ok);
    }

    // ── Question CRUD ────────────────────────────────────────────────────────

    void createQuestionSuccess()
    {
        QVariantMap p;
        p["type"]       = "mcq";
        p["content"]    = "What is 2+2?";
        p["difficulty"] = 1;
        const AdminOpResult r = AdminContentService::instance().createQuestion(p);
        QVERIFY(r.ok);
        QCOMPARE(r.affectedRows, 1);
    }

    void createQuestionMissingContent()
    {
        QVariantMap p;
        p["type"]       = "mcq";
        p["difficulty"] = 1;
        const AdminOpResult r = AdminContentService::instance().createQuestion(p);
        QVERIFY(!r.ok);
    }

    void createQuestionBadDifficulty()
    {
        QVariantMap p;
        p["type"]       = "fill_blank";
        p["content"]    = "Fill in: ___";
        p["difficulty"] = 99;
        const AdminOpResult r = AdminContentService::instance().createQuestion(p);
        QVERIFY(!r.ok);
        QVERIFY(r.message.contains("difficulty"));
    }

    void updateQuestionSuccess()
    {
        // First create a question
        QVariantMap create;
        create["type"]       = "mcq";
        create["content"]    = "Original content";
        create["difficulty"] = 1;
        const AdminOpResult created = AdminContentService::instance().createQuestion(create);
        QVERIFY(created.ok);

        // Get its id
        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.exec("SELECT MAX(id) FROM questions");
        QVERIFY(q.next());
        const int qid = q.value(0).toInt();

        QVariantMap patch;
        patch["content"]    = "Updated content";
        patch["difficulty"] = 2;
        const AdminOpResult r = AdminContentService::instance().updateQuestion(qid, patch);
        QVERIFY(r.ok);
    }

    void softDeleteQuestion()
    {
        QVariantMap create;
        create["type"]       = "mcq";
        create["content"]    = "To be deleted";
        create["difficulty"] = 1;
        QVERIFY(AdminContentService::instance().createQuestion(create).ok);

        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.exec("SELECT MAX(id) FROM questions");
        QVERIFY(q.next());
        const int qid = q.value(0).toInt();

        const AdminOpResult r = AdminContentService::instance()
            .softDeleteQuestion(qid, "Test soft delete");
        QVERIFY(r.ok);

        // Verify is_active == 0
        QSqlQuery check(db);
        check.prepare("SELECT is_active FROM questions WHERE id = :id");
        check.bindValue(":id", qid);
        QVERIFY(check.exec() && check.next());
        QCOMPARE(check.value(0).toInt(), 0);
    }

    // ── Option CRUD ──────────────────────────────────────────────────────────

    void createOptionSuccess()
    {
        // Create a parent question first
        QVariantMap qp;
        qp["type"]       = "mcq";
        qp["content"]    = "Parent question";
        qp["difficulty"] = 1;
        QVERIFY(AdminContentService::instance().createQuestion(qp).ok);

        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.exec("SELECT MAX(id) FROM questions");
        QVERIFY(q.next());
        const int qid = q.value(0).toInt();

        QVariantMap op;
        op["content"]    = "Option A";
        op["is_correct"] = false;
        const AdminOpResult r = AdminContentService::instance().createOption(qid, op);
        QVERIFY(r.ok);
    }

    void deleteOptionSuccess()
    {
        // Insert directly for speed
        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery ins(db);
        ins.exec("INSERT INTO options (question_id, content, is_correct) VALUES (1, 'Temp', 0)");
        const int oid = ins.lastInsertId().toInt();

        const AdminOpResult r = AdminContentService::instance().deleteOption(oid);
        QVERIFY(r.ok);
    }

    // ── Quiz CRUD ────────────────────────────────────────────────────────────

    void createQuizSuccess()
    {
        QVariantMap p;
        p["title"]      = "Test Quiz";
        p["difficulty"] = 2;
        const AdminOpResult r = AdminContentService::instance().createQuiz(p);
        QVERIFY(r.ok);
        QCOMPARE(r.affectedRows, 1);
    }

    void createQuizMissingTitle()
    {
        QVariantMap p;
        p["difficulty"] = 1;
        const AdminOpResult r = AdminContentService::instance().createQuiz(p);
        QVERIFY(!r.ok);
    }

    void softDeleteQuiz()
    {
        QVariantMap create;
        create["title"]      = "Quiz to delete";
        create["difficulty"] = 1;
        QVERIFY(AdminContentService::instance().createQuiz(create).ok);

        QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
        QSqlQuery q(db);
        q.exec("SELECT MAX(id) FROM quizzes");
        QVERIFY(q.next());
        const int qzid = q.value(0).toInt();

        const AdminOpResult r = AdminContentService::instance()
            .softDeleteQuiz(qzid, "test reason");
        QVERIFY(r.ok);

        QSqlQuery check(db);
        check.prepare("SELECT is_active FROM quizzes WHERE id = :id");
        check.bindValue(":id", qzid);
        QVERIFY(check.exec() && check.next());
        QCOMPARE(check.value(0).toInt(), 0);
    }
};

QTEST_MAIN(AdminContentServiceTest)
#include "test_admin_content_service.moc"
