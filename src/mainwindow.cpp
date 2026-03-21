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
#include "ui/QuizModeWindow.h"
#include "ui/SettingsDialog.h"
#include "quiz/UserManager.h"
#include "quiz/AdminAccessController.h"
#include "ui/QuizAdminPanel.h"
#include "core/AppSettings.h"
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
#include <QDir>
#include <QFileInfo>
#include <QActionGroup>
#include <QInputDialog>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>
#include <QGuiApplication>
#include <QOperatingSystemVersion>
#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QShortcut>
#include <QStyle>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NativeWindow);
#else
    setWindowFlags(Qt::FramelessWindowHint);
#endif
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_fileManager = new FileManager(this);
    m_project     = new Project(this);

    setupUi();
    setupMenus();
    setupCustomTitleBar();
    setupToolbar();
    setupDockWidgets();
    setupStatusBar();
    setupConnections();
    setupWelcomeScreen();

    CompilerRegistry::instance().autoScanCompilers();
    loadCompilers();

    {
        const QString lastUser = AppSettings::lastLoggedInUser();
        AppSettings tempSettings(lastUser);
        const QString savedTheme = tempSettings.theme();
        ThemeManager::instance()->setTheme(savedTheme.isEmpty() ? QStringLiteral("dark") : savedTheme);
    }

    updateWindowTitle();
    showWelcomeScreen();

    if (UserManager::instance().isLoggedIn()) {
        onSettingsChanged();
    }

    m_adminShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::SHIFT | Qt::Key_M), this);
    connect(m_adminShortcut, &QShortcut::activated, this, &MainWindow::showQuizAdminPanel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ─────────────────────────────────────────────────────────────────────────────
// setupUi
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupUi()
{
    setWindowTitle("CppAtlas - C++ Learning IDE");
    resize(1200, 800);

    m_editorTabs   = new EditorTabWidget(this);
    m_analysisPanel = new AnalysisPanel(this);
    m_analysisPanel->hide();

    // Horizontal splitter: editor | analysis panel
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(m_editorTabs);
    m_mainSplitter->addWidget(m_analysisPanel);
    m_mainSplitter->setStretchFactor(0, 3);
    m_mainSplitter->setStretchFactor(1, 1);

    // Output panel (standalone widget, not a dock)
    m_outputPanel = new OutputPanel(this);
    m_outputPanel->setMinimumHeight(80);

    // Vertical splitter: (editor+analysis) on top | output on bottom
    m_vertSplitter = new QSplitter(Qt::Vertical, this);
    m_vertSplitter->addWidget(m_mainSplitter);
    m_vertSplitter->addWidget(m_outputPanel);
    m_vertSplitter->setStretchFactor(0, 5);
    m_vertSplitter->setStretchFactor(1, 1);
    // Allow output to collapse to min height
    m_vertSplitter->setCollapsible(1, true);

    // Quiz mode window
    m_quizModeWindow = new QuizModeWindow(this);
    connect(m_quizModeWindow, &QuizModeWindow::exitRequested,
            this, &MainWindow::onQuizModeExit);

    // Mode stack
    m_modeStack = new QStackedWidget(this);
    m_modeStack->addWidget(m_vertSplitter); // index 0: IDE

    setCentralWidget(m_modeStack);
}

// ─────────────────────────────────────────────────────────────────────────────
// setupCustomTitleBar
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupCustomTitleBar()
{
    m_titleBar = new QWidget(this);
    m_titleBar->setObjectName("customTitleBar");
    m_titleBar->setFixedHeight(TITLE_BAR_HEIGHT);
    m_titleBar->setMouseTracking(true);

    QHBoxLayout* layout = new QHBoxLayout(m_titleBar);
    layout->setContentsMargins(8, 0, 0, 0);
    layout->setSpacing(0);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setText("C++");
    m_iconLabel->setObjectName("appIcon");
    m_iconLabel->setFixedSize(36, 28);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_iconLabel);
    layout->addSpacing(8);

    if (menuBar()) {
        menuBar()->setObjectName("titleBarMenu");
        layout->addWidget(menuBar());
    }

    m_titleLabelLeft = new QLabel("CppAtlas - C++ Learning IDE", this);
    m_titleLabelLeft->setObjectName("windowTitleLeft");
    m_titleLabelLeft->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_titleLabelLeft->hide();
    layout->addSpacing(8);
    layout->addWidget(m_titleLabelLeft);

    layout->addStretch();

    m_titleLabel = new QLabel("CppAtlas - C++ Learning IDE", this);
    m_titleLabel->setObjectName("windowTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);

    layout->addStretch();

    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* btnLayout = new QHBoxLayout(buttonContainer);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(0);

    m_minimizeBtn = new QPushButton(this);
    m_minimizeBtn->setObjectName("minimizeButton");
    m_minimizeBtn->setText("─");
    m_minimizeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_minimizeBtn->setFlat(true);
    connect(m_minimizeBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    btnLayout->addWidget(m_minimizeBtn);

    m_maximizeBtn = new QPushButton(this);
    m_maximizeBtn->setObjectName("maximizeButton");
    m_maximizeBtn->setText("□");
    m_maximizeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_maximizeBtn->setFlat(true);
    connect(m_maximizeBtn, &QPushButton::clicked, this, [this]() {
        if (isMaximized()) { showNormal(); m_maximizeBtn->setText("□"); }
        else               { showMaximized(); m_maximizeBtn->setText("❐"); }
    });
    btnLayout->addWidget(m_maximizeBtn);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setObjectName("closeButton");
    m_closeBtn->setText("✕");
    m_closeBtn->setFixedSize(WINDOW_BUTTON_WIDTH, TITLE_BAR_HEIGHT);
    m_closeBtn->setFlat(true);
    connect(m_closeBtn, &QPushButton::clicked, this, &QMainWindow::close);
    btnLayout->addWidget(m_closeBtn);

    layout->addWidget(buttonContainer);
    setMenuWidget(m_titleBar);
}

// ─────────────────────────────────────────────────────────────────────────────
// setupMenus
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupMenus()
{
    // File menu
    m_fileMenu = menuBar()->addMenu("&File");

    QAction* newAction = m_fileMenu->addAction("New &File");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onFileNew);

    QAction* newProjectAction = m_fileMenu->addAction("New &Project...");
    newProjectAction->setShortcut(QKeySequence("Ctrl+Shift+N"));
    connect(newProjectAction, &QAction::triggered, this, &MainWindow::onFileNewProject);

    m_fileMenu->addSeparator();

    QAction* openAction = m_fileMenu->addAction("&Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);

    m_fileMenu->addSeparator();

    QAction* saveAction = m_fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onFileSave);

    QAction* saveAsAction = m_fileMenu->addAction("Save &As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onFileSaveAs);

    m_fileMenu->addSeparator();

    QAction* openFolderAction = m_fileMenu->addAction("Open F&older...");
    openFolderAction->setShortcut(QKeySequence("Ctrl+K"));
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::onFileOpenFolder);

    QAction* openProjectAction = m_fileMenu->addAction("Open Pro&ject...");
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::onFileOpenProject);

    m_fileMenu->addSeparator();
    m_closeProjectAction = m_fileMenu->addAction(QStringLiteral("Close Project"));
    m_closeProjectAction->setEnabled(false);
    connect(m_closeProjectAction, &QAction::triggered, this, &MainWindow::onFileCloseProject);

    m_fileMenu->addSeparator();
    QAction* exitAction = m_fileMenu->addAction("E&xit");
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

    // Find — opens FindReplaceDialog in Find mode, with inline toggle to Replace
    m_findAction = m_editMenu->addAction("&Find / Replace...");
    m_findAction->setShortcut(QKeySequence::Find);
    connect(m_findAction, &QAction::triggered, this, &MainWindow::onEditFind);

    // Replace shortcut also opens the dialog but starts in Replace mode
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

    // File Tree side toggle
    m_fileTreeSideAction = m_viewMenu->addAction("File Tree: Toggle Left/Right");
    connect(m_fileTreeSideAction, &QAction::triggered, this, &MainWindow::onViewToggleFileTreeSide);

    // Output full-height mode
    m_outputFullHeightAction = m_viewMenu->addAction("Output Panel: Full Height Mode");
    m_outputFullHeightAction->setCheckable(true);
    connect(m_outputFullHeightAction, &QAction::toggled, this, &MainWindow::onToggleOutputFullHeight);

    m_viewMenu->addSeparator();

    // Theme submenu
    QMenu* themeMenu = m_viewMenu->addMenu("&Theme");
    QActionGroup* themeGroup = new QActionGroup(this);

    auto addTheme = [&](const QString& name, const QString& data) {
        QAction* a = themeMenu->addAction(name);
        a->setCheckable(true);
        a->setData(data);
        themeGroup->addAction(a);
        return a;
    };
    addTheme("Dark", "dark")->setChecked(true);
    addTheme("Light", "light");
    addTheme("Dracula", "dracula");
    addTheme("Monokai", "monokai");
    addTheme("One Monokai", "one-monokai");

    connect(themeGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        QString themeName = action->data().toString();
        ThemeManager::instance()->setTheme(themeName);
        for (int i = 0; i < m_editorTabs->count(); ++i) {
            CodeEditor* editor = m_editorTabs->editorAt(i);
            if (editor) editor->applyTheme(themeName);
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

    // Tools menu
    m_toolsMenu = menuBar()->addMenu(QStringLiteral("&Tools"));

    m_toggleAnalysisAction = m_toolsMenu->addAction(QStringLiteral("Toggle &Analysis Panel"));
    m_toggleAnalysisAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_A));
    connect(m_toggleAnalysisAction, &QAction::triggered, this, [this]() {
        bool visible = m_analysisPanel->isVisible();
        m_analysisPanel->setVisible(!visible);
        if (m_analysisPanel->isVisible()) {
            m_mainSplitter->setSizes({m_mainSplitter->width() / 2,
                                      m_mainSplitter->width() / 2});
        }
        updateTitlePosition();
    });

    m_toolsMenu->addSeparator();

    QAction* showInsightsAction = m_toolsMenu->addAction(QStringLiteral("C++ &Insights"));
    showInsightsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    connect(showInsightsAction, &QAction::triggered, this, [this]() {
        m_analysisPanel->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabInsights);
        updateTitlePosition();
    });

    QAction* showAssemblyAction = m_toolsMenu->addAction(QStringLiteral("&Assembly View"));
    showAssemblyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(showAssemblyAction, &QAction::triggered, this, [this]() {
        m_analysisPanel->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabAssembly);
        updateTitlePosition();
    });

    QAction* showBenchmarkAction = m_toolsMenu->addAction(QStringLiteral("&Benchmark"));
    showBenchmarkAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    connect(showBenchmarkAction, &QAction::triggered, this, [this]() {
        m_analysisPanel->setVisible(true);
        m_analysisPanel->setCurrentIndex(AnalysisPanel::TabBenchmark);
        updateTitlePosition();
    });

    // Settings menu
    m_settingsMenu = menuBar()->addMenu(QStringLiteral("&Settings"));
    QAction* openSettingsAction = m_settingsMenu->addAction(QStringLiteral("&Preferences..."));
    openSettingsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    connect(openSettingsAction, &QAction::triggered, this, &MainWindow::onOpenSettings);

    // Help menu
    m_helpMenu = menuBar()->addMenu(QStringLiteral("&Help"));
    QAction* aboutAction = m_helpMenu->addAction("&About CppAtlas");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

