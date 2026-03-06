#ifndef QUIZREPOSITORY_H
#define QUIZREPOSITORY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

// ─────────────────────────────────────────────────────────────────────────────
// Data transfer objects (plain structs, no QObject overhead)
// ─────────────────────────────────────────────────────────────────────────────

struct TopicDTO {
    int     id         = -1;
    QString slug;
    QString title;
    QString description;
    int     parentId   = -1;
    int     level      = 0;       // 0=chapter, 1=section, 2=lesson
    int     difficulty = 1;       // 1=Beginner … 4=Expert
    int     orderIndex = 0;
    QString icon       = "📖";
    QString refUrl;
    QString refUrl2;
};

struct TagDTO {
    int     id    = -1;
    QString name;
    QString color = "#569CD6";
};

struct OptionDTO {
    int     id          = -1;
    int     questionId  = -1;
    QString content;
    QString codeSnippet;
    bool    isCorrect   = false;
    int     orderIndex  = 0;
};

struct QuestionDTO {
    int            id          = -1;
    int            quizId      = -1;
    int            topicId     = -1;
    QString        type;          // 'mcq' | 'multi_select' | 'true_false' | 'code_output' | 'fill_blank'
    QString        content;
    QString        codeSnippet;
    QString        explanation;
    int            difficulty   = 1;
    int            timeLimitSec = 0;
    int            points       = 10;
    int            orderIndex   = 0;
    QString        hint;
    QString        refUrl;
    QList<OptionDTO> options;
    QStringList    tags;
};

struct QuizDTO {
    int         id          = -1;
    QString     title;
    QString     description;
    int         topicId     = -1;
    int         difficulty  = 1;
    int         timeLimitSec= 0;
    bool        isTimed     = false;
    QString     type        = "standard"; // 'standard'|'exam'|'challenge'|'interview'
    bool        isActive    = true;
    QStringList tags;
    int         questionCount = 0;
};

struct UserTopicStatDTO {
    int     userId        = -1;
    int     topicId       = -1;
    QString topicSlug;
    QString topicTitle;
    int     attempts      = 0;
    int     correct       = 0;
    double  masteryLevel  = 0.0;   // 0.0–1.0
    QString lastAttemptAt;
};

struct RecommendationDTO {
    int     id       = -1;
    int     topicId  = -1;
    QString topicTitle;
    QString reason;
    QString refUrl;
    bool    isRead   = false;
    QString createdAt;
};

// ─────────────────────────────────────────────────────────────────────────────
// QuizRepository — single point of all SQLite access for quiz content
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Stateless data access layer for CppAtlas quiz content.
 *
 * All methods issue QSqlQuery against QuizDatabase::CONNECTION_NAME.
 * No UI coupling. No state held between calls.
 * QuizEngine and UI layers call this exclusively — never QSqlQuery directly.
 */
class QuizRepository
{
public:
    QuizRepository() = default;

    // ── Topics ───────────────────────────────────────────────────────────────
    QList<TopicDTO>    allTopics() const;
    QList<TopicDTO>    topicsByDifficulty(int difficulty) const;
    QList<TopicDTO>    childTopics(int parentId) const;
    QList<TopicDTO>    rootTopics() const;     ///< topics with parent_id IS NULL
    TopicDTO           topicById(int id) const;
    TopicDTO           topicBySlug(const QString& slug) const;

    // ── Tags ─────────────────────────────────────────────────────────────────
    QList<TagDTO>      allTags() const;
    QList<TagDTO>      tagsForQuestion(int questionId) const;
    QList<TagDTO>      tagsForQuiz(int quizId) const;

    // ── Quizzes ──────────────────────────────────────────────────────────────
    QList<QuizDTO>     allActiveQuizzes() const;
    QList<QuizDTO>     quizzesByTopic(int topicId) const;
    QList<QuizDTO>     quizzesByDifficulty(int difficulty) const;
    QList<QuizDTO>     quizzesByTag(const QString& tagName) const;
    QuizDTO            quizById(int id) const;

    // ── Questions ────────────────────────────────────────────────────────────
    /** All questions for a quiz, with options and tags loaded. */
    QList<QuestionDTO> questionsForQuiz(int quizId) const;

    /** Questions for a quiz, shuffled (for exam/challenge mode). */
    QList<QuestionDTO> questionsForQuizShuffled(int quizId) const;

    /** Random questions from selected topics, for custom test generation. */
    QList<QuestionDTO> randomQuestions(const QList<int>& topicIds,
                                       int count,
                                       int maxDifficulty = 4) const;

    QuestionDTO        questionById(int id) const;

    // ── Sessions (write) ─────────────────────────────────────────────────────
    /** Create a new quiz session row. Returns the new session id, or -1. */
    int  createSession(int userId, int quizId, const QString& mode) const;

    /** Mark session as complete with final score. */
    bool completeSession(int sessionId, int score, int maxScore, int timeSpentSec) const;

    /** Record a single question attempt. */
    bool recordAttempt(int sessionId, int questionId,
                       const QString& userAnswer,
                       bool isCorrect,
                       int timeSpentSec,
                       bool hintUsed) const;

    // ── User Stats (read/write) ───────────────────────────────────────────────
    QList<UserTopicStatDTO> userTopicStats(int userId) const;
    UserTopicStatDTO        userTopicStat(int userId, int topicId) const;

    /** Upsert topic stats after a session. Called by QuizEngine internally. */
    bool updateTopicStats(int userId, int topicId,
                          int deltaAttempts, int deltaCorrect) const;

    // ── Recommendations ──────────────────────────────────────────────────────
    QList<RecommendationDTO> recommendationsForUser(int userId) const;
    bool insertRecommendation(int userId, int topicId,
                               const QString& reason,
                               const QString& refUrl) const;
    bool markRecommendationRead(int recommendationId) const;
    bool clearRecommendationsForUser(int userId) const;

    // ── Custom Tests ─────────────────────────────────────────────────────────
    int  createCustomTest(int userId,
                          const QString& title,
                          const QString& description) const;
    bool addQuestionToCustomTest(int testId, int questionId, int orderIndex) const;
    bool removeCustomTest(int testId) const;

private:
    QuestionDTO      loadQuestionWithOptions(int questionId) const;
    QList<OptionDTO> loadOptions(int questionId) const;
    QStringList      loadTagsForQuestion(int questionId) const;
    QuizDTO          quizFromQuery(class QSqlQuery& q) const;
    TopicDTO         topicFromQuery(class QSqlQuery& q) const;
    QuestionDTO      questionFromQuery(class QSqlQuery& q) const;
};

#endif // QUIZREPOSITORY_H