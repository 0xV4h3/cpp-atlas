#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor/EditorTabWidget.h"
#include "editor/CodeEditor.h"
#include "output/OutputPanel.h"
#include "output/TerminalWidget.h"
#include "output/ProblemsWidget.h"
#include "ui/FileTreeWidget.h"
#include "ui/ThemeManager.h"
#include "ui/GotoLineDialog.h"
#include "ui/FindReplaceDialog.h"
#include "ui/WelcomeScreen.h"
#include "ui/NewFileDialog.h"
#include "ui/NewProjectDialog.h"
#include "ui/AnalysisPanel.h"
#include "core/FileManager.h"
#include "core/Project.h"
#include "core/ProjectManager.h"
#include "core/RecentProjectsManager.h"
#include "compiler/CompilerRegistry.h"
#include "compiler/ICompiler.h"

#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QDir>
#include <QActionGroup>
#include <QInputDialog>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Enable frameless window with custom styling
#ifdef Q_OS_WIN
    // On Windows, use native frameless with resize support
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // Enable Windows Aero Snap, resize from edges
    setAttribute(Qt::WA_NativeWindow);
#else
    // On Linux/macOS
    setWindowFlags(Qt::FramelessWindowHint);
#endif
    
    setAttribute(Qt::WA_TranslucentBackground, false);
    
    // Initialize components
    m_fileManager = new FileManager(this);
    m_project = new Project(this);
    
    setupUi();
    setupMenus();
    setupCustomTitleBar();
    setupToolbar();
    setupDockWidgets();
    setupStatusBar();
    setupConnections();
    setupWelcomeScreen();
    
    // Auto-scan for compilers
    CompilerRegistry::instance().autoScanCompilers();
    loadCompilers();
    
    // Apply default theme
    ThemeManager::instance()->setTheme("dark");
    
    updateWindowTitle();
    
    // Restore window state
    QSettings settings("CppAtlas", "CppAtlas");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    
    // Show Welcome Screen on startup
    showWelcomeScreen();
}

MainWindow::~MainWindow()
{
    // Save window state
    QSettings settings("CppAtlas", "CppAtlas");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    
    delete ui;
}

void MainWindow::setupUi() {
    setWindowTitle("CppAtlas - C++ Learning IDE");
    resize(1200, 800);
    
    // Create stacked widget for Welcome/IDE switching
    m_centralStack = new QStackedWidget(this);
    
    // Create central widget - editor tabs
    m_editorTabs = new EditorTabWidget(this);
    m_centralStack->addWidget(m_editorTabs);
    
    setCentralWidget(m_centralStack);
}