// ─────────────────────────────────────────────────────────────────────────────
// setupToolbar
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupToolbar()
{
    m_mainToolbar = addToolBar("Main Toolbar");
    m_mainToolbar->setObjectName("mainToolbar");
    m_mainToolbar->setMovable(false);

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
            if (basePath.isEmpty()) basePath = QDir::currentPath();
            QDir(basePath).mkdir(folderName);
        }
    });
    QAction* newProjectAction2 = newMenu->addAction("New Project...");
    connect(newProjectAction2, &QAction::triggered, this, &MainWindow::onFileNewProject);
    newButton->setMenu(newMenu);
    connect(newButton, &QToolButton::clicked, this, &MainWindow::onFileNew);
    m_mainToolbar->addWidget(newButton);

    m_mainToolbar->addAction("Open", this, &MainWindow::onFileOpen);
    m_mainToolbar->addAction("Save", this, &MainWindow::onFileSave);
    m_mainToolbar->addSeparator();
    m_mainToolbar->addAction("Build", this, &MainWindow::onBuildCompile);
    m_mainToolbar->addAction("Run",   this, &MainWindow::onBuildRun);
    m_mainToolbar->addAction("Stop",  this, &MainWindow::onBuildStop);
    m_mainToolbar->addSeparator();

    m_mainToolbar->addWidget(new QLabel(" Compiler: "));
    m_compilerCombo = new QComboBox(m_mainToolbar);
    m_compilerCombo->setMinimumWidth(150);
    m_mainToolbar->addWidget(m_compilerCombo);

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

