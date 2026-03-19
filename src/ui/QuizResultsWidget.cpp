#include "ui/QuizResultsWidget.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QDesktopServices>
#include <QUrl>
#include <QtMath>
#include <QScrollArea>
#include <QScrollBar>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// RadarChartWidget
// ─────────────────────────────────────────────────────────────────────────────
RadarChartWidget::RadarChartWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(240, 240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void RadarChartWidget::setData(const QList<QPair<QString, double>>& data)
{
    m_data = data;
    update();
}

void RadarChartWidget::paintEvent(QPaintEvent*)
{
    const int n = m_data.size();
    if (n < 2) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const Theme& t = ThemeManager::instance()->currentTheme();

    QFont labelFont = p.font();
    const int fontPx = qMax(7, 9 - qMax(0, (n - 8) / 4));
    labelFont.setPixelSize(fontPx);
    p.setFont(labelFont);
    QFontMetrics fm(labelFont);

    int maxLabelW = 0;
    for (const auto& pair : m_data)
        maxLabelW = qMax(maxLabelW, fm.horizontalAdvance(pair.first));
    const int halfMin    = qMin(width(), height()) / 2;
    const int margin     = qMin(maxLabelW + 8, static_cast<int>(halfMin * 0.28));

    const QPointF center(width() / 2.0, height() / 2.0);
    const double  radius = halfMin - margin;
    if (radius < 10) return;

    const double angleStep = 2.0 * M_PI / n;

    // ── Grid rings ────────────────────────────────────────────────────────
    const int rings = 4;
    for (int ring = rings; ring >= 1; --ring) {
        const double r = radius * ring / double(rings);
        QPolygonF poly;
        for (int i = 0; i < n; ++i) {
            const double a = -M_PI / 2.0 + i * angleStep;
            poly << QPointF(center.x() + r * qCos(a), center.y() + r * qSin(a));
        }
        QColor gc(t.border.name());
        gc.setAlpha(ring == rings ? 90 : 50);
        p.setPen(QPen(gc, 1, ring == rings ? Qt::SolidLine : Qt::DotLine));
        p.setBrush(Qt::NoBrush);
        p.drawPolygon(poly);
    }

    // ── Axis spokes ───────────────────────────────────────────────────────
    p.setPen(QPen(QColor(t.border.name()), 1));
    for (int i = 0; i < n; ++i) {
        const double a = -M_PI / 2.0 + i * angleStep;
        p.drawLine(center, QPointF(center.x() + radius * qCos(a),
                                   center.y() + radius * qSin(a)));
    }

    // ── Data polygon ──────────────────────────────────────────────────────
    QPolygonF dataPoly;
    for (int i = 0; i < n; ++i) {
        const double a   = -M_PI / 2.0 + i * angleStep;
        const double val = qBound(0.0, m_data[i].second, 1.0);
        dataPoly << QPointF(center.x() + radius * val * qCos(a),
                            center.y() + radius * val * qSin(a));
    }
    QColor fill(t.accent.name());
    fill.setAlpha(65);
    p.setBrush(fill);
    p.setPen(QPen(QColor(t.accent.name()), 2));
    p.drawPolygon(dataPoly);

    // ── Dots ──────────────────────────────────────────────────────────────
    p.setBrush(QColor(t.accent.name()));
    p.setPen(Qt::NoPen);
    for (const QPointF& pt : dataPoly)
        p.drawEllipse(pt, 3.5, 3.5);

    // ── Labels ────────────────────────────────────────────────────────────
    p.setPen(QColor(t.textPrimary.name()));
    p.setFont(labelFont);

    const int boxW = qMin(maxLabelW + 6, margin * 2);
    const int boxH = fm.height() + 4;

    for (int i = 0; i < n; ++i) {
        const double a      = -M_PI / 2.0 + i * angleStep;
        // Place label centre at radius + 60% of margin along the spoke
        const double labelR = radius + margin * 0.65;
        const double lx     = center.x() + labelR * qCos(a);
        const double ly     = center.y() + labelR * qSin(a);

        const QString label = fm.elidedText(m_data[i].first, Qt::ElideRight, boxW);

        Qt::AlignmentFlag hAlign;
        if      (lx < center.x() - 4)  hAlign = Qt::AlignRight;
        else if (lx > center.x() + 4)  hAlign = Qt::AlignLeft;
        else                            hAlign = Qt::AlignHCenter;

        const QRectF box(lx - boxW / 2.0, ly - boxH / 2.0, boxW, boxH);
        p.drawText(box, hAlign | Qt::AlignVCenter, label);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// QuizResultsWidget
// ─────────────────────────────────────────────────────────────────────────────
QuizResultsWidget::QuizResultsWidget(QWidget* parent)
    : QWidget(parent)
    , m_radarChart(new RadarChartWidget(this))
{
    // ── Outer layout ───────────────────────────────────────────────────────
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // ── Scrollable content ─────────────────────────────────────────────────
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName("resultsScroll");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_contentWidget = new QWidget(m_scrollArea);
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(60, 30, 60, 30);
    m_contentLayout->setSpacing(24);
    m_scrollArea->setWidget(m_contentWidget);
    outerLayout->addWidget(m_scrollArea, 1);

    // ── Score card (populated in showResults) ─────────────────────────────
    m_scoreCard = new QWidget(m_contentWidget);
    m_scoreCard->setObjectName("scoreCard");
    QHBoxLayout* scoreLayout = new QHBoxLayout(m_scoreCard);
    scoreLayout->setContentsMargins(24, 20, 24, 20);
    scoreLayout->setSpacing(24);

    // Left: big score
    QVBoxLayout* bigScoreLayout = new QVBoxLayout();
    m_scoreBigLabel = new QLabel("0", m_scoreCard);
    m_scoreBigLabel->setObjectName("scoreBigLabel");
    m_scoreBigLabel->setAlignment(Qt::AlignCenter);
    m_scoreSubLabel = new QLabel("/ 0 points", m_scoreCard);
    m_scoreSubLabel->setObjectName("scoreSubLabel");
    m_scoreSubLabel->setAlignment(Qt::AlignCenter);
    bigScoreLayout->addWidget(m_scoreBigLabel);
    bigScoreLayout->addWidget(m_scoreSubLabel);
    scoreLayout->addLayout(bigScoreLayout);

    // Right: stats
    QVBoxLayout* statsLayout = new QVBoxLayout();
    m_timeLabel  = new QLabel(m_scoreCard);
    m_timeLabel->setObjectName("statsLabel");
    m_statsLabel = new QLabel(m_scoreCard);
    m_statsLabel->setObjectName("statsLabel");
    statsLayout->addWidget(m_timeLabel);
    statsLayout->addWidget(m_statsLabel);
    statsLayout->addStretch();
    scoreLayout->addLayout(statsLayout, 1);

    // Radar chart (right side of score card)
    m_radarChart->setMinimumSize(200, 200);
    m_radarChart->setMaximumSize(260, 260);
    scoreLayout->addWidget(m_radarChart);

    m_contentLayout->addWidget(m_scoreCard);

    // ── Weak areas section ────────────────────────────────────────────────
    m_weakSection = new QWidget(m_contentWidget);
    QVBoxLayout* weakOuter = new QVBoxLayout(m_weakSection);
    weakOuter->setContentsMargins(0, 0, 0, 0);
    weakOuter->setSpacing(8);

    QLabel* weakTitle = new QLabel("📉  Areas to Improve", m_weakSection);
    weakTitle->setObjectName("sectionHeader");
    weakOuter->addWidget(weakTitle);

    m_weakLayout = new QVBoxLayout();
    weakOuter->addLayout(m_weakLayout);
    m_contentLayout->addWidget(m_weakSection);

    // ── Review section ────────────────────────────────────────────────────
    m_reviewSection = new QWidget(m_contentWidget);
    QVBoxLayout* reviewOuter = new QVBoxLayout(m_reviewSection);
    reviewOuter->setContentsMargins(0, 0, 0, 0);
    reviewOuter->setSpacing(8);

    QLabel* reviewTitle = new QLabel("📋  Question Review", m_reviewSection);
    reviewTitle->setObjectName("sectionHeader");
    reviewOuter->addWidget(reviewTitle);

    m_reviewLayout = new QVBoxLayout();
    m_reviewLayout->setSpacing(12);
    reviewOuter->addLayout(m_reviewLayout);
    m_contentLayout->addWidget(m_reviewSection);

    m_contentLayout->addStretch();

    // ── Footer ────────────────────────────────────────────────────────────
    QWidget* footer = new QWidget(this);
    footer->setObjectName("resultsFooter");
    footer->setFixedHeight(60);
    QHBoxLayout* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(60, 8, 60, 8);
    footerLayout->setSpacing(12);

    m_retryBtn = new QPushButton("🔁  Try Again", footer);
    m_retryBtn->setObjectName("retryButton");
    m_retryBtn->setMinimumWidth(140);
    connect(m_retryBtn, &QPushButton::clicked, this, &QuizResultsWidget::retryRequested);
    footerLayout->addWidget(m_retryBtn);

    footerLayout->addStretch();

    m_backBtn = new QPushButton("← Back to Quiz Selection", footer);
    m_backBtn->setObjectName("backButton");
    m_backBtn->setMinimumWidth(200);
    connect(m_backBtn, &QPushButton::clicked,
            this, &QuizResultsWidget::backToSelectionRequested);
    footerLayout->addWidget(m_backBtn);

    outerLayout->addWidget(footer);

    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &QuizResultsWidget::applyTheme);
}

// ─────────────────────────────────────────────────────────────────────────────
// showResults — main entry point
// ─────────────────────────────────────────────────────────────────────────────
void QuizResultsWidget::showResults(const SessionResult& result,
                                     const QList<QuestionDTO>& questions,
                                     int userId)
{
    clearContent();
    buildScoreCard(result);

    const QList<UserTopicStatDTO> allStats = m_analyzer.allTopicStatsSorted(userId);
    buildRadarChart(allStats);

    const QList<UserTopicStatDTO> weak = m_analyzer.weakTopics(userId);
    buildWeakAreas(weak);

    buildReviewList(result, questions);

    m_scrollArea->verticalScrollBar()->setValue(0);
    applyTheme();
}

// ─────────────────────────────────────────────────────────────────────────────
// Section builders
// ─────────────────────────────────────────────────────────────────────────────
void QuizResultsWidget::buildScoreCard(const SessionResult& result)
{
    m_scoreBigLabel->setText(QString::number(result.score));
    m_scoreSubLabel->setText(QString("/ %1 pts  (%2%)")
                              .arg(result.maxScore)
                              .arg(static_cast<int>(result.percentage)));

    const int mins = result.totalTimeSec / 60;
    const int secs = result.totalTimeSec % 60;
    m_timeLabel->setText(QString("⏱  Time: %1:%2")
                          .arg(mins).arg(secs, 2, 10, QChar('0')));

    m_statsLabel->setText(QString("✅  %1 / %2 correct")
                           .arg(result.correctCount)
                           .arg(result.totalQuestions));

    // Color the big score based on percentage
    const Theme& t = ThemeManager::instance()->currentTheme();
    const QString color = result.percentage >= 80.0 ? t.success.name()
                        : result.percentage >= 50.0 ? t.warning.name()
                        : t.error.name();
    m_scoreBigLabel->setStyleSheet(
        QString("color: %1; font-size: 52px; font-weight: bold;").arg(color));
}

void QuizResultsWidget::buildRadarChart(const QList<UserTopicStatDTO>& stats)
{
    QList<QPair<QString, double>> data;
    for (const auto& s : stats) {
        // Only show root-level topics (level 0) in radar chart for clarity
        if (s.attempts > 0) {
            data.append({s.topicTitle, s.masteryLevel});
        }
    }

    m_radarChart->setData(data);
    m_radarChart->setVisible(!data.isEmpty());
}

void QuizResultsWidget::buildWeakAreas(const QList<UserTopicStatDTO>& weak)
{
    // Clear previous
    QLayoutItem* item;
    while ((item = m_weakLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (weak.isEmpty()) {
        QLabel* allGood = new QLabel("🎉  All practiced topics are above the threshold. Keep it up!", m_weakSection);
        allGood->setObjectName("weakItemLabel");
        m_weakLayout->addWidget(allGood);
        return;
    }

    for (const auto& s : weak) {
        const TopicDTO topic = m_repo.topicById(s.topicId);
        const QString url    = topic.refUrl.isEmpty() ? topic.refUrl2 : topic.refUrl;

        QWidget* row = new QWidget(m_weakSection);
        row->setObjectName("weakItemRow");
        QHBoxLayout* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(12, 8, 12, 8);

        QLabel* topicLabel = new QLabel(
            QString("📉  <b>%1</b>  —  %2%")
            .arg(s.topicTitle)
            .arg(static_cast<int>(s.masteryLevel * 100)),
            row);
        topicLabel->setObjectName("weakItemLabel");
        topicLabel->setTextFormat(Qt::RichText);
        rowLayout->addWidget(topicLabel, 1);

        if (!url.isEmpty()) {
            QPushButton* refBtn = new QPushButton("📖  Study", row);
            refBtn->setObjectName("refButton");
            refBtn->setToolTip(url);
            connect(refBtn, &QPushButton::clicked, this, [url]() {
                QDesktopServices::openUrl(QUrl(url));
            });
            rowLayout->addWidget(refBtn);
        }
        m_weakLayout->addWidget(row);
    }
}

void QuizResultsWidget::buildReviewList(const SessionResult& result,
                                         const QList<QuestionDTO>& questions)
{
    QLayoutItem* item;
    while ((item = m_reviewLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (int i = 0; i < result.attempts.size(); ++i) {
        const AttemptRecord& attempt = result.attempts[i];

        // Find matching question
        const QuestionDTO* q = nullptr;
        for (const auto& qst : questions) {
            if (qst.id == attempt.questionId) { q = &qst; break; }
        }
        if (!q) continue;

        QWidget* card = new QWidget(m_reviewSection);
        card->setObjectName(attempt.isCorrect ? "reviewCardCorrect" : "reviewCardWrong");
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        cardLayout->setSpacing(6);

        // Question header
        QHBoxLayout* questionHeader = new QHBoxLayout();
        QLabel* iconLabel = new QLabel(attempt.isCorrect ? "✅" : "❌", card);
        iconLabel->setObjectName("reviewIcon");
        iconLabel->setFixedWidth(24);
        QLabel* qNumLabel = new QLabel(
            QString("<b>Q%1.</b>  %2").arg(i + 1).arg(q->content), card);
        qNumLabel->setObjectName("reviewQuestionText");
        qNumLabel->setWordWrap(true);
        qNumLabel->setTextFormat(Qt::RichText);
        questionHeader->addWidget(iconLabel);
        questionHeader->addWidget(qNumLabel, 1);
        cardLayout->addLayout(questionHeader);

        // Code snippet (if any)
        if (!q->codeSnippet.isEmpty()) {
            QLabel* codeLabel = new QLabel(q->codeSnippet, card);
            codeLabel->setObjectName("reviewCode");
            codeLabel->setWordWrap(false);
            codeLabel->setTextFormat(Qt::PlainText);
            cardLayout->addWidget(codeLabel);
        }

        // User answer
        const QString userAnswerText = [&]() -> QString {
            if (attempt.userAnswer.isEmpty()) return "(skipped)";
            bool ok = false;
            int optId = attempt.userAnswer.toInt(&ok);
            if (ok) {
                for (const auto& opt : q->options)
                    if (opt.id == optId) return opt.content;
            }
            return attempt.userAnswer;
        }();

        QLabel* userAnswerLabel = new QLabel(
            QString("Your answer: <i>%1</i>").arg(userAnswerText.toHtmlEscaped()), card);
        userAnswerLabel->setObjectName("reviewAnswerLabel");
        userAnswerLabel->setTextFormat(Qt::RichText);
        cardLayout->addWidget(userAnswerLabel);

        // Correct answer (always show)
        const QString correctText = [&]() -> QString {
            QStringList correct;
            for (const auto& opt : q->options)
                if (opt.isCorrect) correct << opt.content;
            return correct.join(", ");
        }();

        QLabel* correctLabel = new QLabel(
            QString("Correct: <b>%1</b>").arg(correctText.toHtmlEscaped()), card);
        correctLabel->setObjectName("reviewCorrectLabel");
        correctLabel->setTextFormat(Qt::RichText);
        cardLayout->addWidget(correctLabel);

        // Explanation (collapsible — always shown here for simplicity)
        if (!q->explanation.isEmpty()) {
            QLabel* explLabel = new QLabel(
                QString("<i>%1</i>").arg(q->explanation.toHtmlEscaped()), card);
            explLabel->setObjectName("reviewExplanation");
            explLabel->setWordWrap(true);
            explLabel->setTextFormat(Qt::RichText);
            cardLayout->addWidget(explLabel);
        }

        // Reference link
        if (!q->refUrl.isEmpty()) {
            QPushButton* refBtn = new QPushButton("📖  Learn more", card);
            refBtn->setObjectName("refButton");
            refBtn->setToolTip(q->refUrl);
            connect(refBtn, &QPushButton::clicked, this, [url = q->refUrl]() {
                QDesktopServices::openUrl(QUrl(url));
            });
            cardLayout->addWidget(refBtn, 0, Qt::AlignLeft);
        }

        m_reviewLayout->addWidget(card);
    }
}

void QuizResultsWidget::clearContent()
{
    // The section layouts are cleared by the builders themselves.
    // Score card labels are overwritten by buildScoreCard().
    // Radar chart data is replaced by buildRadarChart().
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────
void QuizResultsWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        QuizResultsWidget { background-color: %1; }
        #resultsScroll { background-color: %1; border: none; }

        #scoreCard {
            background-color: %2;
            border: 1px solid %3;
            border-radius: 8px;
        }
        #scoreSubLabel  { color: %4; font-size: 16px; }
        #statsLabel     { color: %4; font-size: 13px; }

        #sectionHeader {
            color: %5;
            font-size: 15px;
            font-weight: bold;
            padding-bottom: 4px;
            border-bottom: 1px solid %3;
        }

        #weakItemRow {
            background-color: %6;
            border: 1px solid %7;
            border-radius: 4px;
        }
        #weakItemLabel { color: %5; font-size: 13px; }

        #reviewCardCorrect {
            background-color: %8;
            border: 1px solid %9;
            border-radius: 6px;
        }
        #reviewCardWrong {
            background-color: %10;
            border: 1px solid %11;
            border-radius: 6px;
        }
        #reviewIcon            { font-size: 16px; }
        #reviewQuestionText    { color: %5; font-size: 13px; }
        #reviewCode {
            background-color: %12;
            color: %13;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 12px;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 6px 10px;
        }
        #reviewAnswerLabel  { color: %4; font-size: 12px; }
        #reviewCorrectLabel { color: %14; font-size: 12px; }
        #reviewExplanation  { color: %4; font-size: 12px; font-style: italic; }

        #refButton {
            background-color: transparent;
            color: %15;
            border: 1px solid %15;
            border-radius: 3px;
            padding: 3px 10px;
            font-size: 12px;
        }
        #refButton:hover { background-color: %15; color: white; }

        #resultsFooter { background-color: %2; border-top: 1px solid %3; }
        #retryButton {
            background-color: %15;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
            font-weight: bold;
            padding: 8px 16px;
        }
        #retryButton:hover { background-color: %16; }
        #backButton {
            background-color: transparent;
            color: %4;
            border: 1px solid %3;
            border-radius: 4px;
            font-size: 13px;
            padding: 8px 16px;
        }
        #backButton:hover { border-color: %15; color: %15; }
        QLabel { color: %5; }
    )")
    .arg(t.windowBackground.name())          // %1
    .arg(t.panelBackground.name())           // %2
    .arg(t.border.name())                    // %3
    .arg(t.textSecondary.name())             // %4
    .arg(t.textPrimary.name())               // %5
    .arg(t.sidebarBackground.name())         // %6
    .arg(t.border.name())                    // %7
    .arg(t.success.darker(250).name())       // %8  (correct card bg)
    .arg(t.success.darker(150).name())       // %9  (correct card border)
    .arg(t.error.darker(300).name())         // %10 (wrong card bg)
    .arg(t.error.darker(180).name())         // %11 (wrong card border)
    .arg(t.editorBackground.name())          // %12 (code bg)
    .arg(t.editorForeground.name())          // %13 (code fg)
    .arg(t.success.name())                   // %14 (correct answer text)
    .arg(t.accent.name())                    // %15 (ref button + retry)
    .arg(t.accent.lighter(115).name())       // %16 (retry hover)
    );
    m_radarChart->update();

    // Force scroll area and viewport to use window background
    if (m_scrollArea) {
        m_scrollArea->setStyleSheet(
            QString("QScrollArea { background-color: %1; border: none; }"
                    "QScrollArea > QWidget > QWidget { background-color: %1; }")
            .arg(t.windowBackground.name())
        );
        if (m_scrollArea->viewport())
            m_scrollArea->viewport()->setStyleSheet(
                QString("background-color: %1;").arg(t.windowBackground.name())
            );
    }
    if (m_contentWidget)
        m_contentWidget->setStyleSheet(
            QString("QWidget { background-color: %1; }").arg(t.windowBackground.name())
        );
}
