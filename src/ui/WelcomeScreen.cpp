// WelcomeScreen.cpp — complete file
// Changes vs original:
//   + IDE Mode button emits ideModeRequested() instead of being a checkable toggle
//   + "Continue without Project" text fixed (capital P)
//   + setReturnToProjectVisible() hides/shows both footer buttons (mutex)
//   + setCurrentUser() unchanged

#include "ui/WelcomeScreen.h"
#include "core/RecentProjectsManager.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QFont>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QShortcut>

WelcomeScreen::WelcomeScreen(QWidget *parent) : QWidget(parent)
{
    setupUI();
    loadRecentProjects();
    applyTheme();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &WelcomeScreen::applyTheme);

    QShortcut* fs = new QShortcut(QKeySequence(Qt::Key_F11), this);
    connect(fs, &QShortcut::activated, this, [this]() {
        if (QWidget* mw = window()) {
            if (mw->isFullScreen()) mw->showNormal();
            else                    mw->showFullScreen();
        }
    });
}

void WelcomeScreen::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(60, 40, 60, 40);
    mainLayout->setSpacing(30);

    // Header
    QHBoxLayout* headerLayout = new QHBoxLayout();
    m_logoLabel = new QLabel(this);
    m_logoLabel->setText("C++");
    m_logoLabel->setObjectName("welcomeLogo");

    QVBoxLayout* titleLayout = new QVBoxLayout();
    m_titleLabel = new QLabel("CppAtlas", this);
    m_titleLabel->setObjectName("welcomeTitle");
    m_subtitleLabel = new QLabel("C++ Learning IDE", this);
    m_subtitleLabel->setObjectName("welcomeSubtitle");
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_subtitleLabel);
    titleLayout->addStretch();

    headerLayout->addWidget(m_logoLabel);
    headerLayout->addSpacing(20);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // User info bar
    m_userInfoLabel = new QLabel(this);
    m_userInfoLabel->setObjectName("userInfoBar");
    m_userInfoLabel->setVisible(false);
    mainLayout->addWidget(m_userInfoLabel);

    // Mode selection
    createModeSelectionArea();
    mainLayout->addWidget(m_modeSelector);

    // Main content
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(40);

    QWidget* recentWidget = new QWidget(this);
    createRecentProjectsArea(recentWidget);
    contentLayout->addWidget(recentWidget, 2);

    QWidget* actionsWidget = new QWidget(this);
    createQuickActionsArea(actionsWidget);
    contentLayout->addWidget(actionsWidget, 1);
    mainLayout->addLayout(contentLayout, 1);

    // Footer: Return to Project | Continue without Project (mutual exclusion)
    QHBoxLayout* footerLayout = new QHBoxLayout();

    m_returnToProjectBtn = new QPushButton("Return to Project", this);
    m_returnToProjectBtn->setObjectName("continueButton");
    m_returnToProjectBtn->setVisible(false);
    connect(m_returnToProjectBtn, &QPushButton::clicked,
            this, &WelcomeScreen::returnToProjectRequested);
    footerLayout->addWidget(m_returnToProjectBtn);

    footerLayout->addStretch();

    // "Continue without Project" — capital P, visible by default (no project open at start)
    m_continueWithoutProjectBtn = new QPushButton("Continue without Project", this);
    m_continueWithoutProjectBtn->setObjectName("continueButton");
    m_continueWithoutProjectBtn->setVisible(true);
    connect(m_continueWithoutProjectBtn, &QPushButton::clicked,
            this, &WelcomeScreen::continueWithoutProjectRequested);
    footerLayout->addWidget(m_continueWithoutProjectBtn);

    mainLayout->addLayout(footerLayout);
}