// ─────────────────────────────────────────────────────────────────────────────
// setupDockWidgets — ONLY file tree dock remains; output is in vertSplitter
// Remove float (undock) button; keep only close button, enlarge it
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupDockWidgets()
{
    m_fileTreeDock = new QDockWidget("File Tree", this);
    m_fileTreeDock->setObjectName("fileTreeDock");

    // Remove float button, keep only close button
    m_fileTreeDock->setFeatures(QDockWidget::DockWidgetClosable);

    m_fileTree = new FileTreeWidget(m_fileTreeDock);
    m_fileTreeDock->setWidget(m_fileTree);
    addDockWidget(Qt::LeftDockWidgetArea, m_fileTreeDock);

    // Apply enlarged close button via stylesheet
    m_fileTreeDock->setStyleSheet(
        "QDockWidget::close-button {"
        "  subcontrol-position: top right;"
        "  subcontrol-origin: margin;"
        "  width: 40px; height: 40px;"
        "  margin: 2px;"
        "}"
        );
}

// ─────────────────────────────────────────────────────────────────────────────
// setupStatusBar
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupStatusBar()
{
    m_statusLabel    = new QLabel("Ready");
    m_cursorPosLabel = new QLabel("Ln 1, Col 1");
    m_standardLabel  = new QLabel("C++17");
    m_compilerLabel  = new QLabel("No compiler");

    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_cursorPosLabel);
    statusBar()->addPermanentWidget(m_standardLabel);
    statusBar()->addPermanentWidget(m_compilerLabel);
}

// ─────────────────────────────────────────────────────────────────────────────
// setupConnections
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupConnections()
{
    connect(m_editorTabs, &EditorTabWidget::editorChanged,
            this, &MainWindow::onEditorChanged);

    connect(m_fileTree, &FileTreeWidget::fileDoubleClicked,
            this, &MainWindow::onFileTreeDoubleClicked);
    connect(m_fileTree, &FileTreeWidget::newFileRequested,
            this, &MainWindow::onNewFileRequested);
    connect(m_fileTree, &FileTreeWidget::fileCreated,
            this, [this](const QString& filePath) { m_editorTabs->openFile(filePath); });
    connect(m_fileTree, &FileTreeWidget::fileDeleted,
            this, [this](const QString& filePath) { m_editorTabs->closeFileTab(filePath); });
    connect(m_fileTree, &FileTreeWidget::fileRenamed,
            this, [this](const QString& oldPath, const QString& newPath) {
                m_editorTabs->updateFilePath(oldPath, newPath);
            });

    // Problems click → navigate to error location
    connect(m_outputPanel->problems(), &ProblemsWidget::diagnosticClicked,
            this, &MainWindow::onDiagnosticClicked);

    connect(ProjectManager::instance(), &ProjectManager::projectClosed,
            this, [this]() {
                if (m_closeProjectAction) m_closeProjectAction->setEnabled(false);
            });

    connect(m_analysisPanel, &AnalysisPanel::sourceLineActivated,
            this, [this](int line) {
                CodeEditor* ed = m_editorTabs->currentEditor();
                if (ed) ed->gotoLine(line);
            });

    connect(m_compilerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                m_analysisPanel->setCompilerId(m_compilerCombo->currentData().toString());
            });
    connect(m_standardCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                m_analysisPanel->setStandard(m_standardCombo->currentText());
            });
}

// ─────────────────────────────────────────────────────────────────────────────
// setupWelcomeScreen
// IDE Mode button behavior, Continue without Project logic
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupWelcomeScreen()
{
    m_welcomeScreen = new WelcomeScreen(this);

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

    connect(m_welcomeScreen, &WelcomeScreen::quizModeRequested,
            this, &MainWindow::onQuizModeRequested);

    // "Continue without Project" — only active when no project open
    connect(m_welcomeScreen, &WelcomeScreen::continueWithoutProjectRequested,
            this, [this]() {
                hideWelcomeScreen();
                // Create a blank untitled tab directly — no dialog needed
                if (m_editorTabs->count() == 0)
                    m_editorTabs->newFile();
            });

    // "Return to Project" — goes back to IDE when project is open
    connect(m_welcomeScreen, &WelcomeScreen::returnToProjectRequested,
            this, [this]() { hideWelcomeScreen(); });

    // "IDE Mode" button — same as Return to Project if open, else Continue without Project
    connect(m_welcomeScreen, &WelcomeScreen::ideModeRequested,
            this, [this]() {
                hideWelcomeScreen();
                // If no project and no open tabs, create a blank untitled file
                if (!ProjectManager::instance()->hasOpenProject() && m_editorTabs->count() == 0)
                    m_editorTabs->newFile();
            });

    m_modeStack->addWidget(m_welcomeScreen);  // index 1
    m_modeStack->addWidget(m_quizModeWindow); // index 2
}

// ─────────────────────────────────────────────────────────────────────────────
// Mode switching
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::showIDE()
{
    m_modeStack->setCurrentIndex(0);
    m_fileTreeDock->show();
    // Output panel is always visible in IDE (in vertSplitter)
    m_outputPanel->show();
    statusBar()->setVisible(true);
    updateMenuState(false);
    updateCustomTitleLabel("CppAtlas — C++ Learning IDE");
}

void MainWindow::showWelcomeScreen()
{
    if (ProjectManager::instance()->hasOpenProject()) {
        saveCurrentSession();
    }

    m_modeStack->setCurrentIndex(1);
    m_fileTreeDock->hide();
    m_outputPanel->hide();
    statusBar()->setVisible(false);

    bool hasOpenProject = ProjectManager::instance()->hasOpenProject();
    // mutual exclusion — only one of Return/Continue is visible at a time
    m_welcomeScreen->setReturnToProjectVisible(hasOpenProject);

    updateMenuState(true);
    updateCustomTitleLabel("CppAtlas — Welcome");
}

