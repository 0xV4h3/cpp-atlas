#ifndef USERPROFILEWIDGET_H
#define USERPROFILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QList>

#include "quiz/UserManager.h"
#include "quiz/QuizRepository.h"
#include "quiz/ProgressAnalyzer.h"

// Forward declaration — RadarChartWidget defined in QuizResultsWidget.h
class RadarChartWidget;

// ─────────────────────────────────────────────────────────────────────────────
// ProgressRingWidget — circular overall-score indicator (QPainter, no QtCharts)
// ─────────────────────────────────────────────────────────────────────────────
/**
 * @brief Draws a circular progress ring showing overall mastery score (0–100).
 *
 * Inspired by FIFA/PES player card circular rating.
 * Renders with QPainter — no Qt Charts dependency.
 *
 * Usage:
 *   ProgressRingWidget* ring = new ProgressRingWidget(this);
 *   ring->setScore(73);   // 0–100
 */
class ProgressRingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressRingWidget(QWidget* parent = nullptr);
    void setScore(int score);        ///< 0–100

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int m_score = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
// TopicScoreBar — single horizontal bar row (topic name + bar + percentage)
// ─────────────────────────────────────────────────────────────────────────────
/**
 * @brief One row in the topic scores panel: "Memory  ████████░░  78%"
 * Color-coded: green ≥80%, yellow ≥60%, red <60%.
 */
class TopicScoreBar : public QWidget
{
    Q_OBJECT
public:
    explicit TopicScoreBar(const QString& topic,
                           double mastery,   ///< 0.0–1.0
                           QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_topic;
    double  m_mastery;
};

// ─────────────────────────────────────────────────────────────────────────────
// UserProfileWidget — PES-style learning dashboard
// ─────────────────────────────────────────────────────────────────────────────
/**
 * @brief Full user profile / analytics dashboard widget.
 *
 * Layout (matches the ASCII mockup from the planning phase):
 * ┌─────────────────────────────────────────────────────────────────┐
 * │  [Avatar]  username   ●●●●○  Level N - <title>                  │
 * │  [C++]     "tagline"  XP bar to next level                      │
 * ├────────────┬───────────────────────┬───────────────────────────┤
 * │ OVERALL    │   [RADAR CHART]       │  TOPIC SCORES             │
 * │ [Ring 73%] │   QPainter pentagon   │  Memory    78% ████░      │
 * │            │                       │  Templates 45% ██░        │
 * │            │                       │  OOP       92% █████      │
 * ├────────────┴───────────────────────┴───────────────────────────┤
 * │  🔥 Streak: 7 days   ✅ Quizzes: 24   ⏱ Time: 3h 42m          │
 * ├─────────────────────────────────────────────────────────────────┤
 * │  RECOMMENDATIONS                                                │
 * │  ⚠ Templates (45%) → learncpp.com link                         │
 * └─────────────────────────────────────────────────────────────────┘
 *
 * Call refresh(userId) to reload all data from DB.
 */
class UserProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserProfileWidget(QWidget* parent = nullptr);
    ~UserProfileWidget() = default;

    /**
     * @brief Load and display profile data for the given user.
     * Safe to call multiple times (refreshes in-place).
     */
    void refresh(int userId);

signals:
    /** Emitted when user clicks "Start Quiz" from the recommendations panel. */
    void startQuizFromRecommendation(int topicId);

private slots:
    void applyTheme();

private:
    // ── Section builders ────────────────────────────────────────────────────
    void buildAvatarCard(const UserRecord& user, int overallScore);
    void buildAnalyticsRow(const QList<UserTopicStatDTO>& stats,
                           int overallScore);
    void buildStatsBar(int userId);
    void buildRecommendations(int userId);
    void clearDynamicContent();

    // ── XP / level helpers ──────────────────────────────────────────────────
    static int  computeLevel(int totalXp);
    static int  xpForLevel(int level);        ///< XP needed to reach that level
    static QString levelTitle(int level);
    static int  computeXpFromSessions(int userId);
    static int  computeStreak(int userId);
    static int  totalQuizzesCompleted(int userId);
    static int  totalTimeSpentSec(int userId);

    // ── Widgets (static layout — always present) ─────────────────────────────
    QScrollArea*      m_scrollArea      = nullptr;
    QWidget*          m_contentWidget   = nullptr;
    QVBoxLayout*      m_contentLayout   = nullptr;

    // Avatar card
    QWidget*          m_avatarCard      = nullptr;
    QLabel*           m_avatarLabel     = nullptr;  ///< large "C++" monogram
    QLabel*           m_usernameLabel   = nullptr;
    QLabel*           m_levelLabel      = nullptr;
    QLabel*           m_xpBarOuter      = nullptr;  ///< container for XP bar
    QWidget*          m_xpBarFill       = nullptr;  ///< colored fill (width%)

    // Analytics row (3-column)
    QWidget*          m_analyticsRow    = nullptr;
    ProgressRingWidget* m_progressRing  = nullptr;
    RadarChartWidget*   m_radarChart    = nullptr;
    QWidget*          m_topicBarsPanel  = nullptr;
    QVBoxLayout*      m_topicBarsLayout = nullptr;

    // Stats bar
    QWidget*          m_statsBar        = nullptr;
    QLabel*           m_streakLabel     = nullptr;
    QLabel*           m_quizzesLabel    = nullptr;
    QLabel*           m_timeLabel       = nullptr;

    // Recommendations
    QWidget*          m_recsSection     = nullptr;
    QVBoxLayout*      m_recsLayout      = nullptr;

    ProgressAnalyzer  m_analyzer;
    QuizRepository    m_repo;
};

#endif // USERPROFILEWIDGET_H