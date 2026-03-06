#ifndef QUIZENGINE_H
#define QUIZENGINE_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QElapsedTimer>
#include "quiz/QuizRepository.h"

/**
 * @brief Represents a single question attempt within the active session.
 */
struct AttemptRecord {
    int     questionId   = -1;
    QString userAnswer;          // Plain string or JSON array for multi-select
    bool    isCorrect    = false;
    int     timeSpentSec = 0;
    bool    hintUsed     = false;
    int     pointsEarned = 0;
};

/**
 * @brief Summary emitted when a session completes.
 */
struct SessionResult {
    int     sessionId    = -1;
    int     score        = 0;
    int     maxScore     = 0;
    int     totalTimeSec = 0;
    int     totalQuestions = 0;
    int     correctCount   = 0;
    double  percentage     = 0.0;
    QList<AttemptRecord> attempts;
};

/**
 * @brief Stateful quiz session orchestrator.
 *
 * Lifecycle:
 *   1. Call startSession(quizId, userId, mode) — loads questions, creates DB row
 *   2. Call currentQuestion() to get the active QuestionDTO
 *   3. Call submitAnswer(answer) — scores, records attempt, advances
 *   4. Connect to questionChanged(index, total) for UI updates
 *   5. When isFinished() → sessionCompleted(result) is emitted
 *   6. Call abandonSession() to cleanly end without completing
 *
 * Timer:
 *   If the question has a time_limit > 0, a per-question countdown fires
 *   questionTimedOut() when it expires. The engine auto-submits a blank answer.
 *
 * Scoring:
 *   score = sum of question.points for correct answers
 *   Partial credit is NOT applied (binary correct/incorrect per question)
 *   For multi_select: all correct options must be selected, none incorrect.
 */
class QuizEngine : public QObject
{
    Q_OBJECT

public:
    explicit QuizEngine(QObject* parent = nullptr);
    ~QuizEngine() override = default;

    // ── Session lifecycle ────────────────────────────────────────────────────

    /**
     * @brief Start a new quiz session.
     * @param quizId    Quiz to load (use -1 for custom/random question list)
     * @param userId    Logged-in user's id
     * @param mode      "practice" | "exam" | "challenge"
     * @param shuffle   Shuffle questions and options
     * @return true on success
     */
    bool startSession(int quizId, int userId,
                      const QString& mode = "practice",
                      bool shuffle = false);

    /**
     * @brief Start a session with a pre-built question list (custom/random tests).
     */
    bool startCustomSession(const QList<QuestionDTO>& questions,
                            int userId,
                            const QString& mode = "practice");

    /**
     * @brief Submit an answer for the current question and advance.
     * @param answer Plain text for mcq/true_false, JSON array string for multi_select
     */
    void submitAnswer(const QString& answer);

    /**
     * @brief Skip the current question (counts as incorrect, no hint used).
     */
    void skipQuestion();

    /**
     * @brief Request a hint for the current question. Flags hint_used = true.
     * @return The hint text, or empty string if none available.
     */
    QString requestHint();

    /**
     * @brief Abandon the session (records as incomplete, does not update stats).
     */
    void abandonSession();

    // ── State queries ────────────────────────────────────────────────────────
    bool          isActive()   const;
    bool          isFinished() const;
    int           currentIndex() const;   ///< 0-based
    int           totalQuestions() const;
    QuestionDTO   currentQuestion() const;
    int           currentScore() const;
    int           secondsRemainingForQuestion() const;

signals:
    /** Emitted after each answer submission, before advancing. Index is 0-based. */
    void questionChanged(int newIndex, int total);

    /** Emitted when the per-question timer ticks (seconds remaining). */
    void questionTimerTick(int secondsRemaining);

    /** Emitted when the per-question timer expires (engine auto-submits). */
    void questionTimedOut();

    /** Emitted when the session is fully completed. */
    void sessionCompleted(const SessionResult& result);

    /** Emitted when session is abandoned. */
    void sessionAbandoned();

private slots:
    void onTimerTick();

private:
    void advanceToNext();
    void finalizeSession();
    bool evaluateAnswer(const QuestionDTO& q, const QString& answer) const;
    void startQuestionTimer(int limitSec);
    void stopQuestionTimer();

    QuizRepository          m_repo;
    QList<QuestionDTO>      m_questions;
    QList<AttemptRecord>    m_attempts;

    int     m_sessionId    = -1;
    int     m_userId       = -1;
    int     m_currentIndex = 0;
    int     m_score        = 0;
    int     m_totalTimeSec = 0;
    bool    m_active       = false;
    bool    m_finished     = false;
    QString m_mode;

    QTimer* m_questionTimer    = nullptr;
    int     m_questionTimeSec  = 0;   ///< countdown value
    QElapsedTimer  m_elapsed;
};

#endif // QUIZENGINE_H
