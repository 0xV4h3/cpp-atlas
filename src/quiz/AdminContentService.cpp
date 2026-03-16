#include "quiz/AdminContentService.h"
#include "quiz/QuizDatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Singleton
// ─────────────────────────────────────────────────────────────────────────────

AdminContentService& AdminContentService::instance()
{
    static AdminContentService inst;
    return inst;
}

AdminContentService::AdminContentService(QObject* parent)
    : QObject(parent)
{}

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────

static QSqlDatabase adminDb()
{
    return QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
}

static AdminOpResult dbError(const QString& context, const QSqlError& err)
{
    const QString msg = QString("%1: %2").arg(context, err.text());
    qWarning() << "[AdminContentService]" << msg;
    return {false, msg, 0};
}

QString AdminContentService::validateDifficulty(const QVariantMap& m)
{
    if (!m.contains("difficulty")) return QString();
    const int d = m.value("difficulty").toInt();
    if (d < 1 || d > 4)
        return QString("difficulty must be in [1..4], got %1").arg(d);
    return QString();
}

// ─────────────────────────────────────────────────────────────────────────────
// Validation
// ─────────────────────────────────────────────────────────────────────────────

AdminOpResult AdminContentService::validateBeforeSave(const QVariantMap& payload) const
{
    // Required non-empty string fields if present
    static const QStringList stringFields = {"type", "content", "title"};
    for (const QString& f : stringFields) {
        if (payload.contains(f) && payload.value(f).toString().trimmed().isEmpty())
            return {false, QString("'%1' must not be empty").arg(f), 0};
    }

    // Difficulty range
    const QString diffErr = validateDifficulty(payload);
    if (!diffErr.isEmpty())
        return {false, diffErr, 0};

    // Known question types
    if (payload.contains("type")) {
        static const QStringList knownTypes = {
            "mcq", "multi_select", "true_false", "code_output", "fill_blank"
        };
        if (!knownTypes.contains(payload.value("type").toString()))
            return {false, QString("unknown question type '%1'")
                          .arg(payload.value("type").toString()), 0};
    }

    return {true, QString(), 0};
}

// ─────────────────────────────────────────────────────────────────────────────
// Questions
// ─────────────────────────────────────────────────────────────────────────────

AdminOpResult AdminContentService::createQuestion(const QVariantMap& payload)
{
    // Pre-validate
    const AdminOpResult v = validateBeforeSave(payload);
    if (!v.ok) return v;

    if (!payload.contains("type") || payload.value("type").toString().trimmed().isEmpty())
        return {false, "createQuestion: 'type' is required", 0};
    if (!payload.contains("content") || payload.value("content").toString().trimmed().isEmpty())
        return {false, "createQuestion: 'content' is required", 0};

    const int difficulty = payload.value("difficulty", 1).toInt();

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO questions "
        "(quiz_id, topic_id, type, content, code_snippet, explanation, "
        " difficulty, time_limit, points, order_index, hint, ref_url, is_active) "
        "VALUES "
        "(:quiz_id, :topic_id, :type, :content, :code_snippet, :explanation, "
        " :difficulty, :time_limit, :points, :order_index, :hint, :ref_url, 1)"
    );
    q.bindValue(":quiz_id",      payload.value("quiz_id"));
    q.bindValue(":topic_id",     payload.value("topic_id"));
    q.bindValue(":type",         payload.value("type").toString());
    q.bindValue(":content",      payload.value("content").toString());
    q.bindValue(":code_snippet", payload.value("code_snippet").toString());
    q.bindValue(":explanation",  payload.value("explanation").toString());
    q.bindValue(":difficulty",   difficulty);
    q.bindValue(":time_limit",   payload.value("time_limit", 0).toInt());
    q.bindValue(":points",       payload.value("points", 10).toInt());
    q.bindValue(":order_index",  payload.value("order_index", 0).toInt());
    q.bindValue(":hint",         payload.value("hint").toString());
    q.bindValue(":ref_url",      payload.value("ref_url").toString());

    if (!q.exec()) {
        db.rollback();
        return dbError("createQuestion INSERT", q.lastError());
    }

    if (!db.commit()) {
        db.rollback();
        return dbError("createQuestion COMMIT", db.lastError());
    }

    return {true, "Question created.", 1};
}

