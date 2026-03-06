#include "quiz/QuizEngine.h"
#include "quiz/ProgressAnalyzer.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QDebug>
#include <algorithm>

QuizEngine::QuizEngine(QObject* parent)
    : QObject(parent)
    , m_questionTimer(new QTimer(this))
{
    m_questionTimer->setInterval(1000);  // 1 second tick
    connect(m_questionTimer, &QTimer::timeout, this, &QuizEngine::onTimerTick);
}

// ─────────────────────────────────────────────────────────────────────────────
// Session lifecycle
// ────────────────────────────────────────────────────��────────────────────────
bool QuizEngine::startSession(int quizId, int userId,
                               const QString& mode, bool shuffle)
{
    if (m_active) {
        qWarning() << "[QuizEngine] startSession called while session is active";
        return false;
    }

    m_questions = shuffle
                  ? m_repo.questionsForQuizShuffled(quizId)
                  : m_repo.questionsForQuiz(quizId);

    if (m_questions.isEmpty()) {
        qWarning() << "[QuizEngine] No active questions for quiz" << quizId;
        return false;
    }

    m_sessionId    = m_repo.createSession(userId, quizId, mode);
    if (m_sessionId < 0) return false;

    m_userId       = userId;
    m_mode         = mode;
    m_currentIndex = 0;
    m_score        = 0;
    m_totalTimeSec = 0;
    m_active       = true;
    m_finished     = false;
    m_attempts.clear();

    // Start timer for first question
    if (m_questions[0].timeLimitSec > 0)
        startQuestionTimer(m_questions[0].timeLimitSec);

    m_elapsed.start();
    emit questionChanged(0, m_questions.size());
    return true;
}

bool QuizEngine::startCustomSession(const QList<QuestionDTO>& questions,
                                     int userId, const QString& mode)
{
    if (m_active || questions.isEmpty()) return false;

    m_questions    = questions;
    m_sessionId    = m_repo.createSession(userId, -1, mode);
    if (m_sessionId < 0) return false;

    m_userId       = userId;
    m_mode         = mode;
    m_currentIndex = 0;
    m_score        = 0;
    m_totalTimeSec = 0;
    m_active       = true;
    m_finished     = false;
    m_attempts.clear();

    if (m_questions[0].timeLimitSec > 0)
        startQuestionTimer(m_questions[0].timeLimitSec);

    m_elapsed.start();
    emit questionChanged(0, m_questions.size());
    return true;
}

void QuizEngine::submitAnswer(const QString& answer)
{
    if (!m_active || m_finished) return;

    stopQuestionTimer();

    const QuestionDTO& q  = m_questions[m_currentIndex];
    const bool correct    = evaluateAnswer(q, answer);
    const int  timeSpent  = m_elapsed.isValid()
                            ? static_cast<int>(m_elapsed.elapsed() / 1000)
                            : 0;

    AttemptRecord rec;
    rec.questionId   = q.id;
    rec.userAnswer   = answer;
    rec.isCorrect    = correct;
    rec.timeSpentSec = timeSpent;
    rec.hintUsed     = false;   // set by requestHint() before submitAnswer
    rec.pointsEarned = correct ? q.points : 0;
    m_attempts << rec;

    if (correct) m_score += q.points;

    m_repo.recordAttempt(m_sessionId, q.id, answer, correct, timeSpent, false);

    advanceToNext();
}

void QuizEngine::skipQuestion()
{
    submitAnswer(QString());
}

QString QuizEngine::requestHint()
{
    if (!m_active || m_finished) return {};
    const QuestionDTO& q = m_questions[m_currentIndex];
    if (!m_attempts.isEmpty() && m_attempts.last().questionId == q.id) {
        m_attempts.last().hintUsed = true;
    }
    return q.hint;
}

void QuizEngine::abandonSession()
{
    if (!m_active) return;
    stopQuestionTimer();
    m_active   = false;
    m_finished = false;
    emit sessionAbandoned();
}

// ─────────────────────────────────────────────────────────────────────────────
// State queries
// ─────────────────────────────────────────────────────────────────────────────
bool QuizEngine::isActive()   const { return m_active && !m_finished; }
bool QuizEngine::isFinished() const { return m_finished; }
int  QuizEngine::currentIndex() const { return m_currentIndex; }
int  QuizEngine::totalQuestions() const { return m_questions.size(); }
int  QuizEngine::currentScore() const { return m_score; }

QuestionDTO QuizEngine::currentQuestion() const
{
    if (m_currentIndex < m_questions.size())
        return m_questions[m_currentIndex];
    return QuestionDTO{};
}