void MainWindow::setupCustomTitleBar() {
    m_titleBar = new QWidget(this);
    m_titleBar->setObjectName("customTitleBar");
    m_titleBar->setFixedHeight(TITLE_BAR_HEIGHT);
    m_titleBar->setMouseTracking(true);
    
    QHBoxLayout* layout = new QHBoxLayout(m_titleBar);
    layout->setContentsMargins(8, 0, 0, 0);
    layout->setSpacing(0);
    
    // App icon (C++ icon)
    m_iconLabel = new QLabel(this);
    m_iconLabel->setText("C++");
    m_iconLabel->setObjectName("appIcon");
    m_iconLabel->setFixedSize(36, 28);  // Wider to fit "C++"
    m_iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_iconLabel);
    
    layout->addSpacing(8);
    
    // Menu bar goes here (move existing menu bar into title bar)
    if (menuBar()) {
        menuBar()->setObjectName("titleBarMenu");
        layout->addWidget(menuBar());
    }
    
    layout->addStretch();
    
    // Window title (filename - CppAtlas)
    m_titleLabel = new QLabel("CppAtlas - C++ Learning IDE", this);
    m_titleLabel->setObjectName("windowTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);
    
    layout->addStretch();
    
    // Window control buttons (right side)
    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* btnLayout = new QHBoxLayout(buttonContainer);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(0);
    
    // Minimize button
    m_minimizeBtn = new QPushButton(this);
    m_minimizeBtn->setObjectName("minimizeButton");
    m_minimizeBtn->setText("─");  // Minimize symbol
    m_minimizeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_minimizeBtn->setFlat(true);
    connect(m_minimizeBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    btnLayout->addWidget(m_minimizeBtn);
    
    // Maximize/Restore button
    m_maximizeBtn = new QPushButton(this);
    m_maximizeBtn->setObjectName("maximizeButton");
    m_maximizeBtn->setText("□");  // Maximize symbol
    m_maximizeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_maximizeBtn->setFlat(true);
    connect(m_maximizeBtn, &QPushButton::clicked, this, [this]() {
        if (isMaximized()) {
            showNormal();
            m_maximizeBtn->setText("□");
        } else {
            showMaximized();
            m_maximizeBtn->setText("❐");  // Restore icon
        }
    });
    btnLayout->addWidget(m_maximizeBtn);
    
    // Close button
    m_closeBtn = new QPushButton(this);
    m_closeBtn->setObjectName("closeButton");
    m_closeBtn->setText("✕");
    m_closeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_closeBtn->setFlat(true);
    connect(m_closeBtn, &QPushButton::clicked, this, &QMainWindow::close);
    btnLayout->addWidget(m_closeBtn);
    
    layout->addWidget(buttonContainer);
    
    // Set title bar as menu widget (replaces default)
    setMenuWidget(m_titleBar);
}

void MainWindow::setupMenus() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    QAction* newAction = fileMenu->addAction("New &File");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onFileNew);
    
    QAction* newProjectAction = fileMenu->addAction("New &Project...");
    newProjectAction->setShortcut(QKeySequence("Ctrl+Shift+N"));
    connect(newProjectAction, &QAction::triggered, this, &MainWindow::onFileNewProject);
    
    fileMenu->addSeparator();
    
    QAction* openAction = fileMenu->addAction("&Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);
    
    fileMenu->addSeparator();
    
    QAction* saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onFileSave);
    
    QAction* saveAsAction = fileMenu->addAction("Save &As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    
    fileMenu->addSeparator();
    
    QAction* openFolderAction = fileMenu->addAction("Open F&older...");
    openFolderAction->setShortcut(QKeySequence("Ctrl+K"));
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::onFileOpenFolder);
    
    QAction* openProjectAction = fileMenu->addAction("Open Pro&ject...");
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::onFileOpenProject);

    fileMenu->addSeparator();
    m_closeProjectAction = fileMenu->addAction(QStringLiteral("Close Project"));
    m_closeProjectAction->setEnabled(false);
    connect(m_closeProjectAction, &QAction::triggered, this, &MainWindow::onFileCloseProject);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onFileExit);
    
    // Edit menu
    m_editMenu = menuBar()->addMenu("&Edit");
    
    QAction* undoAction = m_editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onEditUndo);
    
    QAction* redoAction = m_editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onEditRedo);
    
    m_editMenu->addSeparator();
    
    QAction* cutAction = m_editMenu->addAction("Cu&t");
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, this, &MainWindow::onEditCut);
    
    QAction* copyAction = m_editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onEditCopy);
    
    QAction* pasteAction = m_editMenu->addAction("&Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onEditPaste);
    
    m_editMenu->addSeparator();
    
    m_findAction = m_editMenu->addAction("&Find...");
    m_findAction->setShortcut(QKeySequence::Find);
    connect(m_findAction, &QAction::triggered, this, &MainWindow::onEditFind);
    
    m_replaceAction = m_editMenu->addAction("&Replace...");
    m_replaceAction->setShortcut(QKeySequence::Replace);
    connect(m_replaceAction, &QAction::triggered, this, &MainWindow::onEditReplace);
    
    m_editMenu->addSeparator();
    
    m_gotoLineAction = m_editMenu->addAction("&Go to Line...");
    m_gotoLineAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    connect(m_gotoLineAction, &QAction::triggered, this, &MainWindow::onEditGotoLine);
    
    // Build menu
    m_buildMenu = menuBar()->addMenu("&Build");
    
    QAction* compileAction = m_buildMenu->addAction("&Build");
    compileAction->setShortcut(Qt::Key_F7);
    connect(compileAction, &QAction::triggered, this, &MainWindow::onBuildCompile);
    
    m_runAction = m_buildMenu->addAction("&Run");
    m_runAction->setShortcut(Qt::Key_F5);
    connect(m_runAction, &QAction::triggered, this, &MainWindow::onBuildRun);
    
    QAction* compileAndRunAction = m_buildMenu->addAction("Build && R&un");
    compileAndRunAction->setShortcut(Qt::CTRL | Qt::Key_F5);
    connect(compileAndRunAction, &QAction::triggered, this, &MainWindow::onBuildCompileAndRun);
    
    m_buildMenu->addSeparator();
    
    QAction* stopAction = m_buildMenu->addAction("&Stop");
    stopAction->setShortcut(Qt::SHIFT | Qt::Key_F5);
    connect(stopAction, &QAction::triggered, this, &MainWindow::onBuildStop);
    
    QAction* cleanAction = m_buildMenu->addAction("&Clean");
    connect(cleanAction, &QAction::triggered, this, &MainWindow::onBuildClean);
    
    // View menu
    m_viewMenu = menuBar()->addMenu("&View");
    
    m_toggleFileTreeAction = m_viewMenu->addAction("Toggle &File Tree");
    connect(m_toggleFileTreeAction, &QAction::triggered, this, &MainWindow::onViewToggleFileTree);
    
    m_toggleOutputAction = m_viewMenu->addAction("Toggle &Output Panel");
    connect(m_toggleOutputAction, &QAction::triggered, this, &MainWindow::onViewToggleOutputPanel);
    
    m_viewMenu->addSeparator();
    
    // Theme submenu
    QMenu* themeMenu = m_viewMenu->addMenu("&Theme");
    QActionGroup* themeGroup = new QActionGroup(this);
    
    QAction* darkThemeAction = themeMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(true);
    darkThemeAction->setData("dark");
    themeGroup->addAction(darkThemeAction);
    
    QAction* lightThemeAction = themeMenu->addAction("Light");
    lightThemeAction->setCheckable(true);
    lightThemeAction->setData("light");
    themeGroup->addAction(lightThemeAction);
    
    QAction* draculaThemeAction = themeMenu->addAction("Dracula");
    draculaThemeAction->setCheckable(true);
    draculaThemeAction->setData("dracula");
    themeGroup->addAction(draculaThemeAction);
    
    QAction* monokaiThemeAction = themeMenu->addAction("Monokai");
    monokaiThemeAction->setCheckable(true);
    monokaiThemeAction->setData("monokai");
    themeGroup->addAction(monokaiThemeAction);

    QAction* oneMonokaiThemeAction = themeMenu->addAction("One Monokai");
    oneMonokaiThemeAction->setCheckable(true);
    oneMonokaiThemeAction->setData("one-monokai");
    themeGroup->addAction(oneMonokaiThemeAction);
    
    connect(themeGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString themeName = action->data().toString();
        ThemeManager::instance()->setTheme(themeName);
        // Update all open editors
        for (int i = 0; i < m_editorTabs->count(); ++i) {
            CodeEditor* editor = m_editorTabs->editorAt(i);
            if (editor) {
                editor->applyTheme(themeName);
            }
        }
    });
    
    m_viewMenu->addSeparator();
    
    QAction* fullscreenAction = m_viewMenu->addAction("&Fullscreen");
    fullscreenAction->setShortcut(Qt::Key_F11);
    connect(fullscreenAction, &QAction::triggered, this, &MainWindow::onViewFullscreen);
    
    m_viewMenu->addSeparator();
    
    QAction* showWelcomeAction = m_viewMenu->addAction("Show &Welcome Screen");
    showWelcomeAction->setShortcut(QKeySequence("Ctrl+Shift+W"));
    connect(showWelcomeAction, &QAction::triggered, this, &MainWindow::showWelcomeScreen);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About CppAtlas",
            "CppAtlas - C++ Learning IDE\n\n"
            "Version 0.1\n\n"
            "An educational Qt-based environment for learning C++.");
    });

    // Tools menu — Analysis Panel + tab shortcuts
    m_toolsMenu = menuBar()->addMenu(QStringLiteral("&Tools"));

    m_toggleAnalysisAction = m_toolsMenu->addAction(
        QStringLiteral("Toggle &Analysis Panel"));
    m_toggleAnalysisAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_A));
    connect(m_toggleAnalysisAction, &QAction::triggered, this, [this]() {
        const bool visible = !m_analysisDock->isVisible();
        m_analysisDock->setVisible(visible);
        if (visible) m_analysisDock->raise();
    });

    m_toolsMenu->addSeparator();

    QAction* showInsightsAction = m_toolsMenu->addAction(
        QStringLiteral("C++ &Insights"));
    showInsightsAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    connect(showInsightsAction, &QAction::triggered, this, [this]() {
        m_analysisDock->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabInsights);
        m_analysisDock->raise();
    });

    QAction* showAssemblyAction = m_toolsMenu->addAction(
        QStringLiteral("&Assembly View"));
    showAssemblyAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(showAssemblyAction, &QAction::triggered, this, [this]() {
        m_analysisDock->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabAssembly);
        m_analysisDock->raise();
    });

    QAction* showBenchmarkAction = m_toolsMenu->addAction(
        QStringLiteral("&Benchmark"));
    showBenchmarkAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    connect(showBenchmarkAction, &QAction::triggered, this, [this]() {
        m_analysisDock->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabBenchmark);
        m_analysisDock->raise();
    });
}

