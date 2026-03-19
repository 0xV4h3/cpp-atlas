#include "ui/UserProfileWidget.h"
#include "ui/QuizResultsWidget.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QDesktopServices>
#include <QUrl>
#include <QtMath>
#include <QScrollBar>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDate>
#include "quiz/QuizDatabase.h"

// ── Avatar helper ─────────────────────────────────────────────────────────────
static QPixmap makeProfileAvatar(const UserRecord& u, int size)
{
    QPixmap circle(size, size);
    circle.fill(Qt::transparent);
    QPainter p(&circle);
    p.setRenderHint(QPainter::Antialiasing);

    if (!u.avatarPath.isEmpty()) {
        QPixmap pix(u.avatarPath);
        if (!pix.isNull()) {
            QPainterPath path;
            path.addEllipse(0, 0, size, size);
            p.setClipPath(path);
            p.drawPixmap(0, 0, size, size,
                         pix.scaled(size, size, Qt::KeepAspectRatioByExpanding,
                                    Qt::SmoothTransformation));
            return circle;
        }
    }

    const QColor color(u.avatarColor.isEmpty() ? "#007ACC" : u.avatarColor);
    p.setBrush(color);
    p.setPen(Qt::NoPen);
    p.drawEllipse(0, 0, size, size);
    p.setPen(Qt::white);
    QFont f = p.font();
    f.setPointSize(qMax(8, size / 3));
    f.setBold(true);
    p.setFont(f);
    const QString initial = u.displayName.isEmpty()
                                ? u.username.left(1).toUpper()
                                : u.displayName.left(1).toUpper();
    p.drawText(QRect(0, 0, size, size), Qt::AlignCenter, initial);
    return circle;
}

// ─────────────────────────────────────────────────────────────────────────────
// ProgressRingWidget
// ─────────────────────────────────────────────────────────────────────────────
ProgressRingWidget::ProgressRingWidget(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(120, 120);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(130, 130);
}

void ProgressRingWidget::setScore(int score) { m_score = qBound(0, score, 100); update(); }

void ProgressRingWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const Theme& t    = ThemeManager::instance()->currentTheme();
    const QRectF rect = QRectF(10, 10, width() - 20, height() - 20);
    const int    penW = 10;

    p.setPen(QPen(QColor(t.border.name()), penW, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(rect.adjusted(penW/2, penW/2, -penW/2, -penW/2));

    const int spanAngle = static_cast<int>(-360.0 * m_score / 100.0 * 16);
    const QColor arcColor = m_score >= 80 ? QColor(t.success.name())
                            : m_score >= 50 ? QColor(t.warning.name())
                                            :                 QColor(t.error.name());
    p.setPen(QPen(arcColor, penW, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(rect.adjusted(penW/2, penW/2, -penW/2, -penW/2), 90 * 16, spanAngle);

    p.setPen(QPen(QColor(t.textPrimary.name())));
    QFont scoreFont = p.font();
    scoreFont.setPointSize(22);
    scoreFont.setBold(true);
    p.setFont(scoreFont);
    p.drawText(rect, Qt::AlignCenter, QString::number(m_score));
}

// ─────────────────────────────────────────────────────────────────────────────
// TopicScoreBar
// ─────────────────────────────────────────────────────────────────────────────
TopicScoreBar::TopicScoreBar(const QString& topic, double mastery, QWidget* parent)
    : QWidget(parent), m_topic(topic), m_mastery(qBound(0.0, mastery, 1.0))
{
    setFixedHeight(26);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void TopicScoreBar::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const Theme& t    = ThemeManager::instance()->currentTheme();
    const int labelW  = 130;
    const int percentW = 38;
    const int barH    = 10;
    const int barY    = (height() - barH) / 2;

    p.setPen(QColor(t.textPrimary.name()));
    QFont f = p.font(); f.setPointSize(10); p.setFont(f);
    QFontMetrics fm(p.font());
    const QString label = fm.elidedText(m_topic, Qt::ElideRight, labelW - 4);
    p.drawText(QRect(0, 0, labelW, height()), Qt::AlignVCenter | Qt::AlignLeft, label);

    const int barX = labelW + 4;
    const int barW = width() - labelW - percentW - 8;
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(t.border.name()));
    p.drawRoundedRect(barX, barY, barW, barH, 5, 5);

    const QColor fillColor = m_mastery >= 0.80 ? QColor(t.success.name())
                             : m_mastery >= 0.60 ? QColor(t.warning.name())
                                                 :                     QColor(t.error.name());
    const int fillW = static_cast<int>(barW * m_mastery);
    if (fillW > 0) {
        p.setBrush(fillColor);
        p.drawRoundedRect(barX, barY, fillW, barH, 5, 5);
    }

    p.setPen(QColor(t.textSecondary.name()));
    QFont pf = p.font(); pf.setPointSize(9); p.setFont(pf);
    p.drawText(QRect(barX + barW + 4, 0, percentW, height()),
               Qt::AlignVCenter | Qt::AlignRight,
               QString("%1%").arg(static_cast<int>(m_mastery * 100)));
}

// ─────────────────────────────────────────────────────────────────────────────
// UserProfileWidget — constructor
// ─────────────────────────────────────────────────────────────────────────────
UserProfileWidget::UserProfileWidget(QWidget* parent)
    : QWidget(parent)
    , m_radarChart(new RadarChartWidget(this))
    , m_progressRing(new ProgressRingWidget(this))
{
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName("profileScroll");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_contentWidget = new QWidget(m_scrollArea);
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(32, 24, 32, 24);
    m_contentLayout->setSpacing(16);
    m_scrollArea->setWidget(m_contentWidget);
    outerLayout->addWidget(m_scrollArea);

    // ── Avatar card ────────────────────────────────────────────────────────
    m_avatarCard = new QWidget(m_contentWidget);
    m_avatarCard->setObjectName("avatarCard");
    QHBoxLayout* avatarLayout = new QHBoxLayout(m_avatarCard);
    avatarLayout->setContentsMargins(20, 16, 20, 16);
    avatarLayout->setSpacing(20);

    // FIX: avatar label — will hold a QPixmap, not "C++" text
    m_avatarLabel = new QLabel(m_avatarCard);
    m_avatarLabel->setObjectName("avatarMonogram");
    m_avatarLabel->setFixedSize(72, 72);
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLayout->addWidget(m_avatarLabel);

    QVBoxLayout* nameBlock = new QVBoxLayout();
    nameBlock->setSpacing(4);

    m_usernameLabel = new QLabel(m_avatarCard);
    m_usernameLabel->setObjectName("usernameLabel");
    nameBlock->addWidget(m_usernameLabel);

    m_levelLabel = new QLabel(m_avatarCard);
    m_levelLabel->setObjectName("levelLabel");
    nameBlock->addWidget(m_levelLabel);

    QWidget* xpBarContainer = new QWidget(m_avatarCard);
    xpBarContainer->setObjectName("xpBarContainer");
    xpBarContainer->setFixedHeight(12);
    xpBarContainer->setMinimumWidth(280);
    QHBoxLayout* xpBarLayout = new QHBoxLayout(xpBarContainer);
    xpBarLayout->setContentsMargins(0, 0, 0, 0);
    xpBarLayout->setSpacing(0);

    m_xpBarFill = new QWidget(xpBarContainer);
    m_xpBarFill->setObjectName("xpBarFill");
    m_xpBarFill->setFixedHeight(12);
    xpBarLayout->addWidget(m_xpBarFill);
    xpBarLayout->addStretch();

    nameBlock->addWidget(xpBarContainer);
    m_xpBarOuter = new QLabel(m_avatarCard);
    m_xpBarOuter->setObjectName("xpLabel");
    nameBlock->addWidget(m_xpBarOuter);

    avatarLayout->addLayout(nameBlock, 1);
    m_contentLayout->addWidget(m_avatarCard);

    // ── Analytics row ──────────────────────────────────────────────────────
    m_analyticsRow = new QWidget(m_contentWidget);
    QHBoxLayout* analyticsLayout = new QHBoxLayout(m_analyticsRow);
    analyticsLayout->setContentsMargins(0, 0, 0, 0);
    analyticsLayout->setSpacing(16);

    QWidget* ringCol = new QWidget(m_analyticsRow);
    ringCol->setObjectName("analyticsCard");
    QVBoxLayout* ringLayout = new QVBoxLayout(ringCol);
    ringLayout->setAlignment(Qt::AlignCenter);
    QLabel* overallLabel = new QLabel("OVERALL", ringCol);
    overallLabel->setObjectName("analyticsCardHeader");
    overallLabel->setAlignment(Qt::AlignCenter);
    ringLayout->addWidget(overallLabel);
    m_progressRing->setParent(ringCol);
    ringLayout->addWidget(m_progressRing, 0, Qt::AlignCenter);
    ringCol->setFixedWidth(150);
    analyticsLayout->addWidget(ringCol, 0);

    QWidget* radarCol = new QWidget(m_analyticsRow);
    radarCol->setObjectName("analyticsCard");
    QVBoxLayout* radarLayout = new QVBoxLayout(radarCol);
    radarLayout->setAlignment(Qt::AlignCenter);
    QLabel* radarHeader = new QLabel("SKILL MAP", radarCol);
    radarHeader->setObjectName("analyticsCardHeader");
    radarHeader->setAlignment(Qt::AlignCenter);
    radarLayout->addWidget(radarHeader);
    m_radarChart->setParent(radarCol);
    m_radarChart->setMinimumSize(300, 300);
    m_radarChart->setMaximumSize(500, 500);
    radarLayout->addWidget(m_radarChart, 0, Qt::AlignCenter);
    analyticsLayout->addWidget(radarCol, 3);

    QWidget* barsCol = new QWidget(m_analyticsRow);
    barsCol->setObjectName("analyticsCard");
    QVBoxLayout* barsOuterLayout = new QVBoxLayout(barsCol);
    QLabel* barsHeader = new QLabel("TOPIC SCORES", barsCol);
    barsHeader->setObjectName("analyticsCardHeader");
    barsOuterLayout->addWidget(barsHeader);

    m_topicBarsPanel = new QWidget(barsCol);
    m_topicBarsLayout = new QVBoxLayout(m_topicBarsPanel);
    m_topicBarsLayout->setContentsMargins(0, 4, 0, 4);
    m_topicBarsLayout->setSpacing(6);
    barsOuterLayout->addWidget(m_topicBarsPanel, 1);
    barsOuterLayout->addStretch();
    barsCol->setMinimumWidth(300);
    analyticsLayout->addWidget(barsCol, 2);

    m_contentLayout->addWidget(m_analyticsRow);

    // ── Stats bar ──────────────────────────────────────────────────────────
    m_statsBar = new QWidget(m_contentWidget);
    m_statsBar->setObjectName("statsBar");
    QHBoxLayout* statsLayout = new QHBoxLayout(m_statsBar);
    statsLayout->setContentsMargins(20, 12, 20, 12);
    statsLayout->setSpacing(0);

    m_streakLabel  = new QLabel(m_statsBar); m_streakLabel->setObjectName("statItem");
    m_quizzesLabel = new QLabel(m_statsBar); m_quizzesLabel->setObjectName("statItem");
    m_timeLabel    = new QLabel(m_statsBar); m_timeLabel->setObjectName("statItem");

    statsLayout->addWidget(m_streakLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(m_quizzesLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(m_timeLabel);
    m_contentLayout->addWidget(m_statsBar);

    // ── Recommendations ────────────────────────────────────────────────────
    m_recsSection = new QWidget(m_contentWidget);
    QVBoxLayout* recsOuter = new QVBoxLayout(m_recsSection);
    recsOuter->setContentsMargins(0, 0, 0, 0);
    recsOuter->setSpacing(8);

    QLabel* recsHeader = new QLabel("\xf0\x9f\x93\x8b  Recommendations", m_recsSection);
    recsHeader->setObjectName("sectionHeader");
    recsOuter->addWidget(recsHeader);

    m_recsLayout = new QVBoxLayout();
    m_recsLayout->setSpacing(8);
    recsOuter->addLayout(m_recsLayout);
    m_contentLayout->addWidget(m_recsSection);

    m_contentLayout->addStretch();

    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &UserProfileWidget::applyTheme);
}

// ─────────────────────────────────────────────────────────────────────────────
// refresh
// ─────────────────────────────────────────────────────────────────────────────
void UserProfileWidget::refresh(int userId)
{
    if (userId < 0) return;
    clearDynamicContent();
    const UserRecord user        = UserManager::instance().currentUser();
    const int overallScore       = m_analyzer.overallScore(userId);
    const QList<UserTopicStatDTO> allStats = m_analyzer.allTopicStatsSorted(userId);
    buildAvatarCard(user, overallScore);
    buildAnalyticsRow(allStats, overallScore);
    buildStatsBar(userId);
    buildRecommendations(userId);
    m_scrollArea->verticalScrollBar()->setValue(0);
    applyTheme();
}

// ─────────────────────────────────────────────────────────────────────────────
// buildAvatarCard — FIX: show real avatar pixmap
// ─────────────────────────────────────────────────────────────────────────────
void UserProfileWidget::buildAvatarCard(const UserRecord& user, int overallScore)
{
    const int totalXp   = computeXpFromSessions(user.id);
    const int level     = computeLevel(totalXp);
    const int thisLevel = xpForLevel(level);
    const int nextLevel = xpForLevel(level + 1);
    const int xpInLevel = totalXp - thisLevel;
    const int xpNeeded  = nextLevel - thisLevel;
    const double xpFrac = xpNeeded > 0
                              ? static_cast<double>(xpInLevel) / xpNeeded : 1.0;

    // FIX: render actual avatar instead of "C++" label text
    if (m_avatarLabel) {
        m_avatarLabel->setPixmap(makeProfileAvatar(user, 72));
        m_avatarLabel->setText(QString()); // clear placeholder text
        m_avatarLabel->setStyleSheet(
            "QLabel { background: transparent; border-radius: 36px; }");
    }

    m_usernameLabel->setText(
        QString("%1%2  <span style='font-size:14px; font-weight:normal;'>%3</span>")
            .arg(user.displayName.isEmpty() ? user.username : user.displayName)
            .arg(user.isAdmin ? " \xf0\x9f\x91\x91" : "")
            .arg(levelTitle(level)));
    m_usernameLabel->setTextFormat(Qt::RichText);

    const int dotsFilled = qBound(0, level % 5, 5);
    QString dots;
    for (int i = 0; i < 5; ++i)
        dots += (i < dotsFilled ? "\xe2\x97\x8f" : "\xe2\x97\x8b");
    m_levelLabel->setText(QString("%1  Level %2").arg(dots).arg(level));

    const int fillPct = static_cast<int>(xpFrac * 100);
    m_xpBarFill->setFixedWidth(qMax(8, fillPct * 280 / 100));
    m_xpBarOuter->setText(
        QString("%1 / %2 XP to Level %3").arg(xpInLevel).arg(xpNeeded).arg(level + 1));

    Q_UNUSED(overallScore);
}

void UserProfileWidget::buildAnalyticsRow(const QList<UserTopicStatDTO>& stats,
                                          int overallScore)
{
    m_progressRing->setScore(overallScore);

    QList<QPair<QString, double>> radarData;
    for (const auto& s : stats) {
        if (s.attempts > 0)
            radarData.append({s.topicTitle, s.masteryLevel});
    }
    m_radarChart->setData(radarData);

    QLayoutItem* item;
    while ((item = m_topicBarsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QList<UserTopicStatDTO> sorted = stats;
    std::sort(sorted.begin(), sorted.end(),
              [](const UserTopicStatDTO& a, const UserTopicStatDTO& b) {
                  return a.masteryLevel > b.masteryLevel;
              });

    for (int i = 0; i < sorted.size(); ++i) {
        const auto& s = sorted[i];
        if (s.attempts == 0) continue;
        auto* bar = new TopicScoreBar(s.topicTitle, s.masteryLevel, m_topicBarsPanel);
        m_topicBarsLayout->addWidget(bar);
    }

    if (m_topicBarsLayout->count() == 0) {
        QLabel* empty = new QLabel("No topics attempted yet.", m_topicBarsPanel);
        empty->setObjectName("emptyLabel");
        m_topicBarsLayout->addWidget(empty);
    }
}

void UserProfileWidget::buildStatsBar(int userId)
{
    const int streak  = computeStreak(userId);
    const int quizzes = totalQuizzesCompleted(userId);
    const int timeSec = totalTimeSpentSec(userId);
    const int hours   = timeSec / 3600;
    const int mins    = (timeSec % 3600) / 60;

    m_streakLabel->setText(
        QString("\xf0\x9f\x94\xa5  Streak: <b>%1 day%2</b>").arg(streak).arg(streak == 1 ? "" : "s"));
    m_streakLabel->setTextFormat(Qt::RichText);

    m_quizzesLabel->setText(QString("\xe2\x9c\x85  Quizzes: <b>%1</b>").arg(quizzes));
    m_quizzesLabel->setTextFormat(Qt::RichText);

    m_timeLabel->setText(QString("\xe2\x8f\xb1  Time: <b>%1h %2m</b>").arg(hours).arg(mins));
    m_timeLabel->setTextFormat(Qt::RichText);
}

void UserProfileWidget::buildRecommendations(int userId)
{
    QLayoutItem* item;
    while ((item = m_recsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    const QList<RecommendationDTO> recs = m_repo.recommendationsForUser(userId);
    if (recs.isEmpty()) {
        QLabel* none = new QLabel(
            "\xf0\x9f\x8e\x89  All topics are on track! Complete more quizzes for personalised advice.",
            m_recsSection);
        none->setObjectName("emptyLabel");
        none->setWordWrap(true);
        m_recsLayout->addWidget(none);
        return;
    }

    for (const auto& rec : recs) {
        QWidget* row = new QWidget(m_recsSection);
        row->setObjectName("recRow");
        QHBoxLayout* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(12, 8, 12, 8);

        QLabel* text = new QLabel(
            QString("\xe2\x9a\xa0  %1").arg(rec.reason), row);
        text->setObjectName("recText");
        text->setWordWrap(true);
        text->setTextFormat(Qt::PlainText);
        rowLayout->addWidget(text, 1);

        if (!rec.refUrl.isEmpty()) {
            QPushButton* link = new QPushButton("\xf0\x9f\x93\x96  Study", row);
            link->setObjectName("recLinkBtn");
            link->setToolTip(rec.refUrl);
            connect(link, &QPushButton::clicked, this, [url = rec.refUrl]() {
                QDesktopServices::openUrl(QUrl(url));
            });
            rowLayout->addWidget(link);
        }
        m_recsLayout->addWidget(row);
    }
}

void UserProfileWidget::clearDynamicContent() {}

// ─────────────────────────────────────────────────────────────────────────────
// XP / Level
// ─────────────────────────────────────────────────────────────────────────────
int UserProfileWidget::computeLevel(int totalXp)
{
    int level = 0;
    while (totalXp >= xpForLevel(level + 1)) ++level;
    return level;
}

int UserProfileWidget::xpForLevel(int level)
{
    return 100 * level * (level + 1) / 2;
}

QString UserProfileWidget::levelTitle(int level)
{
    static const QStringList titles = {
        "Beginner","Apprentice","Explorer","Intermediate",
        "Practitioner","Advanced","Expert","Master","Guru","C++ Legend"
    };
    return titles.value(qMin(level, titles.size() - 1));
}

int UserProfileWidget::computeXpFromSessions(int userId)
{
    QSqlQuery q(QSqlDatabase::database(QuizDatabase::CONNECTION_NAME));
    q.prepare("SELECT COALESCE(SUM(score),0) FROM quiz_sessions WHERE user_id=:uid AND is_complete=1");
    q.bindValue(":uid", userId);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return 0;
}

int UserProfileWidget::computeStreak(int userId)
{
    QSqlQuery q(QSqlDatabase::database(QuizDatabase::CONNECTION_NAME));
    q.prepare(
        "SELECT DATE(started_at) AS day FROM quiz_sessions "
        "WHERE user_id=:uid AND is_complete=1 "
        "GROUP BY DATE(started_at) ORDER BY day DESC");
    q.bindValue(":uid", userId);
    if (!q.exec()) return 0;
    QDate today = QDate::currentDate();
    int streak = 0;
    QDate expected = today;
    while (q.next()) {
        QDate day = QDate::fromString(q.value(0).toString(), Qt::ISODate);
        if (day == expected || (streak == 0 && day == today.addDays(-1))) {
            expected = day.addDays(-1);
            ++streak;
        } else break;
    }
    return streak;
}

int UserProfileWidget::totalQuizzesCompleted(int userId)
{
    QSqlQuery q(QSqlDatabase::database(QuizDatabase::CONNECTION_NAME));
    q.prepare("SELECT COUNT(*) FROM quiz_sessions WHERE user_id=:uid AND is_complete=1");
    q.bindValue(":uid", userId);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return 0;
}

int UserProfileWidget::totalTimeSpentSec(int userId)
{
    QSqlQuery q(QSqlDatabase::database(QuizDatabase::CONNECTION_NAME));
    q.prepare("SELECT COALESCE(SUM(time_spent),0) FROM quiz_sessions WHERE user_id=:uid AND is_complete=1");
    q.bindValue(":uid", userId);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme (unchanged from original)
// ─────────────────────────────────────────────────────────────────────────────
void UserProfileWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        UserProfileWidget { background-color: %1; }
        #profileScroll { background-color: %1; border: none; }
        #avatarCard    { background-color: %2; border: 1px solid %3; border-radius: 8px; }
        #avatarMonogram {
            background-color: transparent;
            border-radius: 36px;
        }
        #usernameLabel { color: %5; font-size: 18px; font-weight: bold; }
        #levelLabel    { color: %6; font-size: 13px; }
        #xpLabel       { color: %7; font-size: 11px; }
        #xpBarContainer { background-color: %3; border-radius: 6px; min-height: 12px; max-height: 12px; }
        #xpBarFill      { background-color: %4; border-radius: 6px; min-height: 12px; max-height: 12px; }
        #analyticsCard  { background-color: %2; border: 1px solid %3; border-radius: 6px; padding: 8px; }
        #analyticsCardHeader { color: %7; font-size: 11px; font-weight: bold; letter-spacing: 1px; }
        #statsBar  { background-color: %2; border: 1px solid %3; border-radius: 6px; }
        #statItem  { color: %5; font-size: 13px; }
        #sectionHeader { color: %5; font-size: 14px; font-weight: bold; padding-bottom: 4px; border-bottom: 1px solid %3; }
        #recRow    { background-color: %2; border: 1px solid %8; border-radius: 4px; }
        #recText   { color: %5; font-size: 12px; }
        #recLinkBtn { background-color: transparent; color: %4; border: 1px solid %4; border-radius: 3px; padding: 3px 10px; font-size: 12px; }
        #recLinkBtn:hover { background-color: %4; color: white; }
        #emptyLabel { color: %7; font-size: 13px; padding: 8px; }
        QLabel { color: %5; }
    )")
                      .arg(t.windowBackground.name())     // %1
                      .arg(t.panelBackground.name())      // %2
                      .arg(t.border.name())               // %3
                      .arg(t.accent.name())               // %4
                      .arg(t.textPrimary.name())          // %5
                      .arg(t.textSecondary.name())        // %6
                      .arg(t.textSecondary.name())        // %7
                      .arg(t.warning.darker(150).name())  // %8
                  );
    m_progressRing->update();
    m_radarChart->update();

    if (m_scrollArea) {
        m_scrollArea->setStyleSheet(
            QString("QScrollArea { background-color: %1; border: none; }"
                    "QScrollArea > QWidget > QWidget { background-color: %1; }")
                .arg(t.windowBackground.name()));
        if (m_scrollArea->viewport())
            m_scrollArea->viewport()->setStyleSheet(
                QString("background-color: %1;").arg(t.windowBackground.name()));
        if (m_contentWidget)
            m_contentWidget->setStyleSheet(
                QString("background-color: %1;").arg(t.windowBackground.name()));
    }

    if (m_xpBarFill)
        m_xpBarFill->setStyleSheet(
            QString("background-color: %1; border-radius: 6px;").arg(t.accent.name()));
}
