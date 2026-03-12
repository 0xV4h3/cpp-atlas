#include "ui/QuizModeWindow.h"
#include "ui/QuizSelectionWidget.h"
#include "ui/QuizSessionWidget.h"
#include "ui/QuizResultsWidget.h"
#include "ui/UserProfileWidget.h"
#include "ui/CustomTestBuilderWidget.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShortcut>

QuizModeWindow::QuizModeWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &QuizModeWindow::applyTheme);

    // F11 fullscreen toggle — bubbles up to MainWindow
    QShortcut* fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
    connect(fullscreenShortcut, &QShortcut::activated, this, [this]() {
        if (QWidget* mw = window()) {
            if (mw->isFullScreen()) mw->showNormal();
            else mw->showFullScreen();
        }
    });
}

void QuizModeWindow::setCurrentUser(const UserRecord& user)
{
    if (m_userLabel) {
        m_userLabel->setText(
            QString("👤 %1%2")
            .arg(user.displayName)
            .arg(user.isAdmin ? "  👑" : ""));
    }
}

void QuizModeWindow::showSelectionScreen()
{
    m_pageHistory.clear();
    m_stack->setCurrentIndex(0);
    m_backBtn->setVisible(false);
}

void QuizModeWindow::navigateTo(int pageIndex, const QString& backLabel)
{
    const int current = m_stack->currentIndex();
    if (current != 0) {
        if (m_pageHistory.isEmpty() || m_pageHistory.top() != current)
            m_pageHistory.push(current);
    }
    m_stack->setCurrentIndex(pageIndex);
    m_backBtn->setText(backLabel);
    m_backBtn->setVisible(pageIndex != 0);
}

void QuizModeWindow::navigateBack()
{
    if (m_pageHistory.isEmpty()) {
        showSelectionScreen();
        return;
    }
    const int prev = m_pageHistory.pop();
    m_stack->setCurrentIndex(prev);
    if (prev == 0) {
        m_backBtn->setVisible(false);
    } else {
        m_backBtn->setVisible(true);
        const QString label = (!m_pageHistory.isEmpty() && m_pageHistory.top() == 4)
                              ? "← My Tests" : "← Back";
        m_backBtn->setText(label);
    }
}

void QuizModeWindow::onExitClicked()
{
    emit exitRequested();
}

void QuizModeWindow::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupHeader();
    mainLayout->addWidget(m_header);

    m_stack = new QStackedWidget(this);

    // Page 0: Selection
    m_selectionWidget = new QuizSelectionWidget(this);
    m_stack->addWidget(m_selectionWidget);

    // Page 1: Quiz Session
    m_sessionWidget = new QuizSessionWidget(this);
    m_stack->addWidget(m_sessionWidget);

    // Page 2: Quiz Results
    m_resultsWidget = new QuizResultsWidget(this);
    m_stack->addWidget(m_resultsWidget);

    // Page 3: User Profile Dashboard
    m_profileWidget = new UserProfileWidget(this);
    m_stack->addWidget(m_profileWidget);

    // Wire profile signals
    connect(m_profileWidget, &UserProfileWidget::startQuizFromRecommendation,
            this, [this](int /*topicId*/) {
        // Future: filter quizzes by topic; for now go to selection
        showSelectionScreen();
    });

    // Page 4: Custom Test Builder
    m_builderWidget = new CustomTestBuilderWidget(this);
    m_stack->addWidget(m_builderWidget);

    // Wire builder signals
    connect(m_builderWidget, &CustomTestBuilderWidget::backRequested,
            this, &QuizModeWindow::navigateBack);
    connect(m_builderWidget, &CustomTestBuilderWidget::launchCustomTest,
            this, [this](const QList<QuestionDTO>& questions) {
        const int userId = UserManager::instance().currentUser().id;
        m_lastQuestions = questions;
        m_lastUserId    = userId;
        m_lastMode      = "practice";
        m_sessionWidget->startCustomSession(questions, userId, "practice");
        navigateTo(1, "← Back");
    });
    // Update back button label when builder switches between My Tests / Builder sub-page
    connect(m_builderWidget, &CustomTestBuilderWidget::subPageChanged,
            this, [this](int sub) {
        if (sub == 1) {
            m_backBtn->setText("← My Tests");
            m_backBtn->setVisible(true);
        } else {
            m_backBtn->setText("← Back");
            m_backBtn->setVisible(true);  // still on Page 4
        }
    });

    // Wire session signals
    connect(m_sessionWidget, &QuizSessionWidget::sessionCompleted,
            this, &QuizModeWindow::onSessionCompleted);
    connect(m_sessionWidget, &QuizSessionWidget::sessionAbandoned,
            this, &QuizModeWindow::onSessionAbandoned);

    // Wire results signals
    connect(m_resultsWidget, &QuizResultsWidget::retryRequested,
            this, &QuizModeWindow::onRetryRequested);
    connect(m_resultsWidget, &QuizResultsWidget::backToSelectionRequested,
            this, &QuizModeWindow::showSelectionScreen);

    mainLayout->addWidget(m_stack, 1);

    // Connect selection widget
    connect(m_selectionWidget, &QuizSelectionWidget::quizSelected,
            this, [this](int quizId) {
        const int userId = UserManager::instance().currentUser().id;
        launchQuiz(quizId, "practice", true, userId);
    });
}

