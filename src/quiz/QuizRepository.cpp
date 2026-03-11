#include "quiz/QuizRepository.h"
#include "quiz/QuizDatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Helper: get open DB
// ─────────────────────────────────────────────────────────────────────────────
static QSqlDatabase db()
{
    return QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────
TopicDTO QuizRepository::topicFromQuery(QSqlQuery& q) const
{
    TopicDTO t;
    t.id          = q.value("id").toInt();
    t.slug        = q.value("slug").toString();
    t.title       = q.value("title").toString();
    t.description = q.value("description").toString();
    t.parentId    = q.value("parent_id").isNull() ? -1 : q.value("parent_id").toInt();
    t.level       = q.value("level").toInt();
    t.difficulty  = q.value("difficulty").toInt();
    t.orderIndex  = q.value("order_index").toInt();
    t.icon        = q.value("icon").toString();
    t.refUrl      = q.value("ref_url").toString();
    t.refUrl2     = q.value("ref_url2").toString();
    return t;
}

QuizDTO QuizRepository::quizFromQuery(QSqlQuery& q) const
{
    QuizDTO qz;
    qz.id           = q.value("id").toInt();
    qz.title        = q.value("title").toString();
    qz.description  = q.value("description").toString();
    qz.topicId      = q.value("topic_id").isNull() ? -1 : q.value("topic_id").toInt();
    qz.difficulty   = q.value("difficulty").toInt();
    qz.timeLimitSec = q.value("time_limit").toInt();
    qz.isTimed      = q.value("is_timed").toInt() == 1;
    qz.type         = q.value("type").toString();
    qz.isActive     = q.value("is_active").toInt() == 1;
    return qz;
}

QuestionDTO QuizRepository::questionFromQuery(QSqlQuery& q) const
{
    QuestionDTO qst;
    qst.id           = q.value("id").toInt();
    qst.quizId       = q.value("quiz_id").isNull() ? -1 : q.value("quiz_id").toInt();
    qst.topicId      = q.value("topic_id").isNull() ? -1 : q.value("topic_id").toInt();
    qst.type         = q.value("type").toString();
    qst.content      = q.value("content").toString();
    qst.codeSnippet  = q.value("code_snippet").toString();
    qst.explanation  = q.value("explanation").toString();
    qst.difficulty   = q.value("difficulty").toInt();
    qst.timeLimitSec = q.value("time_limit").toInt();
    qst.points       = q.value("points").toInt();
    qst.orderIndex   = q.value("order_index").toInt();
    qst.hint         = q.value("hint").toString();
    qst.refUrl       = q.value("ref_url").toString();
    return qst;
}

QList<OptionDTO> QuizRepository::loadOptions(int questionId) const
{
    QList<OptionDTO> opts;
    QSqlQuery q(db());
    q.prepare("SELECT id, question_id, content, code_snippet, is_correct, order_index "
              "FROM options WHERE question_id = :qid ORDER BY order_index");
    q.bindValue(":qid", questionId);
    if (!q.exec()) {
        qWarning() << "[QuizRepository] loadOptions failed:" << q.lastError().text();
        return opts;
    }
    while (q.next()) {
        OptionDTO o;
        o.id          = q.value("id").toInt();
        o.questionId  = q.value("question_id").toInt();
        o.content     = q.value("content").toString();
        o.codeSnippet = q.value("code_snippet").toString();
        o.isCorrect   = q.value("is_correct").toInt() == 1;
        o.orderIndex  = q.value("order_index").toInt();
        opts << o;
    }
    return opts;
}

QStringList QuizRepository::loadTagsForQuestion(int questionId) const
{
    QStringList tags;
    QSqlQuery q(db());
    q.prepare("SELECT t.name FROM tags t "
              "JOIN question_tags qt ON qt.tag_id = t.id "
              "WHERE qt.question_id = :qid");
    q.bindValue(":qid", questionId);
    if (q.exec()) {
        while (q.next()) tags << q.value(0).toString();
    }
    return tags;
}

QuestionDTO QuizRepository::loadQuestionWithOptions(int questionId) const
{
    QSqlQuery q(db());
    q.prepare("SELECT * FROM questions WHERE id = :id");
    q.bindValue(":id", questionId);
    if (!q.exec() || !q.next()) return QuestionDTO{};
    QuestionDTO qst = questionFromQuery(q);
    qst.options = loadOptions(questionId);
    qst.tags    = loadTagsForQuestion(questionId);
    return qst;
}

// ─────────────────────────────────────────────────────────────────────────────
// Topics
// ─────────────────────────────────────────────────────────────────────────────
QList<TopicDTO> QuizRepository::allTopics() const
{
    QList<TopicDTO> list;
    QSqlQuery q(db());
    q.exec("SELECT * FROM topics ORDER BY order_index, id");
    while (q.next()) list << topicFromQuery(q);
    return list;
}

QList<TopicDTO> QuizRepository::topicsByDifficulty(int difficulty) const
{
    QList<TopicDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT * FROM topics WHERE difficulty = :d ORDER BY order_index");
    q.bindValue(":d", difficulty);
    if (q.exec()) while (q.next()) list << topicFromQuery(q);
    return list;
}

QList<TopicDTO> QuizRepository::childTopics(int parentId) const
{
    QList<TopicDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT * FROM topics WHERE parent_id = :pid ORDER BY order_index");
    q.bindValue(":pid", parentId);
    if (q.exec()) while (q.next()) list << topicFromQuery(q);
    return list;
}

QList<TopicDTO> QuizRepository::rootTopics() const
{
    QList<TopicDTO> list;
    QSqlQuery q(db());
    q.exec("SELECT * FROM topics WHERE parent_id IS NULL ORDER BY order_index");
    while (q.next()) list << topicFromQuery(q);
    return list;
}

TopicDTO QuizRepository::topicById(int id) const
{
    QSqlQuery q(db());
    q.prepare("SELECT * FROM topics WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) return topicFromQuery(q);
    return TopicDTO{};
}

TopicDTO QuizRepository::topicBySlug(const QString& slug) const
{
    QSqlQuery q(db());
    q.prepare("SELECT * FROM topics WHERE slug = :slug");
    q.bindValue(":slug", slug);
    if (q.exec() && q.next()) return topicFromQuery(q);
    return TopicDTO{};
}

// ────────────��────────────────────────────────────────────────────────────────
// Tags
// ─────────────────────────────────────────────────────────────────────────────
QList<TagDTO> QuizRepository::allTags() const
{
    QList<TagDTO> list;
    QSqlQuery q(db());
    q.exec("SELECT * FROM tags ORDER BY name");
    while (q.next()) {
        TagDTO t;
        t.id    = q.value("id").toInt();
        t.name  = q.value("name").toString();
        t.color = q.value("color").toString();
        list << t;
    }
    return list;
}

QList<TagDTO> QuizRepository::tagsForQuestion(int questionId) const
{
    QList<TagDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT t.id, t.name, t.color FROM tags t "
              "JOIN question_tags qt ON qt.tag_id = t.id "
              "WHERE qt.question_id = :qid");
    q.bindValue(":qid", questionId);
    if (q.exec()) {
        while (q.next()) {
            TagDTO t;
            t.id    = q.value("id").toInt();
            t.name  = q.value("name").toString();
            t.color = q.value("color").toString();
            list << t;
        }
    }
    return list;
}

QList<TagDTO> QuizRepository::tagsForQuiz(int quizId) const
{
    QList<TagDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT t.id, t.name, t.color FROM tags t "
              "JOIN quiz_tags qt ON qt.tag_id = t.id "
              "WHERE qt.quiz_id = :qid");
    q.bindValue(":qid", quizId);
    if (q.exec()) {
        while (q.next()) {
            TagDTO t;
            t.id    = q.value("id").toInt();
            t.name  = q.value("name").toString();
            t.color = q.value("color").toString();
            list << t;
        }
    }
    return list;
}