AdminOpResult AdminContentService::updateQuestion(int questionId, const QVariantMap& patch)
{
    if (patch.isEmpty())
        return {false, "updateQuestion: empty patch", 0};

    const QString diffErr = validateDifficulty(patch);
    if (!diffErr.isEmpty()) return {false, diffErr, 0};

    // Build SET clause dynamically from allowed columns
    static const QStringList allowed = {
        "type", "content", "code_snippet", "explanation",
        "difficulty", "time_limit", "points", "order_index",
        "hint", "ref_url", "quiz_id", "topic_id"
    };

    QStringList setClauses;
    QVariantMap bindValues;
    for (const QString& col : allowed) {
        if (patch.contains(col)) {
            setClauses << QString("%1 = :%1").arg(col);
            bindValues[col] = patch.value(col);
        }
    }
    if (setClauses.isEmpty())
        return {false, "updateQuestion: no recognized columns in patch", 0};

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(QString("UPDATE questions SET %1 WHERE id = :id AND is_active = 1")
              .arg(setClauses.join(", ")));
    for (auto it = bindValues.constBegin(); it != bindValues.constEnd(); ++it)
        q.bindValue(":" + it.key(), it.value());
    q.bindValue(":id", questionId);

    if (!q.exec()) {
        db.rollback();
        return dbError("updateQuestion UPDATE", q.lastError());
    }
    const int rows = q.numRowsAffected();
    if (!db.commit()) {
        db.rollback();
        return dbError("updateQuestion COMMIT", db.lastError());
    }
    return {true, QString("Question %1 updated.").arg(questionId), rows};
}

AdminOpResult AdminContentService::softDeleteQuestion(int questionId,
                                                       const QString& reason)
{
    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare("UPDATE questions SET is_active = 0 WHERE id = :id");
    q.bindValue(":id", questionId);

    if (!q.exec()) {
        db.rollback();
        return dbError("softDeleteQuestion UPDATE", q.lastError());
    }
    const int rows = q.numRowsAffected();

    if (!reason.isEmpty()) {
        QSqlQuery log(db);
        log.prepare(
            "INSERT INTO admin_deletion_log (entity_type, entity_id, reason) "
            "VALUES ('question', :id, :reason)"
        );
        log.bindValue(":id",     questionId);
        log.bindValue(":reason", reason);
        if (!log.exec()) {
            // Non-fatal: the deletion itself succeeded, just log the warning
            qWarning() << "[AdminContentService] Could not write deletion log:"
                       << log.lastError().text();
        }
    }

    if (!db.commit()) {
        db.rollback();
        return dbError("softDeleteQuestion COMMIT", db.lastError());
    }
    return {true, QString("Question %1 soft-deleted.").arg(questionId), rows};
}

// ─────────────────────────────────────────────────────────────────────────────
// Options
// ─────────────────────────────────────────────────────────────────────────────

AdminOpResult AdminContentService::createOption(int questionId, const QVariantMap& payload)
{
    if (!payload.contains("content") || payload.value("content").toString().trimmed().isEmpty())
        return {false, "createOption: 'content' is required", 0};

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO options (question_id, content, code_snippet, is_correct, order_index) "
        "VALUES (:question_id, :content, :code_snippet, :is_correct, :order_index)"
    );
    q.bindValue(":question_id",  questionId);
    q.bindValue(":content",      payload.value("content").toString());
    q.bindValue(":code_snippet", payload.value("code_snippet").toString());
    q.bindValue(":is_correct",   payload.value("is_correct", false).toBool() ? 1 : 0);
    q.bindValue(":order_index",  payload.value("order_index", 0).toInt());

    if (!q.exec()) {
        db.rollback();
        return dbError("createOption INSERT", q.lastError());
    }
    if (!db.commit()) {
        db.rollback();
        return dbError("createOption COMMIT", db.lastError());
    }
    return {true, "Option created.", 1};
}

AdminOpResult AdminContentService::updateOption(int optionId, const QVariantMap& patch)
{
    if (patch.isEmpty())
        return {false, "updateOption: empty patch", 0};

    static const QStringList allowed = {
        "content", "code_snippet", "is_correct", "order_index"
    };

    QStringList setClauses;
    QVariantMap bindValues;
    for (const QString& col : allowed) {
        if (patch.contains(col)) {
            setClauses << QString("%1 = :%1").arg(col);
            bindValues[col] = patch.value(col);
        }
    }
    if (setClauses.isEmpty())
        return {false, "updateOption: no recognized columns in patch", 0};

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(QString("UPDATE options SET %1 WHERE id = :id")
              .arg(setClauses.join(", ")));
    for (auto it = bindValues.constBegin(); it != bindValues.constEnd(); ++it)
        q.bindValue(":" + it.key(), it.value());
    q.bindValue(":id", optionId);

    if (!q.exec()) {
        db.rollback();
        return dbError("updateOption UPDATE", q.lastError());
    }
    const int rows = q.numRowsAffected();
    if (!db.commit()) {
        db.rollback();
        return dbError("updateOption COMMIT", db.lastError());
    }
    return {true, QString("Option %1 updated.").arg(optionId), rows};
}