void MainWindow::hideWelcomeScreen()
{
    showIDE();
    if (m_closeProjectAction) m_closeProjectAction->setEnabled(
            ProjectManager::instance()->hasOpenProject());
}

void MainWindow::onQuizModeRequested()   { showQuizModeWindow(); }
void MainWindow::onQuizModeExit()        { hideQuizModeWindow(); showWelcomeScreen(); }

void MainWindow::showQuizModeWindow()
{
    m_modeStack->setCurrentIndex(2);
    m_fileTreeDock->hide();
    m_outputPanel->hide();
    statusBar()->setVisible(false);
    updateMenuState(true);
    updateCustomTitleLabel("CppAtlas — Quiz Mode");
}

void MainWindow::hideQuizModeWindow() { showIDE(); }

// ─────────────────────────────────────────────────────────────────────────────
// View menu — File Tree side toggle
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onViewToggleFileTreeSide()
{
    m_fileTreeOnLeft = !m_fileTreeOnLeft;
    removeDockWidget(m_fileTreeDock);
    addDockWidget(m_fileTreeOnLeft ? Qt::LeftDockWidgetArea : Qt::RightDockWidgetArea,
                  m_fileTreeDock);
    m_fileTreeDock->show();
    m_fileTreeSideAction->setText(m_fileTreeOnLeft
                                      ? "File Tree: Toggle Left/Right  (now Left)"
                                      : "File Tree: Toggle Left/Right  (now Right)");
}

// ─────────────────────────────────────────────────────────────────────────────
// Output Panel full-height mode
// When enabled: collapse editor area, give full vertical space to output panel
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onToggleOutputFullHeight()
{
    m_outputFullHeight = m_outputFullHeightAction->isChecked();
    if (m_outputFullHeight) {
        // Collapse editor area to minimum, expand output
        m_vertSplitter->setSizes({0, m_vertSplitter->height()});
    } else {
        // Restore reasonable split
        int total = m_vertSplitter->height();
        m_vertSplitter->setSizes({total * 3 / 4, total / 4});
    }
}

void MainWindow::onViewToggleFileTree()
{
    m_fileTreeDock->setVisible(!m_fileTreeDock->isVisible());
}

void MainWindow::onViewToggleOutputPanel()
{
    m_outputPanel->setVisible(!m_outputPanel->isVisible());
    // When re-showing, restore reasonable sizes
    if (m_outputPanel->isVisible()) {
        int total = m_vertSplitter->height();
        if (m_vertSplitter->sizes().at(1) < 40) {
            m_vertSplitter->setSizes({total * 3 / 4, total / 4});
        }
    }
}

void MainWindow::onViewFullscreen()
{
    if (isFullScreen()) showNormal();
    else                showFullScreen();
}

// ─────────────────────────────────────────────────────────────────────────────
// Settings
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onOpenSettings()
{
    if (!UserManager::instance().isLoggedIn()) return;
    const QString username = UserManager::instance().currentUser().username;

    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(username, this);
        m_settingsDialog->setAttribute(Qt::WA_DeleteOnClose, false);
        connect(m_settingsDialog, &SettingsDialog::settingsChanged,
                this, &MainWindow::onSettingsChanged);
    }
    if (m_settingsDialog->isVisible()) {
        m_settingsDialog->raise();
        m_settingsDialog->activateWindow();
    } else {
        m_settingsDialog->show();
    }
}

void MainWindow::onSettingsChanged()
{
    if (!UserManager::instance().isLoggedIn()) return;
    AppSettings s(UserManager::instance().currentUser().username);

    const QString fontFamily = s.editorFontFamily();
    const int     fontSize   = s.editorFontSize();
    const bool    showLineNums = s.showLineNumbers();
    const bool    wordWrap   = s.wordWrap();

    QFont f(fontFamily.isEmpty() ? QStringLiteral("Monospace") : fontFamily,
            fontSize > 0 ? fontSize : 12);

    for (int i = 0; i < m_editorTabs->count(); ++i) {
        CodeEditor* editor = m_editorTabs->editorAt(i);
        if (!editor) continue;
        editor->applyEditorSettings(f, showLineNums, wordWrap);
    }

    if (m_analysisPanel) {
        m_analysisPanel->applyToolEditorSettings(s);
    }
}

void MainWindow::updateMenuState(bool isWelcomeOrQuiz)
{
    const bool showIDEMenus = !isWelcomeOrQuiz;
    if (m_fileMenu)  m_fileMenu->menuAction()->setVisible(showIDEMenus);
    if (m_editMenu)  m_editMenu->menuAction()->setVisible(showIDEMenus);
    if (m_buildMenu) m_buildMenu->menuAction()->setVisible(showIDEMenus);
    if (m_viewMenu)  m_viewMenu->menuAction()->setVisible(showIDEMenus);
    if (m_toolsMenu) m_toolsMenu->menuAction()->setVisible(showIDEMenus);
    if (m_mainToolbar) m_mainToolbar->setVisible(showIDEMenus);
}

// ─────────────────────────────────────────────────────────────────────────────
// Compiler
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::loadCompilers()
{
    m_compilerCombo->clear();
    auto compilers = CompilerRegistry::instance().getAvailableCompilers();
    if (compilers.isEmpty()) {
        m_compilerCombo->addItem("No compilers found");
        m_compilerCombo->setEnabled(false);
        m_compilerLabel->setText("No compiler");
        return;
    }
    for (const auto& compiler : compilers)
        m_compilerCombo->addItem(compiler->name(), compiler->id());

    QString defaultId = CompilerRegistry::instance().defaultCompilerId();
    int index = m_compilerCombo->findData(defaultId);
    if (index >= 0) m_compilerCombo->setCurrentIndex(index);

    if (m_analysisPanel) {
        m_analysisPanel->setCompilerId(m_compilerCombo->currentData().toString());
        m_analysisPanel->setStandard(m_standardCombo->currentText());
    }
}