// ─────────────────────────────────────────────────────────────────────────────
// Quizzes
// ─────────────────────────────────────────────────────────────────────────────
QStringList QuizRepository::loadTagsForQuiz(int quizId) const {
    QStringList tags;
    QSqlQuery q(db());
    q.prepare("SELECT t.name FROM tags t "
              "JOIN quiz_tags qt ON qt.tag_id = t.id WHERE qt.quiz_id = :id");
    q.bindValue(":id", quizId);
    if (q.exec())
        while (q.next())
            tags << q.value(0).toString();
    return tags;
}

QList<QuizDTO> QuizRepository::allActiveQuizzes() const
{
    QList<QuizDTO> list;
    QSqlQuery q(db());
    q.exec(
        "SELECT qz.*, COUNT(qu.id) AS qcount "
        "FROM quizzes qz "
        "LEFT JOIN questions qu ON qu.quiz_id = qz.id AND qu.is_active = 1 "
        "WHERE qz.is_active = 1 "
        "GROUP BY qz.id "
        "ORDER BY qz.difficulty, qz.id"
    );
    while (q.next()) {
        QuizDTO qz = quizFromQuery(q);
        qz.questionCount = q.value("qcount").toInt();
        qz.tags = loadTagsForQuiz(qz.id);
        list << qz;
    }
    return list;
}