AdminOpResult AdminContentService::deleteOption(int optionId)
{
    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare("DELETE FROM options WHERE id = :id");
    q.bindValue(":id", optionId);

    if (!q.exec()) {
        db.rollback();
        return dbError("deleteOption DELETE", q.lastError());
    }
    const int rows = q.numRowsAffected();
    if (!db.commit()) {
        db.rollback();
        return dbError("deleteOption COMMIT", db.lastError());
    }
    return {true, QString("Option %1 deleted.").arg(optionId), rows};
}

// ─────────────────────────────────────────────────────────────────────────────
// Quizzes
// ─────────────────────────────────────────────────────────────────────────────

AdminOpResult AdminContentService::createQuiz(const QVariantMap& payload)
{
    const AdminOpResult v = validateBeforeSave(payload);
    if (!v.ok) return v;

    if (!payload.contains("title") || payload.value("title").toString().trimmed().isEmpty())
        return {false, "createQuiz: 'title' is required", 0};

    const int difficulty = payload.value("difficulty", 1).toInt();

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO quizzes "
        "(title, description, topic_id, difficulty, time_limit, is_timed, type, is_active) "
        "VALUES "
        "(:title, :description, :topic_id, :difficulty, :time_limit, :is_timed, :type, 1)"
    );
    q.bindValue(":title",       payload.value("title").toString());
    q.bindValue(":description", payload.value("description").toString());
    q.bindValue(":topic_id",    payload.value("topic_id"));
    q.bindValue(":difficulty",  difficulty);
    q.bindValue(":time_limit",  payload.value("time_limit", 0).toInt());
    q.bindValue(":is_timed",    payload.value("is_timed", false).toBool() ? 1 : 0);
    q.bindValue(":type",        payload.value("type", "standard").toString());

    if (!q.exec()) {
        db.rollback();
        return dbError("createQuiz INSERT", q.lastError());
    }
    if (!db.commit()) {
        db.rollback();
        return dbError("createQuiz COMMIT", db.lastError());
    }
    return {true, "Quiz created.", 1};
}

AdminOpResult AdminContentService::updateQuiz(int quizId, const QVariantMap& patch)
{
    if (patch.isEmpty())
        return {false, "updateQuiz: empty patch", 0};

    const QString diffErr = validateDifficulty(patch);
    if (!diffErr.isEmpty()) return {false, diffErr, 0};

    static const QStringList allowed = {
        "title", "description", "topic_id", "difficulty",
        "time_limit", "is_timed", "type"
    };

    QStringList setClauses;
    QVariantMap bindValues;
    for (const QString& col : allowed) {
        if (patch.contains(col)) {
            setClauses << QString("%1 = :%1").arg(col);
            bindValues[col] = patch.value(col);
        }
    }
    if (setClauses.isEmpty())
        return {false, "updateQuiz: no recognized columns in patch", 0};

    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare(QString("UPDATE quizzes SET %1 WHERE id = :id AND is_active = 1")
              .arg(setClauses.join(", ")));
    for (auto it = bindValues.constBegin(); it != bindValues.constEnd(); ++it)
        q.bindValue(":" + it.key(), it.value());
    q.bindValue(":id", quizId);

    if (!q.exec()) {
        db.rollback();
        return dbError("updateQuiz UPDATE", q.lastError());
    }
    const int rows = q.numRowsAffected();
    if (!db.commit()) {
        db.rollback();
        return dbError("updateQuiz COMMIT", db.lastError());
    }
    return {true, QString("Quiz %1 updated.").arg(quizId), rows};
}

AdminOpResult AdminContentService::softDeleteQuiz(int quizId, const QString& reason)
{
    QSqlDatabase db = adminDb();
    db.transaction();

    QSqlQuery q(db);
    q.prepare("UPDATE quizzes SET is_active = 0 WHERE id = :id");
    q.bindValue(":id", quizId);

    if (!q.exec()) {
        db.rollback();
        return dbError("softDeleteQuiz UPDATE", q.lastError());
    }
    const int rows = q.numRowsAffected();

    if (!reason.isEmpty()) {
        QSqlQuery log(db);
        log.prepare(
            "INSERT INTO admin_deletion_log (entity_type, entity_id, reason) "
            "VALUES ('quiz', :id, :reason)"
        );
        log.bindValue(":id",     quizId);
        log.bindValue(":reason", reason);
        if (!log.exec())
            qWarning() << "[AdminContentService] Could not write deletion log:"
                       << log.lastError().text();
    }

    if (!db.commit()) {
        db.rollback();
        return dbError("softDeleteQuiz COMMIT", db.lastError());
    }
    return {true, QString("Quiz %1 soft-deleted.").arg(quizId), rows};
}