void MainWindow::updateStatusBar()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        int line, col;
        editor->getCursorPosition(&line, &col);
        m_cursorPosLabel->setText(QString("Ln %1, Col %2").arg(line + 1).arg(col + 1));
    }
    m_standardLabel->setText(m_standardCombo->currentText().toUpper());
    QString compilerId = m_compilerCombo->currentData().toString();
    auto compiler = CompilerRegistry::instance().getCompiler(compilerId);
    if (compiler) m_compilerLabel->setText(compiler->name());
}

void MainWindow::updateWindowTitle()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    QString title = "CppAtlas - C++ Learning IDE";
    if (editor && !editor->filePath().isEmpty()) {
        QFileInfo info(editor->filePath());
        title = QString("%1 - %2").arg(info.fileName()).arg(title);
        if (editor->isModified()) title = "* " + title;
    }
    setWindowTitle(title);
    updateCustomTitleLabel(title);
}

void MainWindow::updateCustomTitleLabel(const QString& title)
{
    if (m_titleLabel)     m_titleLabel->setText(title);
    if (m_titleLabelLeft) m_titleLabelLeft->setText(title);
}

void MainWindow::updateTitlePosition()
{
    const bool panelVisible = m_analysisPanel && m_analysisPanel->isVisible();
    if (m_titleLabel)     m_titleLabel->setVisible(!panelVisible);
    if (m_titleLabelLeft) m_titleLabelLeft->setVisible(panelVisible);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ProjectManager::instance()->hasOpenProject()) saveCurrentSession();
    m_previousEditor = nullptr;  // prevent dangling pointer during shutdown
    if (m_editorTabs->closeAll()) {
        ProjectManager::instance()->closeCurrentProject();
        event->accept();
    } else {
        event->ignore();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// File menu slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onFileNew()
{
    QString defaultDir = m_fileTree->rootPath();
    if (defaultDir.isEmpty()) defaultDir = QDir::homePath();
    NewFileDialog dialog(defaultDir, this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.createdFiles();
        for (const QString& file : files) m_editorTabs->openFile(file);
    }
}

void MainWindow::onFileNewProject()
{
    NewProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ProjectSettings settings;
        settings.compilerId          = dialog.selectedCompilerId();
        settings.standard            = dialog.selectedStandard();
        settings.createMainCpp       = dialog.createMainCpp();
        settings.createCMakeLists    = dialog.createCMakeLists();
        settings.createProjectFolder = dialog.createProjectFolder();

        if (ProjectManager::instance()->createProject(
                dialog.projectName(), dialog.projectLocation(), settings)) {
            auto project = ProjectManager::instance()->currentProject();
            m_fileTree->openFolder(project->projectDirectory());
            if (settings.createMainCpp) {
                QString mainPath = project->projectDirectory() + QDir::separator()
                + "src" + QDir::separator() + "main.cpp";
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

void MainWindow::onFileOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(),
                                                    "C++ Files (*.cpp *.h *.hpp *.cc *.cxx);;All Files (*)");
    if (!filePath.isEmpty()) {
        m_editorTabs->openFile(filePath);
        m_fileManager->addRecentFile(filePath);
        RecentProjectsManager::instance()->addRecentProject(filePath);
    }
}

void MainWindow::onFileSave()    { m_editorTabs->saveCurrentFile(); updateWindowTitle(); }
void MainWindow::onFileSaveAs()  { m_editorTabs->saveCurrentFileAs(); updateWindowTitle(); }

void MainWindow::onFileOpenFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Open Folder", QString(),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty()) {
        m_fileTree->openFolder(folderPath);
        m_statusLabel->setText("Opened folder: " + folderPath);
        RecentProjectsManager::instance()->addRecentProject(folderPath);
    }
}

void MainWindow::onFileOpenProject()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project", QString(),
                                                    "CppAtlas Project (*.cppatlas);;All Files (*)");
    if (!filePath.isEmpty()) {
        auto result = ProjectManager::instance()->openProject(filePath);
        if (result == Project::LoadResult::Success) {
            auto project = ProjectManager::instance()->currentProject();
            m_fileTree->openFolder(project->projectDirectory());
            m_previousEditor = nullptr;  // reset before restoring session
            restoreProjectSession(project);
            hideWelcomeScreen();
            m_statusLabel->setText("Project: " + project->name());
            m_welcomeScreen->setReturnToProjectVisible(true);
        } else {
            showProjectLoadError(result);
        }
    }
}

void MainWindow::onFileCloseProject()
{
    if (!ProjectManager::instance()->hasOpenProject()) return;
    saveCurrentSession();
    // Clear dangling editor reference BEFORE closing tabs
    m_previousEditor = nullptr;
    if (!m_editorTabs->closeAll()) return;
    ProjectManager::instance()->closeCurrentProject();
    m_fileTree->closeFolder();
    m_closeProjectAction->setEnabled(false);
    m_welcomeScreen->setReturnToProjectVisible(false);
    showWelcomeScreen();
}

void MainWindow::onFileExit() { close(); }

// ─────────────────────────────────────────────────────────────────────────────
// Edit menu slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onEditUndo()  { if (auto* e = m_editorTabs->currentEditor()) e->undo(); }
void MainWindow::onEditRedo()  { if (auto* e = m_editorTabs->currentEditor()) e->redo(); }
void MainWindow::onEditCut()   { if (auto* e = m_editorTabs->currentEditor()) e->cut(); }
void MainWindow::onEditCopy()  { if (auto* e = m_editorTabs->currentEditor()) e->copy(); }
void MainWindow::onEditPaste() { if (auto* e = m_editorTabs->currentEditor()) e->paste(); }

void MainWindow::onEditFind()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) return;

    FindReplaceDialog* dialog = new FindReplaceDialog(FindReplaceDialog::Find, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    wireFindReplaceDialog(dialog, editor);
    dialog->show();
}

void MainWindow::onEditReplace()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) return;

    FindReplaceDialog* dialog = new FindReplaceDialog(FindReplaceDialog::Replace, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    wireFindReplaceDialog(dialog, editor);
    dialog->show();
}

