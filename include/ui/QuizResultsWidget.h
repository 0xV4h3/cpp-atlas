#ifndef QUIZRESULTSWIDGET_H
#define QUIZRESULTSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "quiz/QuizEngine.h"        // SessionResult, AttemptRecord
#include "quiz/QuizRepository.h"    // QuestionDTO, UserTopicStatDTO
#include "quiz/ProgressAnalyzer.h"

/**
 * @brief Post-quiz results and review widget.
 *
 * Layout (vertical):
 *   1. Score card  — big score number, percentage, time spent
 *   2. Radar chart — QPainter pentagon of topic mastery levels (0.0–1.0)
 *                    inspired by PES/FIFA player card attribute charts
 *   3. Weak areas  — list of topics below WEAK_THRESHOLD with learncpp.com links
 *   4. Q&A review  — scrollable list of every question:
 *                    ✅/❌ icon, question text, user answer, correct answer, explanation
 *   5. Footer      — "Try Again" and "Back to Quiz Selection" buttons
 *
 * No DB calls here — all data comes from SessionResult + UserTopicStatDTO list
 * passed in via showResults(). ProgressAnalyzer is called read-only.
 */
class QuizResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuizResultsWidget(QWidget* parent = nullptr);
    ~QuizResultsWidget() = default;

    /**
     * @brief Populate the widget with completed session data.
     * Call this before showing the widget (page 2 of QuizModeWindow stack).
     * @param result     The SessionResult from QuizEngine::sessionCompleted
     * @param questions  The ordered question list from the session (for review)
     * @param userId     For loading updated topic stats after the session
     */
    void showResults(const SessionResult& result,
                     const QList<QuestionDTO>& questions,
                     int userId);

signals:
    /** User wants to replay the same quiz. */
    void retryRequested();

    /** User wants to return to quiz selection. */
    void backToSelectionRequested();

private slots:
    void applyTheme();

private:
    void buildScoreCard(const SessionResult& result);
    void buildRadarChart(const QList<UserTopicStatDTO>& stats);
    void buildWeakAreas(const QList<UserTopicStatDTO>& weak);
    void buildReviewList(const SessionResult& result,
                         const QList<QuestionDTO>& questions);
    void clearContent();

    // ── Radar chart (pure QPainter, no Qt Charts) ─────────────────────────
    class RadarChartWidget* m_radarChart = nullptr;

    // ── Static layout widgets (always present) ────────────────────────────
    QWidget*     m_scoreCard      = nullptr;
    QLabel*      m_scoreBigLabel  = nullptr;
    QLabel*      m_scoreSubLabel  = nullptr;
    QLabel*      m_timeLabel      = nullptr;
    QLabel*      m_statsLabel     = nullptr;

    QWidget*     m_weakSection    = nullptr;
    QVBoxLayout* m_weakLayout     = nullptr;

    QWidget*     m_reviewSection  = nullptr;
    QVBoxLayout* m_reviewLayout   = nullptr;

    QPushButton* m_retryBtn       = nullptr;
    QPushButton* m_backBtn        = nullptr;

    // ── Dynamic content container ─────────────────────────────────────────
    QScrollArea* m_scrollArea     = nullptr;
    QWidget*     m_contentWidget  = nullptr;
    QVBoxLayout* m_contentLayout  = nullptr;

    ProgressAnalyzer m_analyzer;
    QuizRepository   m_repo;
};

// ─────────────────────────────────────────────────────────────────────────────
// RadarChartWidget — standalone QPainter radar/spider chart
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Custom QPainter radar chart for topic mastery visualization.
 *
 * Draws a regular N-polygon (one axis per topic) with a filled polygon
 * representing the user's mastery level on each axis (0.0–1.0).
 * Inspired by PES Football player card attribute charts.
 *
 * ⚠️ Requires Qt Charts: NO. Pure QPainter. Zero extra dependency.
 */
class RadarChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RadarChartWidget(QWidget* parent = nullptr);

    /**
     * @brief Set the data points. Each pair: (label, value 0.0–1.0).
     * Call update() afterwards to repaint.
     */
    void setData(const QList<QPair<QString, double>>& data);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QList<QPair<QString, double>> m_data;
};

#endif // QUIZRESULTSWIDGET_H