void MainWindow::setupToolbar() {
    m_mainToolbar = addToolBar("Main Toolbar");
    m_mainToolbar->setMovable(false);
    
    // New button with dropdown menu
    QToolButton* newButton = new QToolButton(this);
    newButton->setText("New");
    newButton->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu* newMenu = new QMenu(newButton);
    QAction* newFileAction = newMenu->addAction("New File...");
    connect(newFileAction, &QAction::triggered, this, &MainWindow::onFileNew);
    QAction* newFolderAction = newMenu->addAction("New Folder...");
    connect(newFolderAction, &QAction::triggered, this, [this]() {
        bool ok;
        QString folderName = QInputDialog::getText(this, "New Folder",
            "Folder name:", QLineEdit::Normal, "NewFolder", &ok);
        if (ok && !folderName.isEmpty()) {
            QString basePath = m_fileTree->rootPath();
            if (basePath.isEmpty()) {
                basePath = QDir::currentPath();
            }
            QDir(basePath).mkdir(folderName);
        }
    });
    QAction* newProjectAction = newMenu->addAction("New Project...");
    connect(newProjectAction, &QAction::triggered, this, &MainWindow::onFileNewProject);
    newButton->setMenu(newMenu);
    connect(newButton, &QToolButton::clicked, this, &MainWindow::onFileNew);
    m_mainToolbar->addWidget(newButton);
    
    m_mainToolbar->addAction("Open", this, &MainWindow::onFileOpen);
    m_mainToolbar->addAction("Save", this, &MainWindow::onFileSave);
    
    m_mainToolbar->addSeparator();
    
    // Build, Run, Stop buttons
    m_mainToolbar->addAction("Build", this, &MainWindow::onBuildCompile);
    m_mainToolbar->addAction("Run", this, &MainWindow::onBuildRun);
    m_mainToolbar->addAction("Stop", this, &MainWindow::onBuildStop);
    
    m_mainToolbar->addSeparator();
    
    // Compiler selection
    m_mainToolbar->addWidget(new QLabel(" Compiler: "));
    m_compilerCombo = new QComboBox(m_mainToolbar);
    m_compilerCombo->setMinimumWidth(150);
    m_mainToolbar->addWidget(m_compilerCombo);
    
    // Standard selection
    m_mainToolbar->addWidget(new QLabel(" Standard: "));
    m_standardCombo = new QComboBox(m_mainToolbar);
    m_standardCombo->addItems({"c++11", "c++14", "c++17", "c++20", "c++23"});
    m_standardCombo->setCurrentText("c++17");
    m_mainToolbar->addWidget(m_standardCombo);
    
    connect(m_compilerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCompilerChanged);
    connect(m_standardCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStandardChanged);
}

void MainWindow::setupDockWidgets() {
    // File tree dock
    m_fileTreeDock = new QDockWidget("File Tree", this);
    m_fileTree = new FileTreeWidget(m_fileTreeDock);
    m_fileTreeDock->setWidget(m_fileTree);
    addDockWidget(Qt::LeftDockWidgetArea, m_fileTreeDock);
    
    // Output panel dock
    m_outputPanelDock = new QDockWidget("Output", this);
    m_outputPanel = new OutputPanel(m_outputPanelDock);
    m_outputPanelDock->setWidget(m_outputPanel);
    addDockWidget(Qt::BottomDockWidgetArea, m_outputPanelDock);

    // Analysis dock (right side — Insights | Assembly | Benchmark tabs)
    m_analysisDock  = new QDockWidget(QStringLiteral("Analysis"), this);
    m_analysisPanel = new AnalysisPanel(m_analysisDock);
    m_analysisDock->setWidget(m_analysisPanel);
    addDockWidget(Qt::RightDockWidgetArea, m_analysisDock);
    m_analysisDock->hide(); // Hidden by default; open via Tools menu
}

void MainWindow::setupStatusBar() {
    m_statusLabel = new QLabel("Ready");
    m_cursorPosLabel = new QLabel("Ln 1, Col 1");
    m_standardLabel = new QLabel("C++17");
    m_compilerLabel = new QLabel("No compiler");
    
    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_cursorPosLabel);
    statusBar()->addPermanentWidget(m_standardLabel);
    statusBar()->addPermanentWidget(m_compilerLabel);
}

