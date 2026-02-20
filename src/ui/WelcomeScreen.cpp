#include "ui/WelcomeScreen.h"
#include "core/RecentProjectsManager.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileInfo>
#include <QFont>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadRecentProjects();
    applyTheme();
    
    // Connect to theme changes
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &WelcomeScreen::applyTheme);
}

void WelcomeScreen::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(60, 40, 60, 40);
    mainLayout->setSpacing(30);
    
    // === Header with logo and title ===
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
    
    // === Mode Selection (IDE vs Quiz) ===
    createModeSelectionArea();
    mainLayout->addWidget(m_modeSelector);
    
    // === Main content: Recent Projects + Quick Actions ===
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(40);
    
    // Left side: Recent Projects
    QWidget* recentWidget = new QWidget(this);
    createRecentProjectsArea(recentWidget);
    contentLayout->addWidget(recentWidget, 2);
    
    // Right side: Quick Actions
    QWidget* actionsWidget = new QWidget(this);
    createQuickActionsArea(actionsWidget);
    contentLayout->addWidget(actionsWidget, 1);
    
    mainLayout->addLayout(contentLayout, 1);
    
    // === Footer ===
    QHBoxLayout* footerLayout = new QHBoxLayout();
    
    m_returnToProjectBtn = new QPushButton("Return to Project", this);
    m_returnToProjectBtn->setObjectName("continueButton");
    m_returnToProjectBtn->setVisible(false);
    connect(m_returnToProjectBtn, &QPushButton::clicked,
            this, &WelcomeScreen::returnToProjectRequested);
    footerLayout->addWidget(m_returnToProjectBtn);
    
    footerLayout->addStretch();
    
    QPushButton* continueBtn = new QPushButton("Continue without project", this);
    continueBtn->setObjectName("continueButton");
    connect(continueBtn, &QPushButton::clicked,
            this, &WelcomeScreen::continueWithoutProjectRequested);
    footerLayout->addWidget(continueBtn);
    
    mainLayout->addLayout(footerLayout);
}

void WelcomeScreen::createModeSelectionArea() {
    m_modeSelector = new QWidget(this);
    m_modeSelector->setObjectName("modeSelector");
    
    QHBoxLayout* layout = new QHBoxLayout(m_modeSelector);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);
    
    // IDE Mode button (large card-style)
    m_ideModeBtn = new QPushButton(this);
    m_ideModeBtn->setObjectName("modeButton");
    m_ideModeBtn->setText("IDE Mode\nC++ Development Environment");
    m_ideModeBtn->setCheckable(true);
    m_ideModeBtn->setChecked(true);
    m_ideModeBtn->setMinimumSize(250, 80);
    
    // Quiz Mode button (large card-style)
    m_quizModeBtn = new QPushButton(this);
    m_quizModeBtn->setObjectName("modeButton");
    m_quizModeBtn->setText("Quiz Mode\nC++ Learning & Assessment");
    m_quizModeBtn->setCheckable(true);
    m_quizModeBtn->setMinimumSize(250, 80);
    connect(m_quizModeBtn, &QPushButton::clicked,
            this, &WelcomeScreen::quizModeRequested);
    
    // Make them exclusive
    connect(m_ideModeBtn, &QPushButton::toggled, this, [this](bool checked) {
        if (checked) m_quizModeBtn->setChecked(false);
    });
    connect(m_quizModeBtn, &QPushButton::toggled, this, [this](bool checked) {
        if (checked) m_ideModeBtn->setChecked(false);
    });
    
    layout->addStretch();
    layout->addWidget(m_ideModeBtn);
    layout->addWidget(m_quizModeBtn);
    layout->addStretch();
}

void WelcomeScreen::createQuickActionsArea(QWidget* actionsWidget) {
    actionsWidget->setObjectName("quickActionsPanel");
    
    QVBoxLayout* layout = new QVBoxLayout(actionsWidget);
    layout->setSpacing(12);
    
    QLabel* title = new QLabel("Get Started", actionsWidget);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);
    
    // New File button
    m_newFileBtn = new QPushButton("New File", actionsWidget);
    m_newFileBtn->setObjectName("quickActionButton");
    m_newFileBtn->setMinimumHeight(40);
    connect(m_newFileBtn, &QPushButton::clicked,
            this, &WelcomeScreen::newFileRequested);
    layout->addWidget(m_newFileBtn);
    
    // Open File button
    m_openFileBtn = new QPushButton("Open File...", actionsWidget);
    m_openFileBtn->setObjectName("quickActionButton");
    m_openFileBtn->setMinimumHeight(40);
    connect(m_openFileBtn, &QPushButton::clicked,
            this, &WelcomeScreen::openFileRequested);
    layout->addWidget(m_openFileBtn);
    
    // Open Folder button
    m_openFolderBtn = new QPushButton("Open Folder...", actionsWidget);
    m_openFolderBtn->setObjectName("quickActionButton");
    m_openFolderBtn->setMinimumHeight(40);
    connect(m_openFolderBtn, &QPushButton::clicked,
            this, &WelcomeScreen::openFolderRequested);
    layout->addWidget(m_openFolderBtn);
    
    // Create Project button
    m_createProjectBtn = new QPushButton("Create Project...", actionsWidget);
    m_createProjectBtn->setObjectName("quickActionButton");
    m_createProjectBtn->setMinimumHeight(40);
    connect(m_createProjectBtn, &QPushButton::clicked,
            this, &WelcomeScreen::createProjectRequested);
    layout->addWidget(m_createProjectBtn);
    
    // Open Project button
    m_openProjectBtn = new QPushButton("Open Project...", actionsWidget);
    m_openProjectBtn->setObjectName("quickActionButton");
    m_openProjectBtn->setMinimumHeight(40);
    connect(m_openProjectBtn, &QPushButton::clicked,
            this, &WelcomeScreen::openProjectRequested);
    layout->addWidget(m_openProjectBtn);
    
    layout->addStretch();
}