void WelcomeScreen::createModeSelectionArea()
{
    m_modeSelector = new QWidget(this);
    m_modeSelector->setObjectName("modeSelector");

    QHBoxLayout* layout = new QHBoxLayout(m_modeSelector);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);

    // IDE Mode: action button (not checkable) — emits ideModeRequested()
    m_ideModeBtn = new QPushButton(this);
    m_ideModeBtn->setObjectName("modeButton");
    m_ideModeBtn->setText("IDE Mode\nC++ Development Environment");
    m_ideModeBtn->setMinimumSize(250, 80);
    connect(m_ideModeBtn, &QPushButton::clicked,
            this, &WelcomeScreen::ideModeRequested);

    // Quiz Mode
    m_quizModeBtn = new QPushButton(this);
    m_quizModeBtn->setObjectName("modeButton");
    m_quizModeBtn->setText("Quiz Mode\nC++ Learning & Assessment");
    m_quizModeBtn->setMinimumSize(250, 80);
    connect(m_quizModeBtn, &QPushButton::clicked,
            this, &WelcomeScreen::quizModeRequested);

    layout->addStretch();
    layout->addWidget(m_ideModeBtn);
    layout->addWidget(m_quizModeBtn);
    layout->addStretch();
}

void WelcomeScreen::createQuickActionsArea(QWidget* actionsWidget)
{
    actionsWidget->setObjectName("quickActionsPanel");
    QVBoxLayout* layout = new QVBoxLayout(actionsWidget);
    layout->setSpacing(12);

    QLabel* title = new QLabel("Get Started", actionsWidget);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    auto addBtn = [&](const QString& text, auto signal) -> QPushButton* {
        QPushButton* btn = new QPushButton(text, actionsWidget);
        btn->setObjectName("quickActionButton");
        btn->setMinimumHeight(40);
        connect(btn, &QPushButton::clicked, this, signal);
        layout->addWidget(btn);
        return btn;
    };

    m_newFileBtn       = addBtn("New File",          &WelcomeScreen::newFileRequested);
    m_openFileBtn      = addBtn("Open File...",      &WelcomeScreen::openFileRequested);
    m_openFolderBtn    = addBtn("Open Folder...",    &WelcomeScreen::openFolderRequested);
    m_createProjectBtn = addBtn("Create Project...", &WelcomeScreen::createProjectRequested);
    m_openProjectBtn   = addBtn("Open Project...",   &WelcomeScreen::openProjectRequested);

    layout->addStretch();
}

void WelcomeScreen::createRecentProjectsArea(QWidget* recentWidget)
{
    recentWidget->setObjectName("recentProjectsPanel");
    QVBoxLayout* layout = new QVBoxLayout(recentWidget);
    layout->setSpacing(12);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* title = new QLabel("Recent Projects", recentWidget);
    title->setObjectName("sectionTitle");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    m_clearRecentBtn = new QPushButton("Clear", recentWidget);
    m_clearRecentBtn->setObjectName("linkButton");
    connect(m_clearRecentBtn, &QPushButton::clicked,
            this, &WelcomeScreen::clearRecentProjects);
    headerLayout->addWidget(m_clearRecentBtn);
    layout->addLayout(headerLayout);

    m_recentProjectsList = new QListWidget(recentWidget);
    m_recentProjectsList->setObjectName("recentProjectsList");
    m_recentProjectsList->setAlternatingRowColors(false);
    m_recentProjectsList->setSpacing(2);
    connect(m_recentProjectsList, &QListWidget::itemClicked,
            this, &WelcomeScreen::onRecentProjectClicked);
    connect(m_recentProjectsList, &QListWidget::itemDoubleClicked,
            this, &WelcomeScreen::onRecentProjectDoubleClicked);
    layout->addWidget(m_recentProjectsList, 1);
}

void WelcomeScreen::loadRecentProjects()
{
    m_recentProjectsList->clear();
    const QStringList recent = RecentProjectsManager::instance()->recentProjects();

    for (const QString& path : recent) {
        QFileInfo info(path);
        if (!info.exists()) continue;

        QString displayName;
        if (info.suffix() == "cppatlas") {
            QFile file(path);
            if (file.open(QIODevice::ReadOnly)) {
                displayName = QJsonDocument::fromJson(file.readAll())
                .object()["name"].toString();
                file.close();
            }
            if (displayName.isEmpty()) displayName = info.completeBaseName();
        } else {
            displayName = info.fileName().isEmpty() ? info.absolutePath() : info.fileName();
        }

        QListWidgetItem* item = new QListWidgetItem(m_recentProjectsList);
        item->setText(QString("%1\n%2").arg(displayName).arg(info.absoluteFilePath()));
        item->setData(Qt::UserRole, path);
        item->setToolTip(path);
    }

    if (m_recentProjectsList->count() == 0) {
        QListWidgetItem* item = new QListWidgetItem("No recent projects");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        m_recentProjectsList->addItem(item);
    }
}