void MainWindow::setupConnections() {
    // Editor signals
    connect(m_editorTabs, &EditorTabWidget::editorChanged,
            this, &MainWindow::onEditorChanged);
    
    // File tree signals
    connect(m_fileTree, &FileTreeWidget::fileDoubleClicked,
            this, &MainWindow::onFileTreeDoubleClicked);
    connect(m_fileTree, &FileTreeWidget::newFileRequested,
            this, &MainWindow::onNewFileRequested);
    connect(m_fileTree, &FileTreeWidget::fileCreated,
            this, [this](const QString& filePath) {
        m_editorTabs->openFile(filePath);
    });
    connect(m_fileTree, &FileTreeWidget::fileDeleted,
            this, [this](const QString& filePath) {
        m_editorTabs->closeFileTab(filePath);
    });
    connect(m_fileTree, &FileTreeWidget::fileRenamed,
            this, [this](const QString& oldPath, const QString& newPath) {
        m_editorTabs->updateFilePath(oldPath, newPath);
    });
    
    // Problems widget signals
    connect(m_outputPanel->problems(), &ProblemsWidget::diagnosticClicked,
            this, &MainWindow::onDiagnosticClicked);

    // Keep close project action in sync with project state
    connect(ProjectManager::instance(), &ProjectManager::projectClosed,
            this, [this]() {
        if (m_closeProjectAction) m_closeProjectAction->setEnabled(false);
    });

    // Forward assembly line activation to editor navigation
    connect(m_analysisPanel, &AnalysisPanel::sourceLineActivated,
            this, [this](int line) {
        CodeEditor* ed = m_editorTabs->currentEditor();
        if (ed) ed->gotoLine(line);
    });

    // Keep AnalysisPanel in sync with MainWindow toolbar compiler/standard selections
    connect(m_compilerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
        m_analysisPanel->setCompilerId(
            m_compilerCombo->currentData().toString());
    });
    connect(m_standardCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
        m_analysisPanel->setStandard(m_standardCombo->currentText());
    });
}