int QuizEngine::secondsRemainingForQuestion() const
{
    return m_questionTimeSec;
}

// ─────────────────────────────────────────────────────────────────────────────
// Private
// ─────────────────────────────────────────────────────────────────────────────
void QuizEngine::advanceToNext()
{
    m_totalTimeSec += m_elapsed.isValid()
                      ? static_cast<int>(m_elapsed.elapsed() / 1000) : 0;

    ++m_currentIndex;
    if (m_currentIndex >= m_questions.size()) {
        finalizeSession();
        return;
    }

    // Start timer for next question if timed
    const int nextLimit = m_questions[m_currentIndex].timeLimitSec;
    if (nextLimit > 0) startQuestionTimer(nextLimit);

    m_elapsed.restart();
    emit questionChanged(m_currentIndex, m_questions.size());
}

void QuizEngine::finalizeSession()
{
    m_active   = false;
    m_finished = true;
    stopQuestionTimer();

    const int maxScore = [&]() {
        int s = 0;
        for (const auto& q : m_questions) s += q.points;
        return s;
    }();

    m_repo.completeSession(m_sessionId, m_score, maxScore, m_totalTimeSec);

    // Update per-topic stats
    QHash<int, QPair<int,int>> topicDelta;  // topicId -> (attempts, correct)
    for (const auto& attempt : m_attempts) {
        for (const auto& q : m_questions) {
            if (q.id == attempt.questionId && q.topicId > 0) {
                auto& d = topicDelta[q.topicId];
                d.first  += 1;
                d.second += attempt.isCorrect ? 1 : 0;
                break;
            }
        }
    }
    for (auto it = topicDelta.cbegin(); it != topicDelta.cend(); ++it) {
        m_repo.updateTopicStats(m_userId, it.key(),
                                it.value().first, it.value().second);
    }

    // Build result
    SessionResult result;
    result.sessionId      = m_sessionId;
    result.score          = m_score;
    result.maxScore       = maxScore;
    result.totalTimeSec   = m_totalTimeSec;
    result.totalQuestions = m_questions.size();
    result.correctCount   = static_cast<int>(
        std::count_if(m_attempts.cbegin(), m_attempts.cend(),
                      [](const AttemptRecord& a) { return a.isCorrect; }));
    result.percentage     = maxScore > 0
                            ? (100.0 * m_score / maxScore) : 0.0;
    result.attempts       = m_attempts;

    // Run adaptive recommendations (non-blocking, uses stored stats)
    ProgressAnalyzer analyzer;
    analyzer.generateRecommendations(m_userId);

    emit sessionCompleted(result);
}

bool QuizEngine::evaluateAnswer(const QuestionDTO& q, const QString& answer) const
{
    if (answer.isEmpty()) return false;

    if (q.type == "mcq" || q.type == "true_false" || q.type == "code_output") {
        // answer is the option id as string, or the text content
        // We compare by option id first, then by content
        bool ok = false;
        const int optId = answer.toInt(&ok);
        for (const auto& opt : q.options) {
            if (ok) {
                if (opt.id == optId && opt.isCorrect) return true;
            } else {
                if (opt.content.trimmed().compare(answer.trimmed(),
                    Qt::CaseInsensitive) == 0 && opt.isCorrect) return true;
            }
        }
        return false;
    }

    if (q.type == "multi_select") {
        // answer is a JSON array of option ids: "[1,3,5]"
        QJsonDocument doc = QJsonDocument::fromJson(answer.toUtf8());
        if (!doc.isArray()) return false;
        QJsonArray arr = doc.array();
        QSet<int> selected;
        for (const auto& v : arr) selected.insert(v.toInt());

        QSet<int> correct;
        for (const auto& opt : q.options)
            if (opt.isCorrect) correct.insert(opt.id);

        return selected == correct;
    }

    if (q.type == "fill_blank") {
        // Compare against first correct option text, case-insensitive
        for (const auto& opt : q.options) {
            if (opt.isCorrect && opt.content.trimmed().compare(
                answer.trimmed(), Qt::CaseInsensitive) == 0) return true;
        }
        return false;
    }

    return false;
}

void QuizEngine::startQuestionTimer(int limitSec)
{
    m_questionTimeSec = limitSec;
    m_questionTimer->start();
}

void QuizEngine::stopQuestionTimer()
{
    m_questionTimer->stop();
}

void QuizEngine::onTimerTick()
{
    --m_questionTimeSec;
    emit questionTimerTick(m_questionTimeSec);
    if (m_questionTimeSec <= 0) {
        stopQuestionTimer();
        emit questionTimedOut();
        // Auto-submit blank
        submitAnswer(QString());
    }
}