// Helper — wire all signals for a FindReplaceDialog
// NOTE: this must be declared in mainwindow.h as a private method
// We inline it here to avoid header changes; add it as private:
void MainWindow::wireFindReplaceDialog(FindReplaceDialog* dialog, CodeEditor* editor)
{
    connect(dialog, &FindReplaceDialog::findNext, this, [this, dialog, editor]() {
        QString text = dialog->findText();
        if (text.isEmpty()) return;
        bool found = editor->findFirst(text, dialog->useRegex(),
                                       dialog->caseSensitive(), dialog->wholeWord(), true, true);
        if (!found) QMessageBox::information(this, "Find", "Text not found.");
    });

    connect(dialog, &FindReplaceDialog::findPrevious, this, [this, dialog, editor]() {
        QString text = dialog->findText();
        if (text.isEmpty()) return;
        bool found = editor->findFirst(text, dialog->useRegex(),
                                       dialog->caseSensitive(), dialog->wholeWord(), true, false);
        if (!found) QMessageBox::information(this, "Find", "Text not found.");
    });

    connect(dialog, &FindReplaceDialog::replaceNext, this, [dialog, editor]() {
        QString findText = dialog->findText();
        QString replaceText = dialog->replaceText();
        if (findText.isEmpty()) return;
        if (editor->hasSelectedText()) {
            QString sel = editor->selectedText();
            bool matches = dialog->caseSensitive()
                               ? (sel == findText)
                               : (sel.toLower() == findText.toLower());
            if (matches) editor->replaceSelectedText(replaceText);
        }
        editor->findFirst(findText, dialog->useRegex(),
                          dialog->caseSensitive(), dialog->wholeWord(), true, true);
    });

    connect(dialog, &FindReplaceDialog::replaceAll, this, [this, dialog, editor]() {
        QString findText = dialog->findText();
        QString replaceText = dialog->replaceText();
        if (findText.isEmpty()) return;
        if (!dialog->useRegex() && replaceText.contains(findText,
                                                        dialog->caseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Replace All",
                                 "Replacement text contains search text. This would cause an infinite loop.");
            return;
        }
        int count = 0;
        editor->setCursorPosition(0, 0);
        int lastLine = -1, lastCol = -1;
        while (editor->findFirst(findText, dialog->useRegex(),
                                 dialog->caseSensitive(), dialog->wholeWord(), false, true)) {
            int line, col;
            editor->getCursorPosition(&line, &col);
            if (line == lastLine && col == lastCol) break;
            lastLine = line; lastCol = col;
            editor->replaceSelectedText(replaceText);
            if (++count > 10000) {
                QMessageBox::warning(this, "Replace All",
                                     QString("Stopped after %1 replacements (safety limit).").arg(count));
                return;
            }
        }
        QMessageBox::information(this, "Replace All",
                                 QString("Replaced %1 occurrence(s).").arg(count));
    });
}

void MainWindow::onEditGotoLine()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) return;
    GotoLineDialog dialog(editor->lines(), this);
    if (dialog.exec() == QDialog::Accepted)
        editor->gotoLine(dialog.lineNumber());
}