void MainWindow::setupWelcomeScreen() {
    m_welcomeScreen = new WelcomeScreen(this);
    m_centralStack->addWidget(m_welcomeScreen);
    
    // Connect Welcome Screen signals
    connect(m_welcomeScreen, &WelcomeScreen::newFileRequested, this, [this]() {
        hideWelcomeScreen();
        onFileNew();
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::openFileRequested, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Open File", QString(),
            "C++ Files (*.cpp *.h *.hpp *.cc *.cxx);;All Files (*)");
        if (!file.isEmpty()) {
            hideWelcomeScreen();
            m_editorTabs->openFile(file);
            RecentProjectsManager::instance()->addRecentProject(file);
        }
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::openFolderRequested, this, [this]() {
        QString folder = QFileDialog::getExistingDirectory(this, "Open Folder");
        if (!folder.isEmpty()) {
            hideWelcomeScreen();
            m_fileTree->openFolder(folder);
            RecentProjectsManager::instance()->addRecentProject(folder);
            setWindowTitle(QString("%1 - CppAtlas").arg(QFileInfo(folder).fileName()));
        }
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::createProjectRequested, this, [this]() {
        onFileNewProject();
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::openProjectRequested, this, [this]() {
        onFileOpenProject();
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::recentProjectSelected,
            this, [this](const QString& path) {
        QFileInfo info(path);
        if (info.suffix() == "cppatlas") {
            // Open as a project
            auto result = ProjectManager::instance()->openProject(path);
            if (result == Project::LoadResult::Success) {
                auto project = ProjectManager::instance()->currentProject();
                m_fileTree->openFolder(project->projectDirectory());
                restoreProjectSession(project);
                
                hideWelcomeScreen();
                m_statusLabel->setText("Project: " + project->name());
                m_welcomeScreen->setReturnToProjectVisible(true);
            } else {
                showProjectLoadError(result);
            }
        } else {
            hideWelcomeScreen();
            if (info.isDir()) {
                m_fileTree->openFolder(path);
                setWindowTitle(QString("%1 - CppAtlas").arg(info.fileName()));
            } else {
                m_editorTabs->openFile(path);
            }
            RecentProjectsManager::instance()->addRecentProject(path);
        }
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::quizModeRequested, this, [this]() {
        QMessageBox::information(this, "Quiz Mode",
            "Quiz Mode will be available in a future update.\n\n"
            "This will include:\n"
            "- C++ knowledge assessments\n"
            "- Interactive coding challenges\n"
            "- Progress tracking");
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::continueWithoutProjectRequested,
            this, [this]() {
        hideWelcomeScreen();
        m_editorTabs->newFile();
    });
    
    connect(m_welcomeScreen, &WelcomeScreen::returnToProjectRequested,
            this, [this]() {
        hideWelcomeScreen();
    });
}

void MainWindow::showWelcomeScreen() {
    // Save current project session before showing welcome
    if (ProjectManager::instance()->hasOpenProject()) {
        saveCurrentSession();
    }
    
    m_centralStack->setCurrentWidget(m_welcomeScreen);
    
    // Hide IDE-specific docks
    m_fileTreeDock->hide();
    m_outputPanelDock->hide();
    m_analysisDockWasVisible = m_analysisDock->isVisible();
    m_analysisDock->hide();
    
    // Show "Return to Project" button if a project/folder is open
    bool hasOpenProject = ProjectManager::instance()->hasOpenProject();
    m_welcomeScreen->setReturnToProjectVisible(hasOpenProject);
    
    updateMenuState(true);
}

void MainWindow::hideWelcomeScreen() {
    m_centralStack->setCurrentWidget(m_editorTabs);
    
    // Show IDE docks
    m_fileTreeDock->show();
    m_outputPanelDock->show();
    if (m_analysisDockWasVisible) m_analysisDock->show();
    
    if (m_closeProjectAction) m_closeProjectAction->setEnabled(true);
    updateMenuState(false);
}

void MainWindow::updateMenuState(bool isWelcomeVisible) {
    // Build menu - disable entirely in welcome screen
    if (m_buildMenu) {
        m_buildMenu->setEnabled(!isWelcomeVisible);
    }
    if (m_runAction) {
        m_runAction->setEnabled(!isWelcomeVisible);
    }
    
    // Edit menu - disable Find, Replace, Go to Line in welcome screen
    if (m_findAction) {
        m_findAction->setEnabled(!isWelcomeVisible);
    }
    if (m_replaceAction) {
        m_replaceAction->setEnabled(!isWelcomeVisible);
    }
    if (m_gotoLineAction) {
        m_gotoLineAction->setEnabled(!isWelcomeVisible);
    }
    
    // View menu - disable Toggle File Tree and Toggle Output Panel
    if (m_toggleFileTreeAction) {
        m_toggleFileTreeAction->setEnabled(!isWelcomeVisible);
    }
    if (m_toggleOutputAction) {
        m_toggleOutputAction->setEnabled(!isWelcomeVisible);
    }
    
    // Tools menu - disable entirely in welcome screen
    if (m_toolsMenu) {
        m_toolsMenu->setEnabled(!isWelcomeVisible);
    }
    
    // Main toolbar - hide/show
    if (m_mainToolbar) {
        m_mainToolbar->setVisible(!isWelcomeVisible);
    }
}

void MainWindow::loadCompilers() {
    m_compilerCombo->clear();
    
    auto compilers = CompilerRegistry::instance().getAvailableCompilers();
    
    if (compilers.isEmpty()) {
        m_compilerCombo->addItem("No compilers found");
        m_compilerCombo->setEnabled(false);
        m_compilerLabel->setText("No compiler");
        return;
    }
    
    for (const auto& compiler : compilers) {
        m_compilerCombo->addItem(compiler->name(), compiler->id());
    }
    
    // Select default compiler
    QString defaultId = CompilerRegistry::instance().defaultCompilerId();
    int index = m_compilerCombo->findData(defaultId);
    if (index >= 0) {
        m_compilerCombo->setCurrentIndex(index);
    }

    // Sync initial compiler/standard to AnalysisPanel after registry is populated
    if (m_analysisPanel) {
        m_analysisPanel->setCompilerId(
            m_compilerCombo->currentData().toString());
        m_analysisPanel->setStandard(m_standardCombo->currentText());
    }
}

void MainWindow::updateStatusBar() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        int line, col;
        editor->getCursorPosition(&line, &col);
        m_cursorPosLabel->setText(QString("Ln %1, Col %2").arg(line + 1).arg(col + 1));
    }
    
    m_standardLabel->setText(m_standardCombo->currentText().toUpper());
    
    QString compilerId = m_compilerCombo->currentData().toString();
    auto compiler = CompilerRegistry::instance().getCompiler(compilerId);
    if (compiler) {
        m_compilerLabel->setText(compiler->name());
    }
}

void MainWindow::updateWindowTitle() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    QString title = "CppAtlas - C++ Learning IDE";
    
    if (editor && !editor->filePath().isEmpty()) {
        QFileInfo info(editor->filePath());
        title = QString("%1 - %2").arg(info.fileName()).arg(title);
        
        if (editor->isModified()) {
            title = "* " + title;
        }
    }
    
    setWindowTitle(title);
    updateCustomTitleLabel(title);
}

void MainWindow::updateCustomTitleLabel(const QString& title) {
    if (m_titleLabel) {
        m_titleLabel->setText(title);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save project session before closing
    saveCurrentSession();
    
    if (m_editorTabs->closeAll()) {
        ProjectManager::instance()->closeCurrentProject();
        event->accept();
    } else {
        event->ignore();
    }
}

// File menu slots
void MainWindow::onFileNew() {
    QString defaultDir = m_fileTree->rootPath();
    if (defaultDir.isEmpty()) {
        defaultDir = QDir::homePath();
    }
    
    NewFileDialog dialog(defaultDir, this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.createdFiles();
        for (const QString& file : files) {
            m_editorTabs->openFile(file);
        }
    }
}

void MainWindow::onFileNewProject() {
    NewProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ProjectSettings settings;
        settings.compilerId = dialog.selectedCompilerId();
        settings.standard = dialog.selectedStandard();
        settings.createMainCpp = dialog.createMainCpp();
        settings.createCMakeLists = dialog.createCMakeLists();
        settings.createProjectFolder = dialog.createProjectFolder();
        
        if (ProjectManager::instance()->createProject(
                dialog.projectName(), dialog.projectLocation(), settings)) {
            auto project = ProjectManager::instance()->currentProject();
            m_fileTree->openFolder(project->projectDirectory());
            
            // Open main.cpp if created
            if (settings.createMainCpp) {
                QString mainPath = project->projectDirectory() + QDir::separator() + "src" + QDir::separator() + "main.cpp";
                m_editorTabs->openFile(mainPath);
            }
            
            hideWelcomeScreen();
            m_statusLabel->setText("Project: " + project->name());
            m_welcomeScreen->setReturnToProjectVisible(true);
        } else {
            QMessageBox::warning(this, "Error", "Failed to create project.");
        }
    }
}

void MainWindow::onFileOpen() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open File",
        QString(),
        "C++ Files (*.cpp *.h *.hpp *.cc *.cxx);;All Files (*)"
    );
    
    if (!filePath.isEmpty()) {
        m_editorTabs->openFile(filePath);
        m_fileManager->addRecentFile(filePath);
        RecentProjectsManager::instance()->addRecentProject(filePath);
    }
}

void MainWindow::onFileSave() {
    m_editorTabs->saveCurrentFile();
    updateWindowTitle();
}

void MainWindow::onFileSaveAs() {
    m_editorTabs->saveCurrentFileAs();
    updateWindowTitle();
}

void MainWindow::onFileOpenFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        "Open Folder",
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!folderPath.isEmpty()) {
        m_fileTree->openFolder(folderPath);
        m_statusLabel->setText("Opened folder: " + folderPath);
        RecentProjectsManager::instance()->addRecentProject(folderPath);
    }
}

