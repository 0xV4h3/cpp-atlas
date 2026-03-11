#ifndef QUIZSESSIONWIDGET_H
#define QUIZSESSIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QButtonGroup>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QScrollArea>

#include "quiz/QuizEngine.h"
#include "quiz/QuizRepository.h"

/**
 * @brief Full interactive quiz player widget.
 *
 * Displays one question at a time. Supports question types:
 *   - mcq          : radio-button option cards
 *   - true_false   : two large True / False buttons
 *   - code_output  : code snippet block + radio options
 *   - multi_select : checkbox option cards  (submit button required)
 *   - fill_blank   : QLineEdit answer field (submit button required)
 *
 * Features:
 *   - Progress bar  : question index / total
 *   - Per-question countdown timer (shown only when timeLimitSec > 0)
 *   - Hint button   : reveals hint text, marks attempt as hint_used
 *   - Skip button   : submits blank (counts as incorrect)
 *   - Keyboard:     1–4 select MCQ options; Enter submits; H shows hint
 *
 * Signals:
 *   sessionCompleted(SessionResult)  → QuizModeWindow shows ResultsWidget
 *   sessionAbandoned()               → QuizModeWindow shows SelectionWidget
 */
class QuizSessionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuizSessionWidget(QWidget* parent = nullptr);
    ~QuizSessionWidget() = default;

    /**
     * @brief Start a new quiz session. Called by QuizModeWindow.
     * @param quizId   Quiz ID from QuizRepository
     * @param userId   Logged-in user's id
     * @param mode     "practice" | "exam" | "challenge"
     * @param shuffle  Shuffle questions and options
     */
    void startQuiz(int quizId, int userId,
                   const QString& mode = "practice",
                   bool shuffle = true);

    /** Start a session with a pre-built custom question list. */
    void startCustomSession(const QList<QuestionDTO>& questions,
                            int userId,
                            const QString& mode = "practice");

    /**
     * @brief Returns the question list from the most recently completed session.
     * Used by QuizModeWindow to pass questions to QuizResultsWidget for review.
     */
    QList<QuestionDTO> lastQuestions() const { return m_engine ? m_engine->lastQuestions() : QList<QuestionDTO>{}; }

signals:
    void sessionCompleted(const SessionResult& result);
    void sessionAbandoned();

private slots:
    void onQuestionChanged(int index, int total);
    void onTimerTick(int secondsRemaining);
    void onQuestionTimedOut();
    void onOptionSelected(int optionId);
    void onSubmitClicked();
    void onSkipClicked();
    void onHintClicked();
    void onAbandonClicked();
    void applyTheme();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    void setupUi();
    void renderQuestion(const QuestionDTO& q);
    void renderMCQ(const QuestionDTO& q);
    void renderTrueFalse(const QuestionDTO& q);
    void renderCodeOutput(const QuestionDTO& q);
    void renderMultiSelect(const QuestionDTO& q);
    void renderFillBlank(const QuestionDTO& q);
    void clearOptionArea();
    void showFeedback(bool correct, const QString& explanation);
    void updateProgressBar(int index, int total);
    void updateTimerDisplay(int seconds);
    QString formatTime(int seconds) const;

    // ── Engine ────────────────────────────────────────────────────────────
    QuizEngine*   m_engine       = nullptr;

    // ── Header bar ────────────────────────────────────────────────────────
    QLabel*       m_progressLabel= nullptr;
    QProgressBar* m_progressBar  = nullptr;
    QLabel*       m_timerLabel   = nullptr;
    QLabel*       m_scoreLabel   = nullptr;
    QPushButton*  m_abandonBtn   = nullptr;

    // ── Question area ─────────────────────────────────────────────────────
    QLabel*       m_questionNumLabel  = nullptr;
    QLabel*       m_difficultyLabel   = nullptr;
    QLabel*       m_questionText      = nullptr;
    QLabel*       m_codeSnippetLabel  = nullptr;  ///< monospace code block

    // ── Option area (dynamic, cleared per question) ───────────────────────
    QWidget*      m_optionArea        = nullptr;
    QVBoxLayout*  m_optionLayout      = nullptr;
    QScrollArea*  m_optionScroll      = nullptr;
    QButtonGroup* m_radioGroup        = nullptr;   ///< for MCQ/true_false
    QList<QCheckBox*> m_checkBoxes;                ///< for multi_select
    class QLineEdit*  m_fillBlankEdit = nullptr;   ///< for fill_blank

    // ── Footer controls ───────────────────────────────────────────────────
    QLabel*       m_hintLabel    = nullptr;
    QPushButton*  m_hintBtn      = nullptr;
    QPushButton*  m_skipBtn      = nullptr;
    QPushButton*  m_submitBtn    = nullptr;

    // ── Feedback overlay ──────────────────────────────────────────────────
    QLabel*       m_feedbackLabel     = nullptr;

    // ── State ─────────────────────────────────────────────────────────────
    int           m_selectedOptionId  = -1;
    bool          m_awaitingNext      = false;  ///< true after feedback shown
    QString       m_currentMode;
};

#endif // QUIZSESSIONWIDGET_H