// ─────────────────────────────────────────────────────────────────────────────
// Build menu slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onBuildCompile()
{
    QString sourceFile = getCurrentSourceFile();
    if (sourceFile.isEmpty()) {
        showBuildError("No file to compile. Please save your file first.");
        return;
    }
    QString compilerId = m_compilerCombo->currentData().toString();
    auto compiler = CompilerRegistry::instance().getCompiler(compilerId);
    if (!compiler) { showBuildError("No compiler selected."); return; }

    CompileRequest request;
    request.sourceFile         = sourceFile;
    request.outputFile         = getExecutablePath(sourceFile);
    request.standard           = m_standardCombo->currentText();
    request.additionalFlags    = QStringList() << "-Wall" << "-Wextra";
    request.optimizationEnabled = false;
    request.optLevel           = OptimizationLevel::O0;

    m_outputPanel->terminal()->clear();
    m_outputPanel->showTerminalTab();
    m_outputPanel->problems()->clear();
    m_statusLabel->setText("Building...");

    CompileResult result = compiler->compile(request);
    Theme theme = ThemeManager::instance()->currentTheme();

    if (!result.rawOutput.isEmpty())
        m_outputPanel->terminal()->appendText(result.rawOutput, theme.textPrimary);
    if (!result.rawError.isEmpty())
        m_outputPanel->terminal()->appendText(result.rawError, QColor("#F48771"));

    {
        const QString sourceDir = QFileInfo(sourceFile).absolutePath();
        QList<DiagnosticMessage> resolved = result.diagnostics;
        for (DiagnosticMessage& diag : resolved) {
            if (!diag.file.isEmpty() && !QFileInfo(diag.file).isAbsolute()) {
                const QString abs = QDir(sourceDir).absoluteFilePath(diag.file);
                if (QFileInfo::exists(abs))
                    diag.file = abs;
                else
                    diag.file = sourceDir + QDir::separator() + diag.file;
            }
        }
        m_outputPanel->problems()->setDiagnostics(resolved);
    }

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

void MainWindow::onBuildRun()
{
    if (m_currentExecutable.isEmpty() || !QFile::exists(m_currentExecutable)) {
        QMessageBox::warning(this, "Error", "No executable to run. Please build first.");
        return;
    }
    m_outputPanel->showTerminalTab();
    m_outputPanel->terminal()->runCommand(m_currentExecutable);
    m_statusLabel->setText("Running...");
}

void MainWindow::onBuildCompileAndRun()
{
    onBuildCompile();
    if (!m_currentExecutable.isEmpty() && QFile::exists(m_currentExecutable))
        onBuildRun();
}

void MainWindow::onBuildStop()
{
    if (m_outputPanel->terminal()->isRunning()) {
        m_outputPanel->terminal()->stopProcess();
        m_statusLabel->setText("Program stopped");
    }
}

void MainWindow::onBuildClean()
{
    if (!m_currentExecutable.isEmpty() && QFile::exists(m_currentExecutable)) {
        QFile::remove(m_currentExecutable);
        m_currentExecutable.clear();
        m_statusLabel->setText("Clean complete");
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Toolbar slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onCompilerChanged(int) { updateStatusBar(); }
void MainWindow::onStandardChanged(int) { updateStatusBar(); }

// ─────────────────────────────────────────────────────────────────────────────
// File tree slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onFileTreeDoubleClicked(const QString& filePath) { m_editorTabs->openFile(filePath); }

void MainWindow::onNewFileRequested(const QString& directory)
{
    NewFileDialog dialog(directory, this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.createdFiles();
        for (const QString& file : files) m_editorTabs->openFile(file);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Editor slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onEditorChanged(CodeEditor* editor)
{
    updateStatusBar();
    updateWindowTitle();

    if (editor) {
        connect(editor, &CodeEditor::cursorPositionChanged,
                this, &MainWindow::updateStatusBar, Qt::UniqueConnection);
        connect(editor, &CodeEditor::modificationChanged,
                this, &MainWindow::updateWindowTitle, Qt::UniqueConnection);

        if (m_analysisPanel)
            m_analysisPanel->setSourceCode(editor->text(), editor->filePath());

        // QPointer auto-nulls when editor is deleted, so this is safe
        if (m_previousEditor && m_previousEditor != editor)
            disconnect(m_previousEditor, &QsciScintilla::textChanged,
                       this, &MainWindow::onActiveEditorTextChanged);
        m_previousEditor = editor;
        connect(editor, &QsciScintilla::textChanged,
                this, &MainWindow::onActiveEditorTextChanged, Qt::UniqueConnection);
    } else {
        // No editor open — clear the previous editor reference
        m_previousEditor = nullptr;
        if (m_analysisPanel)
            m_analysisPanel->setSourceCode(QString(), QString());
    }
}

void MainWindow::onActiveEditorTextChanged()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor && m_analysisPanel)
        m_analysisPanel->setSourceCode(editor->text(), editor->filePath());
}

// ─────────────────────────────────────────────────────────────────────────────
// Problems → navigate to exact error location (file + line + column)
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onDiagnosticClicked(const QString& file, int line, int column)
{
    QString resolvedFile = file;

    if (!QFileInfo(file).isAbsolute()) {
        QStringList searchDirs;
        if (!m_currentExecutable.isEmpty())
            searchDirs << QFileInfo(m_currentExecutable).absolutePath();
        if (!m_fileTree->rootPath().isEmpty())
            searchDirs << m_fileTree->rootPath();
        for (int i = 0; i < m_editorTabs->count(); ++i) {
            CodeEditor* ed = m_editorTabs->editorAt(i);
            if (ed && !ed->filePath().isEmpty())
                searchDirs << QFileInfo(ed->filePath()).absolutePath();
        }

        searchDirs.removeDuplicates();
        for (const QString& dir : searchDirs) {
            QString candidate = QDir(dir).absoluteFilePath(file);
            if (QFileInfo::exists(candidate)) {
                resolvedFile = candidate;
                break;
            }
        }
    }

    CodeEditor* editor = nullptr;
    for (int i = 0; i < m_editorTabs->count(); ++i) {
        CodeEditor* ed = m_editorTabs->editorAt(i);
        if (ed && (ed->filePath() == resolvedFile || ed->filePath() == file)) {
            m_editorTabs->setCurrentIndex(i);
            editor = ed;
            break;
        }
    }

    if (!editor && QFileInfo::exists(resolvedFile))
        editor = m_editorTabs->openFile(resolvedFile);
    if (!editor && resolvedFile != file && QFileInfo::exists(file))
        editor = m_editorTabs->openFile(file);

    if (editor) {
        editor->gotoLine(line);
        editor->setCursorPosition(line - 1, qMax(0, column - 1));
        editor->setFocus();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Window dragging / native resize
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
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

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
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

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    m_dragging = false;
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    const int buttonContainerWidth = WINDOW_BUTTON_WIDTH * WINDOW_BUTTON_COUNT;
    if (event->pos().y() < TITLE_BAR_HEIGHT && event->pos().x() < width() - buttonContainerWidth) {
        if (isMaximized()) { showNormal(); m_maximizeBtn->setText("□"); }
        else               { showMaximized(); m_maximizeBtn->setText("❐"); }
        event->accept();
        return;
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#else
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
#endif
    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_NCHITTEST) {
            RECT winrect;
            GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
            int x = GET_X_LPARAM(msg->lParam);
            int y = GET_Y_LPARAM(msg->lParam);
            if (x < winrect.left  + RESIZE_BORDER_WIDTH && y < winrect.top    + RESIZE_BORDER_WIDTH) { *result = HTTOPLEFT;     return true; }
            if (x > winrect.right - RESIZE_BORDER_WIDTH && y < winrect.top    + RESIZE_BORDER_WIDTH) { *result = HTTOPRIGHT;    return true; }
            if (x < winrect.left  + RESIZE_BORDER_WIDTH && y > winrect.bottom - RESIZE_BORDER_WIDTH) { *result = HTBOTTOMLEFT;  return true; }
            if (x > winrect.right - RESIZE_BORDER_WIDTH && y > winrect.bottom - RESIZE_BORDER_WIDTH) { *result = HTBOTTOMRIGHT; return true; }
            if (x < winrect.left  + RESIZE_BORDER_WIDTH) { *result = HTLEFT;   return true; }
            if (x > winrect.right - RESIZE_BORDER_WIDTH) { *result = HTRIGHT;  return true; }
            if (y < winrect.top   + RESIZE_BORDER_WIDTH) { *result = HTTOP;    return true; }
            if (y > winrect.bottom - RESIZE_BORDER_WIDTH) { *result = HTBOTTOM; return true; }
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

// ─────────────────────────────────────────────────────────────────────────────
// Helper methods
// ─────────────────────────────────────────────────────────────────────────────
QString MainWindow::getCurrentSourceFile()
{
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (!editor) return QString();
    QString filePath = editor->filePath();
    if (filePath.isEmpty()) {
        if (!m_editorTabs->saveCurrentFileAs()) return QString();
        filePath = editor->filePath();
    } else if (editor->isModified()) {
        if (!m_editorTabs->saveCurrentFile()) return QString();
    }
    return filePath;
}

QString MainWindow::getExecutablePath(const QString& sourceFile)
{
    QFileInfo info(sourceFile);
    QString baseName = info.completeBaseName();
#ifdef Q_OS_WIN
    return info.absolutePath() + "/" + baseName + ".exe";
#else
    return info.absolutePath() + "/" + baseName;
#endif
}

void MainWindow::showBuildError(const QString& message)
{
    m_outputPanel->showTerminalTab();
    m_outputPanel->terminal()->appendText(message + "\n", QColor("#F44747"));
    m_statusLabel->setText("Build error");
}

void MainWindow::saveCurrentSession()
{
    if (!ProjectManager::instance()->hasOpenProject()) return;
    auto project = ProjectManager::instance()->currentProject();
    QStringList openFiles;
    for (int i = 0; i < m_editorTabs->count(); ++i) {
        CodeEditor* editor = m_editorTabs->editorAt(i);
        if (editor && !editor->filePath().isEmpty())
            openFiles.append(editor->filePath());
    }
    QString activeFile;
    if (m_editorTabs->currentEditor())
        activeFile = m_editorTabs->currentEditor()->filePath();
    project->saveSession(openFiles, activeFile, QStringList());
    project->save();
}

void MainWindow::showProjectLoadError(Project::LoadResult result)
{
    QString message;
    switch (result) {
    case Project::LoadResult::FileNotFound:    message = "Project file not found."; break;
    case Project::LoadResult::InvalidFormat:   message = "Invalid project file format."; break;
    case Project::LoadResult::VersionMismatch: message = "Unsupported project file version."; break;
    case Project::LoadResult::PermissionDenied: message = "Permission denied when reading project file."; break;
    default: message = "Failed to load project."; break;
    }
    QMessageBox::warning(this, "Error", message);
}

void MainWindow::restoreProjectSession(Project* project)
{
    for (const QString& file : project->openFiles()) {
        QString fullPath = QDir(project->projectDirectory()).absoluteFilePath(file);
        if (QFileInfo::exists(fullPath)) m_editorTabs->openFile(fullPath);
        else if (QFileInfo::exists(file)) m_editorTabs->openFile(file);
    }
    if (!project->activeFile().isEmpty()) {
        QString activePath = QDir(project->projectDirectory()).absoluteFilePath(project->activeFile());
        if (QFileInfo::exists(activePath)) m_editorTabs->openFile(activePath);
        else if (QFileInfo::exists(project->activeFile())) m_editorTabs->openFile(project->activeFile());
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// About dialog
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::showAboutDialog()
{
    Qt::WindowFlags flags = Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint
                            | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowMaximizeButtonHint;
#ifdef Q_OS_WIN
    flags |= Qt::MSWindowsFixedSizeDialogHint;
#endif
    QDialog* dlg = new QDialog(this, flags);
    dlg->setWindowTitle("About CppAtlas");
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowModality(Qt::NonModal);

    QVBoxLayout* layout = new QVBoxLayout(dlg);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    QLabel* logoLabel = new QLabel("C++", dlg);
    logoLabel->setObjectName("aboutLogo");
    logoLabel->setAlignment(Qt::AlignHCenter);
    QFont logoFont = logoLabel->font(); logoFont.setPointSize(28); logoFont.setBold(true);
    logoLabel->setFont(logoFont);
    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);

    QLabel* appNameLabel = new QLabel("CppAtlas", dlg);
    appNameLabel->setObjectName("aboutAppName");
    appNameLabel->setAlignment(Qt::AlignHCenter);
    QFont appFont = appNameLabel->font(); appFont.setPointSize(16); appFont.setBold(true);
    appNameLabel->setFont(appFont);
    layout->addWidget(appNameLabel, 0, Qt::AlignHCenter);

    QLabel* versionLabel = new QLabel(
        QString("Version %1").arg(QApplication::applicationVersion()), dlg);
    versionLabel->setObjectName("aboutVersion");
    versionLabel->setAlignment(Qt::AlignHCenter);
    layout->addWidget(versionLabel, 0, Qt::AlignHCenter);

    QFrame* sep = new QFrame(dlg);
    sep->setFrameShape(QFrame::HLine);
    layout->addWidget(sep);

    QLabel* descLabel = new QLabel(
        "CppAtlas — C++ Learning IDE\n\nAn educational Qt-based environment for C++ development and learning.\n\nBuilt with Qt and QScintilla.",
        dlg);
    descLabel->setObjectName("aboutDesc");
    descLabel->setAlignment(Qt::AlignHCenter);
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    QPushButton* githubBtn = new QPushButton("View on GitHub", dlg);
    connect(githubBtn, &QPushButton::clicked, dlg, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/0xV4h3/cpp-atlas"));
    });
    layout->addWidget(githubBtn, 0, Qt::AlignHCenter);

    QPushButton* closeBtn = new QPushButton("Close", dlg);
    connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignHCenter);

    dlg->setFixedSize(410, dlg->layout()->sizeHint().height());
    dlg->show();
}

// ─────────────────────────────────────────────────────────────────────────────
// Admin panel
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setStartupAdminRequested(bool enabled) { m_startupAdminRequested = enabled; }

void MainWindow::tryOpenAdminPanelFromStartupRequest()
{
    if (m_startupAdminRequested) showQuizAdminPanel();
}

void MainWindow::showQuizAdminPanel()
{
#ifdef QT_DEBUG
    const bool requireReleasePassword = false;
#else
    const bool requireReleasePassword = true;
#endif
    const AdminAccessResult result =
        AdminAccessController::instance().verifyAccess(this, requireReleasePassword);

    switch (result) {
    case AdminAccessResult::Granted:
        if (!m_adminPanel) {
            m_adminPanel = new QuizAdminPanel(this);
            m_adminPanel->setAttribute(Qt::WA_DeleteOnClose);
            connect(m_adminPanel, &QObject::destroyed, this, [this]() { m_adminPanel = nullptr; });
        }
        m_adminPanel->show();
        m_adminPanel->raise();
        m_adminPanel->activateWindow();
        break;
    case AdminAccessResult::NotLoggedIn:
        QMessageBox::warning(this, tr("Admin Access"),
                             tr("You must be logged in to access the admin panel."));
        break;
    case AdminAccessResult::NotAdmin:
        QMessageBox::warning(this, tr("Admin Access"),
                             tr("Your account does not have administrator privileges."));
        break;
    case AdminAccessResult::PasswordRejected:
        QMessageBox::warning(this, tr("Admin Access"),
                             tr("Admin password verification failed."));
        break;
    case AdminAccessResult::Error:
        QMessageBox::critical(this, tr("Admin Access"),
                              tr("Admin authentication is not configured."));
        break;
    }
}