QList<QuizDTO> QuizRepository::quizzesByTopic(int topicId) const
{
    QList<QuizDTO> list;
    QSqlQuery q(db());
    q.prepare(
        "SELECT qz.*, COUNT(qu.id) AS qcount "
        "FROM quizzes qz "
        "LEFT JOIN questions qu ON qu.quiz_id = qz.id AND qu.is_active = 1 "
        "WHERE qz.topic_id = :tid AND qz.is_active = 1 "
        "GROUP BY qz.id "
        "ORDER BY qz.difficulty"
    );
    q.bindValue(":tid", topicId);
    if (!q.exec()) return list;
    while (q.next()) {
        QuizDTO qz = quizFromQuery(q);
        qz.questionCount = q.value("qcount").toInt();
        qz.tags = loadTagsForQuiz(qz.id);
        list << qz;
    }
    return list;
}

QList<QuizDTO> QuizRepository::quizzesByDifficulty(int difficulty) const
{
    QList<QuizDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT * FROM quizzes WHERE difficulty = :d AND is_active = 1");
    q.bindValue(":d", difficulty);
    if (q.exec()) {
        while (q.next()) {
            QuizDTO qz = quizFromQuery(q);
            qz.tags = loadTagsForQuiz(qz.id);
            list << qz;
        }
    }
    return list;
}

QList<QuizDTO> QuizRepository::quizzesByTag(const QString& tagName) const
{
    QList<QuizDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT qz.* FROM quizzes qz "
              "JOIN quiz_tags qt ON qt.quiz_id = qz.id "
              "JOIN tags t ON t.id = qt.tag_id "
              "WHERE t.name = :name AND qz.is_active = 1");
    q.bindValue(":name", tagName);
    if (q.exec()) {
        while (q.next()) {
            QuizDTO qz = quizFromQuery(q);
            qz.tags = loadTagsForQuiz(qz.id);
            list << qz;
        }
    }
    return list;
}

QuizDTO QuizRepository::quizById(int id) const
{
    QSqlQuery q(db());
    q.prepare("SELECT * FROM quizzes WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        QuizDTO qz = quizFromQuery(q);
        qz.tags = loadTagsForQuiz(qz.id);
        return qz;
    }
    return QuizDTO{};
}

// ─────────────────────────────────────────────────────────────────────────────
// Questions
// ─────────────────────────────────────────────────────────────────────────────
QList<QuestionDTO> QuizRepository::questionsForQuiz(int quizId) const
{
    QList<QuestionDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT * FROM questions WHERE quiz_id = :qid AND is_active = 1 "
              "ORDER BY order_index");
    q.bindValue(":qid", quizId);
    if (!q.exec()) {
        qWarning() << "[QuizRepository] questionsForQuiz failed:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        QuestionDTO qst = questionFromQuery(q);
        qst.options = loadOptions(qst.id);
        qst.tags    = loadTagsForQuestion(qst.id);
        list << qst;
    }
    return list;
}

QList<QuestionDTO> QuizRepository::questionsForQuizShuffled(int quizId) const
{
    QList<QuestionDTO> list = questionsForQuiz(quizId);
    // Fisher-Yates shuffle
    for (int i = list.size() - 1; i > 0; --i) {
        int j = static_cast<int>(QRandomGenerator::global()->bounded(i + 1));
        list.swapItemsAt(i, j);
    }
    // Also shuffle each question's options
    for (auto& qst : list) {
        for (int i = qst.options.size() - 1; i > 0; --i) {
            int j = static_cast<int>(QRandomGenerator::global()->bounded(i + 1));
            qst.options.swapItemsAt(i, j);
        }
    }
    return list;
}