void MainWindow::onFileOpenProject() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Project",
        QString(),
        "CppAtlas Project (*.cppatlas);;All Files (*)"
    );
    
    if (!filePath.isEmpty()) {
        auto result = ProjectManager::instance()->openProject(filePath);
        if (result == Project::LoadResult::Success) {
            auto project = ProjectManager::instance()->currentProject();
            m_fileTree->openFolder(project->projectDirectory());
            restoreProjectSession(project);
            
            hideWelcomeScreen();
            m_statusLabel->setText("Project: " + project->name());
            m_welcomeScreen->setReturnToProjectVisible(true);
        } else {
            showProjectLoadError(result);
        }
    }
}

void MainWindow::onFileCloseProject() {
    if (!ProjectManager::instance()->hasOpenProject()) return;
    saveCurrentSession();
    if (!m_editorTabs->closeAll()) return; // user cancelled unsaved files
    ProjectManager::instance()->closeCurrentProject();
    m_fileTree->closeFolder();
    m_closeProjectAction->setEnabled(false);
    m_welcomeScreen->setReturnToProjectVisible(false);
    showWelcomeScreen();
}

void MainWindow::onFileExit() {
    close();
}

// Edit menu slots
void MainWindow::onEditUndo() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        editor->undo();
    }
}

void MainWindow::onEditRedo() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        editor->redo();
    }
}

void MainWindow::onEditCut() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        editor->cut();
    }
}

void MainWindow::onEditCopy() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        editor->copy();
    }
}

void MainWindow::onEditPaste() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        editor->paste();
    }
}

void MainWindow::onEditFind() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) {
        return;
    }
    
    FindReplaceDialog* dialog = new FindReplaceDialog(FindReplaceDialog::Find, this);
    
    connect(dialog, &FindReplaceDialog::findNext, this, [this, dialog, editor]() {
        QString text = dialog->findText();
        if (text.isEmpty()) return;
        
        bool found = editor->findFirst(
            text,
            dialog->useRegex(),  // use regex option
            dialog->caseSensitive(),
            dialog->wholeWord(),
            false,  // wrap
            true    // forward
        );
        
        if (!found) {
            QMessageBox::information(this, "Find", "Text not found.");
        }
    });
    
    connect(dialog, &FindReplaceDialog::findPrevious, this, [this, dialog, editor]() {
        QString text = dialog->findText();
        if (text.isEmpty()) return;
        
        bool found = editor->findFirst(
            text,
            dialog->useRegex(),  // use regex option
            dialog->caseSensitive(),
            dialog->wholeWord(),
            false,  // wrap
            false   // forward (backward)
        );
        
        if (!found) {
            QMessageBox::information(this, "Find", "Text not found.");
        }
    });
    
    dialog->show();
}

void MainWindow::onEditReplace() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) {
        return;
    }
    
    FindReplaceDialog* dialog = new FindReplaceDialog(FindReplaceDialog::Replace, this);
    
    connect(dialog, &FindReplaceDialog::findNext, this, [this, dialog, editor]() {
        QString text = dialog->findText();
        if (text.isEmpty()) return;
        
        bool found = editor->findFirst(
            text,
            dialog->useRegex(),  // use regex option
            dialog->caseSensitive(),
            dialog->wholeWord(),
            false,
            true
        );
        
        if (!found) {
            QMessageBox::information(this, "Find", "Text not found.");
        }
    });
    
    connect(dialog, &FindReplaceDialog::replaceNext, this, [dialog, editor]() {
        QString findText = dialog->findText();
        QString replaceText = dialog->replaceText();
        if (findText.isEmpty()) return;
        
        // Replace current selection if it matches (considering case sensitivity)
        if (editor->hasSelectedText()) {
            QString selectedText = editor->selectedText();
            bool matches = dialog->caseSensitive() ? 
                (selectedText == findText) : 
                (selectedText.toLower() == findText.toLower());
                
            if (matches) {
                editor->replaceSelectedText(replaceText);
            }
        }
        
        // Find next
        editor->findFirst(
            findText,
            dialog->useRegex(),  // use regex option
            dialog->caseSensitive(),
            dialog->wholeWord(),
            false,
            true
        );
    });
    
    connect(dialog, &FindReplaceDialog::replaceAll, this, [this, dialog, editor]() {
        QString findText = dialog->findText();
        QString replaceText = dialog->replaceText();
        if (findText.isEmpty()) return;
        
        // Prevent infinite loop if replacement contains search text
        if (!dialog->useRegex() && replaceText.contains(findText, 
            dialog->caseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Replace All", 
                "Replacement text contains search text. This would cause an infinite loop.");
            return;
        }
        
        int count = 0;
        
        // Go to beginning
        editor->setCursorPosition(0, 0);
        
        // Use a position-based approach to avoid infinite loops
        int lastLine = -1, lastCol = -1;
        while (editor->findFirst(
            findText,
            dialog->useRegex(),  // use regex option
            dialog->caseSensitive(),
            dialog->wholeWord(),
            false,
            true
        )) {
            int line, col;
            editor->getCursorPosition(&line, &col);
            
            // Break if we're at the same position (shouldn't happen but safety check)
            if (line == lastLine && col == lastCol) {
                break;
            }
            
            lastLine = line;
            lastCol = col;
            
            editor->replaceSelectedText(replaceText);
            count++;
            
            // Safety limit
            if (count > 10000) {
                QMessageBox::warning(this, "Replace All", 
                    QString("Stopped after %1 replacements (safety limit).").arg(count));
                return;
            }
        }
        
        QMessageBox::information(this, "Replace All", 
            QString("Replaced %1 occurrence(s).").arg(count));
    });
    
    dialog->show();
}

void MainWindow::onEditGotoLine() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) {
        return;
    }
    
    int maxLine = editor->lines();
    GotoLineDialog dialog(maxLine, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        int line = dialog.lineNumber();
        editor->gotoLine(line);
    }
}