void QuizModeWindow::setupHeader()
{
    m_header = new QWidget(this);
    m_header->setObjectName("quizModeHeader");
    m_header->setFixedHeight(52);

    QHBoxLayout* layout = new QHBoxLayout(m_header);
    layout->setContentsMargins(20, 0, 20, 0);
    layout->setSpacing(12);

    // Back button (hidden on selection screen)
    m_backBtn = new QPushButton("← Back", m_header);
    m_backBtn->setObjectName("quizNavButton");
    m_backBtn->setVisible(false);
    connect(m_backBtn, &QPushButton::clicked, this, &QuizModeWindow::navigateBack);
    layout->addWidget(m_backBtn);

    // Title
    m_titleLabel = new QLabel("📚  Quiz Mode — C++ Learning", m_header);
    m_titleLabel->setObjectName("quizModeTitle");
    layout->addWidget(m_titleLabel);

    layout->addStretch();

    // Profile button
    m_profileBtn = new QPushButton("👤  Profile", m_header);
    m_profileBtn->setObjectName("quizNavButton");
    connect(m_profileBtn, &QPushButton::clicked,
            this, &QuizModeWindow::onProfileClicked);
    layout->addWidget(m_profileBtn);

    // Custom Test Builder button
    m_builderBtn = new QPushButton("🔧  Custom Test", m_header);
    m_builderBtn->setObjectName("quizNavButton");
    connect(m_builderBtn, &QPushButton::clicked,
            this, &QuizModeWindow::onBuilderClicked);
    layout->addWidget(m_builderBtn);

    // User display
    m_userLabel = new QLabel(m_header);
    m_userLabel->setObjectName("quizUserLabel");
    layout->addWidget(m_userLabel);

    layout->addSpacing(16);

    // Exit button
    m_exitBtn = new QPushButton("✕  Exit Quiz Mode", m_header);
    m_exitBtn->setObjectName("quizExitButton");
    connect(m_exitBtn, &QPushButton::clicked, this, &QuizModeWindow::onExitClicked);
    layout->addWidget(m_exitBtn);
}

void QuizModeWindow::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        QuizModeWindow {
            background-color: %1;
        }
        #quizModeHeader {
            background-color: %2;
            border-bottom: 2px solid %3;
        }
        #quizModeTitle {
            color: %4;
            font-size: 15px;
            font-weight: bold;
        }
        #quizUserLabel {
            color: %5;
            font-size: 13px;
        }
        #quizNavButton {
            background-color: transparent;
            color: %6;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 13px;
        }
        #quizNavButton:hover { background-color: %7; }
        #quizExitButton {
            background-color: transparent;
            color: %8;
            border: 1px solid %8;
            border-radius: 4px;
            padding: 4px 14px;
            font-size: 13px;
        }
        #quizExitButton:hover { background-color: %8; color: white; }
        #placeholderLabel {
            color: %5;
            font-size: 18px;
        }
    )")
    .arg(t.windowBackground.name())    // %1
    .arg(t.toolbarBackground.name())   // %2
    .arg(t.border.name())              // %3
    .arg(t.textPrimary.name())         // %4
    .arg(t.textSecondary.name())       // %5
    .arg(t.accent.name())              // %6
    .arg(t.accent.darker(140).name())  // %7
    .arg(t.error.name())               // %8
    );
}
void QuizModeWindow::launchQuiz(int quizId, const QString& mode,
                                 bool shuffle, int userId)
{
    m_lastQuizId  = quizId;
    m_lastMode    = mode;
    m_lastShuffle = shuffle;
    m_lastUserId  = userId;

    navigateTo(1, "← Back");
    m_sessionWidget->startQuiz(quizId, userId, mode, shuffle);
}

void QuizModeWindow::onSessionCompleted(const SessionResult& result)
{
    m_stack->setCurrentIndex(2);
    m_backBtn->setVisible(false);
    m_pageHistory.clear();
    m_resultsWidget->showResults(result,
                                  m_sessionWidget->lastQuestions(),
                                  m_lastUserId);
}

void QuizModeWindow::onSessionAbandoned()
{
    showSelectionScreen();
}

void QuizModeWindow::onRetryRequested()
{
    launchQuiz(m_lastQuizId, m_lastMode, m_lastShuffle, m_lastUserId);
}

void QuizModeWindow::showProfilePage()
{
    const int userId = UserManager::instance().currentUser().id;
    m_profileWidget->refresh(userId);
    navigateTo(3, "← Back");
}

void QuizModeWindow::onProfileClicked()
{
    showProfilePage();
}

void QuizModeWindow::showBuilderPage()
{
    m_builderWidget->refresh();
    navigateTo(4, "← Back");
}

void QuizModeWindow::onBuilderClicked()
{
    showBuilderPage();
}