QList<QuestionDTO> QuizRepository::randomQuestions(const QList<int>& topicIds,
                                                     int count,
                                                     int maxDifficulty) const
{
    if (topicIds.isEmpty() || count <= 0) return {};

    // Build IN clause  — safe because topicIds are ints from our DB
    QStringList idStrs;
    for (int id : topicIds) idStrs << QString::number(id);
    const QString inClause = idStrs.join(',');

    QSqlQuery q(db());
    q.prepare(QString(
        "SELECT id FROM questions "
        "WHERE topic_id IN (%1) AND difficulty <= :maxd AND is_active = 1 "
        "ORDER BY RANDOM() LIMIT :n"
    ).arg(inClause));
    q.bindValue(":maxd", maxDifficulty);
    q.bindValue(":n",    count);

    QList<QuestionDTO> list;
    if (!q.exec()) {
        qWarning() << "[QuizRepository] randomQuestions failed:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        list << loadQuestionWithOptions(q.value(0).toInt());
    }
    return list;
}

QuestionDTO QuizRepository::questionById(int id) const
{
    return loadQuestionWithOptions(id);
}

// ─────────────────────────────────────────────────────────────────────────────
// Sessions
// ─────────────────────────────────────────────────────────────────────────────
int QuizRepository::createSession(int userId, int quizId, const QString& mode) const
{
    QSqlQuery q(db());
    q.prepare("INSERT INTO quiz_sessions (user_id, quiz_id, mode, started_at) "
              "VALUES (:uid, :qid, :mode, :now)");
    q.bindValue(":uid",  userId);
    q.bindValue(":qid",  quizId > 0 ? QVariant(quizId) : QVariant());
    q.bindValue(":mode", mode);
    q.bindValue(":now",  QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    if (!q.exec()) {
        qWarning() << "[QuizRepository] createSession failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

bool QuizRepository::completeSession(int sessionId, int score,
                                      int maxScore, int timeSpentSec) const
{
    QSqlQuery q(db());
    q.prepare("UPDATE quiz_sessions SET "
              "  is_complete = 1, score = :s, max_score = :ms, "
              "  time_spent = :t, finished_at = :now "
              "WHERE id = :id");
    q.bindValue(":s",   score);
    q.bindValue(":ms",  maxScore);
    q.bindValue(":t",   timeSpentSec);
    q.bindValue(":now", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    q.bindValue(":id",  sessionId);
    return q.exec();
}

bool QuizRepository::recordAttempt(int sessionId, int questionId,
                                    const QString& userAnswer,
                                    bool isCorrect, int timeSpentSec,
                                    bool hintUsed) const
{
    QSqlQuery q(db());
    q.prepare("INSERT INTO question_attempts "
              "(session_id, question_id, user_answer, is_correct, time_spent, "
              " hint_used, answered_at) "
              "VALUES (:sid, :qid, :ans, :ok, :t, :hint, :now)");
    q.bindValue(":sid",  sessionId);
    q.bindValue(":qid",  questionId);
    q.bindValue(":ans",  userAnswer);
    q.bindValue(":ok",   isCorrect ? 1 : 0);
    q.bindValue(":t",    timeSpentSec);
    q.bindValue(":hint", hintUsed ? 1 : 0);
    q.bindValue(":now",  QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    return q.exec();
}

// ─────────────────────────────────────────────────────────────────────────────
// User Stats
// ─────────────────────────────────────────────────────────────────────────────
QList<UserTopicStatDTO> QuizRepository::userTopicStats(int userId) const
{
    QList<UserTopicStatDTO> list;
    QSqlQuery q(db());
    q.prepare(
        "SELECT uts.*, t.slug AS topic_slug, t.title AS topic_title "
        "FROM user_topic_stats uts "
        "JOIN topics t ON t.id = uts.topic_id "
        "WHERE uts.user_id = :uid "
        "ORDER BY t.order_index"
    );
    q.bindValue(":uid", userId);
    if (!q.exec()) return list;
    while (q.next()) {
        UserTopicStatDTO s;
        s.userId       = q.value("user_id").toInt();
        s.topicId      = q.value("topic_id").toInt();
        s.topicSlug    = q.value("topic_slug").toString();
        s.topicTitle   = q.value("topic_title").toString();
        s.attempts     = q.value("attempts").toInt();
        s.correct      = q.value("correct").toInt();
        s.masteryLevel = q.value("mastery_level").toDouble();
        s.lastAttemptAt= q.value("last_attempt_at").toString();
        list << s;
    }
    return list;
}

UserTopicStatDTO QuizRepository::userTopicStat(int userId, int topicId) const
{
    QSqlQuery q(db());
    q.prepare("SELECT uts.*, t.slug, t.title "
              "FROM user_topic_stats uts "
              "JOIN topics t ON t.id = uts.topic_id "
              "WHERE uts.user_id = :uid AND uts.topic_id = :tid");
    q.bindValue(":uid", userId);
    q.bindValue(":tid", topicId);
    if (q.exec() && q.next()) {
        UserTopicStatDTO s;
        s.userId       = q.value("user_id").toInt();
        s.topicId      = q.value("topic_id").toInt();
        s.topicSlug    = q.value("slug").toString();
        s.topicTitle   = q.value("title").toString();
        s.attempts     = q.value("attempts").toInt();
        s.correct      = q.value("correct").toInt();
        s.masteryLevel = q.value("mastery_level").toDouble();
        s.lastAttemptAt= q.value("last_attempt_at").toString();
        return s;
    }
    return UserTopicStatDTO{};
}

bool QuizRepository::updateTopicStats(int userId, int topicId,
                                       int deltaAttempts, int deltaCorrect) const
{
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    // Upsert pattern for both Qt5 and Qt6 (SQLite supports INSERT OR REPLACE)
    QSqlQuery existing(db());
    existing.prepare("SELECT attempts, correct FROM user_topic_stats "
                     "WHERE user_id = :uid AND topic_id = :tid");
    existing.bindValue(":uid", userId);
    existing.bindValue(":tid", topicId);

    if (existing.exec() && existing.next()) {
        const int newAttempts = existing.value(0).toInt() + deltaAttempts;
        const int newCorrect  = existing.value(1).toInt() + deltaCorrect;
        const double mastery  = newAttempts > 0
                                ? static_cast<double>(newCorrect) / newAttempts
                                : 0.0;
        QSqlQuery upd(db());
        upd.prepare("UPDATE user_topic_stats SET "
                    "  attempts = :a, correct = :c, mastery_level = :m, "
                    "  last_attempt_at = :now "
                    "WHERE user_id = :uid AND topic_id = :tid");
        upd.bindValue(":a",   newAttempts);
        upd.bindValue(":c",   newCorrect);
        upd.bindValue(":m",   mastery);
        upd.bindValue(":now", now);
        upd.bindValue(":uid", userId);
        upd.bindValue(":tid", topicId);
        return upd.exec();
    } else {
        const double mastery = deltaAttempts > 0
                               ? static_cast<double>(deltaCorrect) / deltaAttempts
                               : 0.0;
        QSqlQuery ins(db());
        ins.prepare("INSERT INTO user_topic_stats "
                    "(user_id, topic_id, attempts, correct, mastery_level, last_attempt_at) "
                    "VALUES (:uid, :tid, :a, :c, :m, :now)");
        ins.bindValue(":uid", userId);
        ins.bindValue(":tid", topicId);
        ins.bindValue(":a",   deltaAttempts);
        ins.bindValue(":c",   deltaCorrect);
        ins.bindValue(":m",   mastery);
        ins.bindValue(":now", now);
        return ins.exec();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Recommendations
// ─────────────────────────────────────────────────────────────────────────────
QList<RecommendationDTO> QuizRepository::recommendationsForUser(int userId) const
{
    QList<RecommendationDTO> list;
    QSqlQuery q(db());
    q.prepare("SELECT r.*, t.title AS topic_title FROM recommendations r "
              "LEFT JOIN topics t ON t.id = r.topic_id "
              "WHERE r.user_id = :uid AND r.is_read = 0 "
              "ORDER BY r.created_at DESC LIMIT 10");
    q.bindValue(":uid", userId);
    if (!q.exec()) return list;
    while (q.next()) {
        RecommendationDTO r;
        r.id         = q.value("id").toInt();
        r.topicId    = q.value("topic_id").isNull() ? -1 : q.value("topic_id").toInt();
        r.topicTitle = q.value("topic_title").toString();
        r.reason     = q.value("reason").toString();
        r.refUrl     = q.value("ref_url").toString();
        r.isRead     = q.value("is_read").toInt() == 1;
        r.createdAt  = q.value("created_at").toString();
        list << r;
    }
    return list;
}

bool QuizRepository::insertRecommendation(int userId, int topicId,
                                           const QString& reason,
                                           const QString& refUrl) const
{
    QSqlQuery q(db());
    q.prepare("INSERT INTO recommendations (user_id, topic_id, reason, ref_url, created_at) "
              "VALUES (:uid, :tid, :reason, :url, :now)");
    q.bindValue(":uid",    userId);
    q.bindValue(":tid",    topicId > 0 ? QVariant(topicId) : QVariant());
    q.bindValue(":reason", reason);
    q.bindValue(":url",    refUrl);
    q.bindValue(":now",    QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    return q.exec();
}

bool QuizRepository::markRecommendationRead(int recommendationId) const
{
    QSqlQuery q(db());
    q.prepare("UPDATE recommendations SET is_read = 1 WHERE id = :id");
    q.bindValue(":id", recommendationId);
    return q.exec();
}

bool QuizRepository::clearRecommendationsForUser(int userId) const
{
    QSqlQuery q(db());
    q.prepare("DELETE FROM recommendations WHERE user_id = :uid");
    q.bindValue(":uid", userId);
    return q.exec();
}

// ─────────────────────────────────────────────────────────────────────────────
// Custom Tests
// ─────────────────────────────────────────────────────────────────────────────
int QuizRepository::createCustomTest(int userId, const QString& title,
                                      const QString& description) const
{
    QSqlQuery q(db());
    q.prepare("INSERT INTO custom_tests (user_id, title, description, created_at) "
              "VALUES (:uid, :title, :desc, :now)");
    q.bindValue(":uid",   userId);
    q.bindValue(":title", title);
    q.bindValue(":desc",  description);
    q.bindValue(":now",   QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    if (!q.exec()) return -1;
    return q.lastInsertId().toInt();
}

bool QuizRepository::addQuestionToCustomTest(int testId, int questionId,
                                              int orderIndex) const
{
    QSqlQuery q(db());
    q.prepare("INSERT OR IGNORE INTO custom_test_questions "
              "(test_id, question_id, order_index) VALUES (:tid, :qid, :ord)");
    q.bindValue(":tid", testId);
    q.bindValue(":qid", questionId);
    q.bindValue(":ord", orderIndex);
    return q.exec();
}

bool QuizRepository::removeCustomTest(int testId) const
{
    QSqlQuery q(db());
    q.prepare("DELETE FROM custom_tests WHERE id = :id");
    q.bindValue(":id", testId);
    return q.exec();
}
QList<QuizDTO> QuizRepository::customTestsForUser(int userId) const
{
    QList<QuizDTO> list;
    QSqlQuery q(db());
    q.prepare(
        "SELECT ct.id, ct.title, ct.description, ct.created_at, "
        "       COUNT(ctq.question_id) AS qcount "
        "FROM custom_tests ct "
        "LEFT JOIN custom_test_questions ctq ON ctq.test_id = ct.id "
        "WHERE ct.user_id = :uid "
        "GROUP BY ct.id "
        "ORDER BY ct.created_at DESC"
    );
    q.bindValue(":uid", userId);
    if (!q.exec()) {
        qWarning() << "[QuizRepository] customTestsForUser failed:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        QuizDTO qz;
        qz.id            = q.value("id").toInt();
        qz.title         = q.value("title").toString();
        qz.description   = q.value("description").toString();
        qz.type          = "custom";
        qz.isActive      = true;
        qz.questionCount = q.value("qcount").toInt();
        qz.tags          = loadTagsForQuiz(qz.id);
        list << qz;
    }
    return list;
}

QList<QuestionDTO> QuizRepository::questionsForCustomTest(int testId) const
{
    QList<QuestionDTO> list;
    QSqlQuery q(db());
    q.prepare(
        "SELECT q.id FROM questions q "
        "JOIN custom_test_questions ctq ON ctq.question_id = q.id "
        "WHERE ctq.test_id = :tid "
        "ORDER BY ctq.order_index"
    );
    q.bindValue(":tid", testId);
    if (!q.exec()) {
        qWarning() << "[QuizRepository] questionsForCustomTest failed:" << q.lastError().text();
        return list;
    }
    while (q.next()) {
        list << loadQuestionWithOptions(q.value(0).toInt());
    }
    return list;
}