// Build menu slots
void MainWindow::onBuildCompile() {
    QString sourceFile = getCurrentSourceFile();
    if (sourceFile.isEmpty()) {
        showBuildError("No file to compile. Please save your file first.");
        return;
    }
    
    // Get selected compiler
    QString compilerId = m_compilerCombo->currentData().toString();
    auto compiler = CompilerRegistry::instance().getCompiler(compilerId);
    
    if (!compiler) {
        showBuildError("No compiler selected.");
        return;
    }
    
    // Prepare compilation request
    CompileRequest request;
    request.sourceFile = sourceFile;
    request.outputFile = getExecutablePath(sourceFile);
    request.standard = m_standardCombo->currentText();
    request.additionalFlags = QStringList() << "-Wall" << "-Wextra";
    request.optimizationEnabled = false;
    request.optLevel = OptimizationLevel::O0;
    
    // Clear previous output and show terminal
    m_outputPanel->terminal()->clear();
    m_outputPanel->showTerminalTab();
    m_outputPanel->problems()->clear();
    
    m_statusLabel->setText("Building...");
    
    // Compile
    CompileResult result = compiler->compile(request);
    
    // Show output in terminal
    Theme theme = ThemeManager::instance()->currentTheme();
    if (!result.rawOutput.isEmpty()) {
        m_outputPanel->terminal()->appendText(result.rawOutput, theme.textPrimary);
    }
    if (!result.rawError.isEmpty()) {
        m_outputPanel->terminal()->appendText(result.rawError, QColor("#F48771"));
    }
    
    // Show diagnostics
    m_outputPanel->problems()->setDiagnostics(result.diagnostics);
    
    if (result.success) {
        m_currentExecutable = result.outputFile;
        m_statusLabel->setText(QString("Build succeeded (%1 ms)").arg(result.compilationTimeMs));
        m_outputPanel->terminal()->appendText("\nBuild succeeded!\n", QColor("#4EC994"));
    } else {
        m_statusLabel->setText("Build failed");
        m_outputPanel->terminal()->appendText("\nBuild failed!\n", QColor("#F44747"));
        m_outputPanel->showProblemsTab();
    }
}

void MainWindow::onBuildRun() {
    if (m_currentExecutable.isEmpty() || !QFile::exists(m_currentExecutable)) {
        QMessageBox::warning(this, "Error", 
            "No executable to run. Please build first.");
        return;
    }
    
    m_outputPanel->showTerminalTab();
    m_outputPanel->terminal()->runCommand(m_currentExecutable);
    m_statusLabel->setText("Running...");
}

void MainWindow::onBuildCompileAndRun() {
    onBuildCompile();
    
    // Check if compilation succeeded
    if (!m_currentExecutable.isEmpty() && QFile::exists(m_currentExecutable)) {
        onBuildRun();
    }
}

void MainWindow::onBuildStop() {
    if (m_outputPanel->terminal()->isRunning()) {
        m_outputPanel->terminal()->stopProcess();
        m_statusLabel->setText("Program stopped");
    }
}

void MainWindow::onBuildClean() {
    if (!m_currentExecutable.isEmpty() && QFile::exists(m_currentExecutable)) {
        QFile::remove(m_currentExecutable);
        m_currentExecutable.clear();
        m_statusLabel->setText("Clean complete");
    }
}

// View menu slots
void MainWindow::onViewToggleFileTree() {
    m_fileTreeDock->setVisible(!m_fileTreeDock->isVisible());
}

void MainWindow::onViewToggleOutputPanel() {
    m_outputPanelDock->setVisible(!m_outputPanelDock->isVisible());
}

void MainWindow::onViewFullscreen() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}

// Toolbar slots
void MainWindow::onCompilerChanged(int index) {
    Q_UNUSED(index);
    updateStatusBar();
}

void MainWindow::onStandardChanged(int index) {
    Q_UNUSED(index);
    updateStatusBar();
}

// File tree slots
void MainWindow::onFileTreeDoubleClicked(const QString& filePath) {
    m_editorTabs->openFile(filePath);
}

void MainWindow::onNewFileRequested(const QString& directory) {
    NewFileDialog dialog(directory, this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.createdFiles();
        for (const QString& file : files) {
            m_editorTabs->openFile(file);
        }
    }
}

// Editor slots
void MainWindow::onEditorChanged(CodeEditor* editor) {
    updateStatusBar();
    updateWindowTitle();
    
    if (editor) {
        connect(editor, &CodeEditor::cursorPositionChanged,
                this, &MainWindow::updateStatusBar, Qt::UniqueConnection);
        connect(editor, &CodeEditor::modificationChanged,
                this, &MainWindow::updateWindowTitle, Qt::UniqueConnection);

        // Keep AnalysisPanel in sync with the active editor
        if (m_analysisPanel)
            m_analysisPanel->setSourceCode(editor->text(), editor->filePath());

        if (m_previousEditor != editor)
            disconnect(m_previousEditor, &QsciScintilla::textChanged,
                       this, &MainWindow::onActiveEditorTextChanged);
        m_previousEditor = editor;
        connect(editor, &QsciScintilla::textChanged,
                this, &MainWindow::onActiveEditorTextChanged);
    }
}

void MainWindow::onActiveEditorTextChanged() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor && m_analysisPanel) {
        m_analysisPanel->setSourceCode(editor->text(), editor->filePath());
    }
}

// Problems slots
void MainWindow::onDiagnosticClicked(const QString& file, int line, int column) {
    Q_UNUSED(column);
    
    // Open file if not already open
    CodeEditor* editor = nullptr;
    
    // Check if file is already open
    for (int i = 0; i < m_editorTabs->count(); ++i) {
        CodeEditor* ed = m_editorTabs->editorAt(i);
        if (ed && ed->filePath() == file) {
            m_editorTabs->setCurrentIndex(i);
            editor = ed;
            break;
        }
    }
    
    // Open file if not found
    if (!editor) {
        editor = m_editorTabs->openFile(file);
    }
    
    // Go to line
    if (editor) {
        editor->gotoLine(line);
    }
}