void WelcomeScreen::createRecentProjectsArea(QWidget* recentWidget) {
    recentWidget->setObjectName("recentProjectsPanel");
    
    QVBoxLayout* layout = new QVBoxLayout(recentWidget);
    layout->setSpacing(12);
    
    // Header with title and clear button
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
    
    // Projects list
    m_recentProjectsList = new QListWidget(recentWidget);
    m_recentProjectsList->setObjectName("recentProjectsList");
    m_recentProjectsList->setAlternatingRowColors(true);
    m_recentProjectsList->setSpacing(2);
    
    connect(m_recentProjectsList, &QListWidget::itemClicked,
            this, &WelcomeScreen::onRecentProjectClicked);
    connect(m_recentProjectsList, &QListWidget::itemDoubleClicked,
            this, &WelcomeScreen::onRecentProjectDoubleClicked);
    
    layout->addWidget(m_recentProjectsList, 1);
}

void WelcomeScreen::loadRecentProjects() {
    m_recentProjectsList->clear();
    
    QStringList recentProjects = RecentProjectsManager::instance()->recentProjects();
    
    for (const QString& projectPath : recentProjects) {
        QFileInfo info(projectPath);
        if (!info.exists()) continue;
        
        QListWidgetItem* item = new QListWidgetItem(m_recentProjectsList);
        
        QString displayName;
        
        // Parse .cppatlas to get project name
        if (info.suffix() == "cppatlas") {
            QFile file(projectPath);
            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                QString name = doc.object()["name"].toString();
                if (!name.isEmpty()) {
                    displayName = name;
                } else {
                    displayName = info.completeBaseName();
                }
                file.close();
            } else {
                displayName = info.completeBaseName();
            }
        } else {
            displayName = info.fileName().isEmpty() ? info.absolutePath() : info.fileName();
        }
        
        QString displayText = QString("%1\n%2")
            .arg(displayName)
            .arg(info.absoluteFilePath());
        
        item->setText(displayText);
        item->setData(Qt::UserRole, projectPath);
        item->setToolTip(projectPath);
    }
    
    // Show placeholder if empty
    if (m_recentProjectsList->count() == 0) {
        QListWidgetItem* item = new QListWidgetItem("No recent projects");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        m_recentProjectsList->addItem(item);
    }
}

void WelcomeScreen::onRecentProjectClicked(QListWidgetItem* item) {
    // Reserved for future single-click behavior (e.g., preview or details)
    Q_UNUSED(item);
}

void WelcomeScreen::onRecentProjectDoubleClicked(QListWidgetItem* item) {
    QString path = item->data(Qt::UserRole).toString();
    if (!path.isEmpty()) {
        emit recentProjectSelected(path);
    }
}

void WelcomeScreen::clearRecentProjects() {
    RecentProjectsManager::instance()->clearRecent();
    loadRecentProjects();
}

void WelcomeScreen::setReturnToProjectVisible(bool visible) {
    m_returnToProjectBtn->setVisible(visible);
}

void WelcomeScreen::applyTheme() {
    Theme theme = ThemeManager::instance()->currentTheme();
    
    setStyleSheet(QString(R"(
        WelcomeScreen {
            background-color: %1;
        }
        
        #welcomeLogo {
            font-size: 48px;
            font-weight: bold;
            color: %2;
            font-family: "Consolas", monospace;
        }
        
        #welcomeTitle {
            font-size: 32px;
            font-weight: bold;
            color: %3;
        }
        
        #welcomeSubtitle {
            font-size: 14px;
            color: %4;
        }
        
        #sectionTitle {
            font-size: 16px;
            font-weight: bold;
            color: %3;
            padding-bottom: 8px;
        }
        
        #modeButton {
            background-color: %5;
            color: %3;
            border: 2px solid %6;
            border-radius: 8px;
            padding: 15px;
            text-align: left;
            font-size: 13px;
        }
        
        #modeButton:checked {
            border-color: %2;
            background-color: %7;
        }
        
        #modeButton:hover {
            background-color: %7;
        }
        
        #quickActionButton {
            background-color: transparent;
            color: %2;
            border: none;
            text-align: left;
            padding: 10px 15px;
            font-size: 14px;
        }
        
        #quickActionButton:hover {
            background-color: %5;
            border-radius: 4px;
        }
        
        #recentProjectsList {
            background-color: %5;
            border: 1px solid %6;
            border-radius: 4px;
            color: %3;
        }
        
        #recentProjectsList::item {
            padding: 10px;
            border-bottom: 1px solid %6;
        }
        
        #recentProjectsList::item:selected {
            background-color: %2;
        }
        
        #recentProjectsList::item:hover:!selected {
            background-color: %7;
        }
        
        #linkButton {
            background: transparent;
            color: %2;
            border: none;
            text-decoration: underline;
        }
        
        #continueButton {
            background-color: transparent;
            color: %4;
            border: 1px solid %6;
            padding: 8px 20px;
            border-radius: 4px;
        }
        
        #continueButton:hover {
            background-color: %5;
        }
    )").arg(theme.windowBackground.name())     // %1
       .arg(theme.accent.name())               // %2
       .arg(theme.textPrimary.name())          // %3
       .arg(theme.textSecondary.name())        // %4
       .arg(theme.panelBackground.name())      // %5
       .arg(theme.border.name())               // %6
       .arg(theme.sidebarBackground.name()));  // %7
}