void WelcomeScreen::onRecentProjectClicked(QListWidgetItem*) {}

void WelcomeScreen::onRecentProjectDoubleClicked(QListWidgetItem* item)
{
    const QString path = item->data(Qt::UserRole).toString();
    if (!path.isEmpty()) emit recentProjectSelected(path);
}

void WelcomeScreen::clearRecentProjects()
{
    RecentProjectsManager::instance()->clearRecent();
    loadRecentProjects();
}

// FIX: mutual exclusion — Return to Project and Continue without Project
// are never both visible at the same time.
void WelcomeScreen::setReturnToProjectVisible(bool visible)
{
    if (m_returnToProjectBtn)         m_returnToProjectBtn->setVisible(visible);
    if (m_continueWithoutProjectBtn)  m_continueWithoutProjectBtn->setVisible(!visible);
}

void WelcomeScreen::setCurrentUser(const QString& displayName,
                                   const QString& username,
                                   bool isAdmin)
{
    if (!m_userInfoLabel) return;
    m_userInfoLabel->setText(
        QString("\xf0\x9f\x91\xa4  %1  (@%2)%3")
            .arg(displayName, username, isAdmin ? " \xf0\x9f\x91\x91 Admin" : ""));
    m_userInfoLabel->setVisible(true);
}

void WelcomeScreen::applyTheme()
{
    const Theme& theme = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        WelcomeScreen { background-color: %1; }

        #welcomeLogo {
            font-size: 48px;
            font-weight: bold;
            color: %2;
            font-family: "Consolas", monospace;
        }
        #welcomeTitle   { font-size: 32px; font-weight: bold; color: %3; }
        #welcomeSubtitle { font-size: 14px; color: %4; }
        #sectionTitle   { font-size: 16px; font-weight: bold; color: %3; padding-bottom: 8px; }

        #modeButton {
            background-color: %5;
            color: %3;
            border: 2px solid %6;
            border-radius: 8px;
            padding: 15px;
            text-align: left;
            font-size: 13px;
        }
        #modeButton:hover  { background-color: %7; border-color: %2; }
        #modeButton:pressed { background-color: %2; color: white; }

        #quickActionButton {
            background-color: transparent;
            color: %2;
            border: none;
            text-align: left;
            padding: 10px 15px;
            font-size: 14px;
        }
        #quickActionButton:hover { background-color: %5; border-radius: 4px; }

        #recentProjectsList {
            background-color: %5;
            border: 1px solid %6;
            border-radius: 4px;
            color: %3;
        }
        #recentProjectsList::item {
            padding: 10px;
            border-bottom: 1px solid %6;
            color: %3;
        }
        #recentProjectsList::item:hover    { background-color: %2; color: white; border-radius: 4px; }
        #recentProjectsList::item:selected { background-color: %8; color: %3; }

        #linkButton {
            background: transparent;
            color: %2;
            border: none;
            text-decoration: underline;
            font-weight: bold;
        }
        #linkButton:hover { color: %8; }

        #continueButton {
            background-color: %2;
            color: white;
            border: none;
            border-radius: 4px;
            font-weight: bold;
            padding: 8px 20px;
        }
        #continueButton:hover   { background-color: %8; }
        #continueButton:pressed { background-color: %2; }

        #userInfoBar {
            color: %4;
            font-size: 12px;
            padding: 4px 0px;
            border-bottom: 1px solid %6;
            margin-bottom: 4px;
        }

        QLabel { color: %3; }
    )")
                      .arg(theme.windowBackground.name())     // %1
                      .arg(theme.accent.name())               // %2
                      .arg(theme.textPrimary.name())          // %3
                      .arg(theme.textSecondary.name())        // %4
                      .arg(theme.panelBackground.name())      // %5
                      .arg(theme.border.name())               // %6
                      .arg(theme.sidebarBackground.name())    // %7
                      .arg(theme.accent.lighter(115).name())  // %8
                  );
}