// Window dragging
void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        // Only drag from title bar area, excluding buttons
        const int buttonContainerWidth = WINDOW_BUTTON_WIDTH * WINDOW_BUTTON_COUNT;
        if (pos.y() < TITLE_BAR_HEIGHT && pos.x() < width() - buttonContainerWidth) {
            m_dragging = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
#else
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
#endif
            event->accept();
            return;
        }
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        move(event->globalPosition().toPoint() - m_dragPosition);
#else
        move(event->globalPos() - m_dragPosition);
#endif
        event->accept();
        return;
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    m_dragging = false;
    QMainWindow::mouseReleaseEvent(event);
}

// Double-click title bar to maximize/restore
void MainWindow::mouseDoubleClickEvent(QMouseEvent* event) {
    const int buttonContainerWidth = WINDOW_BUTTON_WIDTH * WINDOW_BUTTON_COUNT;
    if (event->pos().y() < TITLE_BAR_HEIGHT && event->pos().x() < width() - buttonContainerWidth) {
        if (isMaximized()) {
            showNormal();
            m_maximizeBtn->setText("□");
        } else {
            showMaximized();
            m_maximizeBtn->setText("❐");
        }
        event->accept();
        return;
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

// Windows native event for resize from edges
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#else
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
#endif
    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_NCHITTEST) {
            // Enable resize from window edges
            RECT winrect;
            GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
            
            int x = GET_X_LPARAM(msg->lParam);
            int y = GET_Y_LPARAM(msg->lParam);
            
            // Check corners first
            if (x < winrect.left + RESIZE_BORDER_WIDTH && y < winrect.top + RESIZE_BORDER_WIDTH) {
                *result = HTTOPLEFT;
                return true;
            }
            if (x > winrect.right - RESIZE_BORDER_WIDTH && y < winrect.top + RESIZE_BORDER_WIDTH) {
                *result = HTTOPRIGHT;
                return true;
            }
            if (x < winrect.left + RESIZE_BORDER_WIDTH && y > winrect.bottom - RESIZE_BORDER_WIDTH) {
                *result = HTBOTTOMLEFT;
                return true;
            }
            if (x > winrect.right - RESIZE_BORDER_WIDTH && y > winrect.bottom - RESIZE_BORDER_WIDTH) {
                *result = HTBOTTOMRIGHT;
                return true;
            }
            // Check edges
            if (x < winrect.left + RESIZE_BORDER_WIDTH) {
                *result = HTLEFT;
                return true;
            }
            if (x > winrect.right - RESIZE_BORDER_WIDTH) {
                *result = HTRIGHT;
                return true;
            }
            if (y < winrect.top + RESIZE_BORDER_WIDTH) {
                *result = HTTOP;
                return true;
            }
            if (y > winrect.bottom - RESIZE_BORDER_WIDTH) {
                *result = HTBOTTOM;
                return true;
            }
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}
#else
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#else
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
#endif
    return QMainWindow::nativeEvent(eventType, message, result);
}
#endif

// Helper methods
QString MainWindow::getCurrentSourceFile() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) {
        return QString();
    }
    
    QString filePath = editor->filePath();
    
    // If file is not saved, save it first
    if (filePath.isEmpty()) {
        if (!m_editorTabs->saveCurrentFileAs()) {
            return QString();
        }
        filePath = editor->filePath();
    } else if (editor->isModified()) {
        if (!m_editorTabs->saveCurrentFile()) {
            return QString();
        }
    }
    
    return filePath;
}

QString MainWindow::getExecutablePath(const QString& sourceFile) {
    QFileInfo info(sourceFile);
    QString baseName = info.completeBaseName();
    
#ifdef Q_OS_WIN
    return info.absolutePath() + "/" + baseName + ".exe";
#else
    return info.absolutePath() + "/" + baseName;
#endif
}

void MainWindow::showBuildError(const QString& message) {
    m_outputPanel->showTerminalTab();
    m_outputPanel->terminal()->appendText(message + "\n", QColor("#F44747"));
    m_statusLabel->setText("Build error");
}

void MainWindow::saveCurrentSession() {
    if (!ProjectManager::instance()->hasOpenProject()) return;
    
    auto project = ProjectManager::instance()->currentProject();
    
    QStringList openFiles;
    for (int i = 0; i < m_editorTabs->count(); ++i) {
        CodeEditor* editor = m_editorTabs->editorAt(i);
        if (editor && !editor->filePath().isEmpty()) {
            openFiles.append(editor->filePath());
        }
    }
    
    QString activeFile;
    if (m_editorTabs->currentEditor()) {
        activeFile = m_editorTabs->currentEditor()->filePath();
    }
    
    QStringList expandedFolders;
    
    project->saveSession(openFiles, activeFile, expandedFolders);
    project->save();
}

void MainWindow::showProjectLoadError(Project::LoadResult result) {
    QString message;
    switch (result) {
        case Project::LoadResult::FileNotFound:
            message = "Project file not found.";
            break;
        case Project::LoadResult::InvalidFormat:
            message = "Invalid project file format.";
            break;
        case Project::LoadResult::VersionMismatch:
            message = "Unsupported project file version.";
            break;
        case Project::LoadResult::PermissionDenied:
            message = "Permission denied when reading project file.";
            break;
        default:
            message = "Failed to load project.";
            break;
    }
    QMessageBox::warning(this, "Error", message);
}

void MainWindow::restoreProjectSession(Project* project) {
    for (const QString& file : project->openFiles()) {
        QString fullPath = QDir(project->projectDirectory()).absoluteFilePath(file);
        if (QFileInfo::exists(fullPath)) {
            m_editorTabs->openFile(fullPath);
        } else if (QFileInfo::exists(file)) {
            m_editorTabs->openFile(file);
        }
    }
    if (!project->activeFile().isEmpty()) {
        QString activePath = QDir(project->projectDirectory()).absoluteFilePath(project->activeFile());
        if (QFileInfo::exists(activePath)) {
            m_editorTabs->openFile(activePath);
        } else if (QFileInfo::exists(project->activeFile())) {
            m_editorTabs->openFile(project->activeFile());
        }
    }
}
