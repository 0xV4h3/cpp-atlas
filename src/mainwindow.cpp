#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor/EditorTabWidget.h"
#include "editor/CodeEditor.h"
#include "output/OutputPanel.h"
#include "output/BuildOutputWidget.h"
#include "output/RunOutputWidget.h"
#include "output/ProblemsWidget.h"
#include "ui/FileTreeWidget.h"
#include "core/FileManager.h"
#include "core/Project.h"
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
#include <QInputDialog>
#include <QStandardPaths>
#include <QTemporaryDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Initialize components
    m_fileManager = new FileManager(this);
    m_project = new Project(this);
    
    setupUi();
    setupMenus();
    setupToolbar();
    setupDockWidgets();
    setupStatusBar();
    setupConnections();
    
    // Auto-scan for compilers
    CompilerRegistry::instance().autoScanCompilers();
    loadCompilers();
    
    // Set default directory
    m_fileTree->setRootPath(QDir::currentPath());
    
    // Open a default file
    m_editorTabs->newFile();
    
    updateWindowTitle();
    
    // Restore window state
    QSettings settings("CppAtlas", "CppAtlas");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
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
    
    // Create central widget - editor tabs
    m_editorTabs = new EditorTabWidget(this);
    setCentralWidget(m_editorTabs);
}

void MainWindow::setupMenus() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    QAction* newAction = fileMenu->addAction("&New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onFileNew);
    
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
    
    QAction* openProjectAction = fileMenu->addAction("Open &Project...");
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::onFileOpenProject);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onFileExit);
    
    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    
    QAction* undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onEditUndo);
    
    QAction* redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onEditRedo);
    
    editMenu->addSeparator();
    
    QAction* cutAction = editMenu->addAction("Cu&t");
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, this, &MainWindow::onEditCut);
    
    QAction* copyAction = editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onEditCopy);
    
    QAction* pasteAction = editMenu->addAction("&Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onEditPaste);
    
    editMenu->addSeparator();
    
    QAction* findAction = editMenu->addAction("&Find...");
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, &QAction::triggered, this, &MainWindow::onEditFind);
    
    QAction* replaceAction = editMenu->addAction("&Replace...");
    replaceAction->setShortcut(QKeySequence::Replace);
    connect(replaceAction, &QAction::triggered, this, &MainWindow::onEditReplace);
    
    // Build menu
    QMenu* buildMenu = menuBar()->addMenu("&Build");
    
    QAction* compileAction = buildMenu->addAction("&Build");
    compileAction->setShortcut(Qt::Key_F7);
    connect(compileAction, &QAction::triggered, this, &MainWindow::onBuildCompile);
    
    QAction* runAction = buildMenu->addAction("&Run");
    runAction->setShortcut(Qt::Key_F5);
    connect(runAction, &QAction::triggered, this, &MainWindow::onBuildRun);
    
    QAction* compileAndRunAction = buildMenu->addAction("Build && R&un");
    compileAndRunAction->setShortcut(Qt::CTRL | Qt::Key_F5);
    connect(compileAndRunAction, &QAction::triggered, this, &MainWindow::onBuildCompileAndRun);
    
    buildMenu->addSeparator();
    
    QAction* stopAction = buildMenu->addAction("&Stop");
    stopAction->setShortcut(Qt::SHIFT | Qt::Key_F5);
    connect(stopAction, &QAction::triggered, this, &MainWindow::onBuildStop);
    
    QAction* cleanAction = buildMenu->addAction("&Clean");
    connect(cleanAction, &QAction::triggered, this, &MainWindow::onBuildClean);
    
    // View menu
    QMenu* viewMenu = menuBar()->addMenu("&View");
    
    QAction* toggleFileTreeAction = viewMenu->addAction("Toggle &File Tree");
    connect(toggleFileTreeAction, &QAction::triggered, this, &MainWindow::onViewToggleFileTree);
    
    QAction* toggleOutputAction = viewMenu->addAction("Toggle &Output Panel");
    connect(toggleOutputAction, &QAction::triggered, this, &MainWindow::onViewToggleOutputPanel);
    
    viewMenu->addSeparator();
    
    QAction* fullscreenAction = viewMenu->addAction("&Fullscreen");
    fullscreenAction->setShortcut(Qt::Key_F11);
    connect(fullscreenAction, &QAction::triggered, this, &MainWindow::onViewFullscreen);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About CppAtlas",
            "CppAtlas - C++ Learning IDE\n\n"
            "Version 0.1\n\n"
            "An educational Qt-based environment for learning C++.");
    });
}

void MainWindow::setupToolbar() {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    
    // New, Open, Save buttons
    toolbar->addAction("New", this, &MainWindow::onFileNew);
    toolbar->addAction("Open", this, &MainWindow::onFileOpen);
    toolbar->addAction("Save", this, &MainWindow::onFileSave);
    
    toolbar->addSeparator();
    
    // Build, Run, Stop buttons
    toolbar->addAction("Build", this, &MainWindow::onBuildCompile);
    toolbar->addAction("Run", this, &MainWindow::onBuildRun);
    toolbar->addAction("Stop", this, &MainWindow::onBuildStop);
    
    toolbar->addSeparator();
    
    // Compiler selection
    toolbar->addWidget(new QLabel(" Compiler: "));
    m_compilerCombo = new QComboBox(toolbar);
    m_compilerCombo->setMinimumWidth(150);
    toolbar->addWidget(m_compilerCombo);
    
    // Standard selection
    toolbar->addWidget(new QLabel(" Standard: "));
    m_standardCombo = new QComboBox(toolbar);
    m_standardCombo->addItems({"c++11", "c++14", "c++17", "c++20", "c++23"});
    m_standardCombo->setCurrentText("c++17");
    toolbar->addWidget(m_standardCombo);
    
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
    
    // Problems widget signals
    connect(m_outputPanel->problems(), &ProblemsWidget::diagnosticClicked,
            this, &MainWindow::onDiagnosticClicked);
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
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_editorTabs->closeAll()) {
        event->accept();
    } else {
        event->ignore();
    }
}

// File menu slots
void MainWindow::onFileNew() {
    m_editorTabs->newFile();
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

void MainWindow::onFileOpenProject() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Project",
        QString(),
        "CppAtlas Project (*.cppatlas);;All Files (*)"
    );
    
    if (!filePath.isEmpty()) {
        if (m_project->load(filePath)) {
            m_fileTree->setRootPath(m_project->directory());
            m_statusLabel->setText("Project loaded: " + m_project->name());
        } else {
            QMessageBox::warning(this, "Error", "Failed to load project.");
        }
    }
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
    if (editor) {
        // TODO: Implement find dialog
        QMessageBox::information(this, "Find", "Find functionality not yet implemented.");
    }
}

void MainWindow::onEditReplace() {
    CodeEditor* editor = m_editorTabs->currentEditor();
    if (editor) {
        // TODO: Implement replace dialog
        QMessageBox::information(this, "Replace", "Replace functionality not yet implemented.");
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
    
    // Clear previous output
    m_outputPanel->buildOutput()->clear();
    m_outputPanel->showBuildTab();
    m_outputPanel->problems()->clear();
    
    m_statusLabel->setText("Building...");
    
    // Compile
    CompileResult result = compiler->compile(request);
    
    // Show output
    if (!result.rawOutput.isEmpty()) {
        m_outputPanel->buildOutput()->appendText(result.rawOutput);
    }
    if (!result.rawError.isEmpty()) {
        m_outputPanel->buildOutput()->appendError(result.rawError);
    }
    
    // Show diagnostics
    m_outputPanel->problems()->setDiagnostics(result.diagnostics);
    
    if (result.success) {
        m_currentExecutable = result.outputFile;
        m_statusLabel->setText(QString("Build succeeded (%1 ms)").arg(result.compilationTimeMs));
        m_outputPanel->buildOutput()->appendText("\nBuild succeeded!");
    } else {
        m_statusLabel->setText("Build failed");
        m_outputPanel->buildOutput()->appendError("\nBuild failed!");
        m_outputPanel->showProblemsTab();
    }
}

void MainWindow::onBuildRun() {
    if (m_currentExecutable.isEmpty() || !QFile::exists(m_currentExecutable)) {
        QMessageBox::warning(this, "Error", 
            "No executable to run. Please build first.");
        return;
    }
    
    m_outputPanel->showRunTab();
    m_outputPanel->runOutput()->runProgram(m_currentExecutable);
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
    if (m_outputPanel->runOutput()->isRunning()) {
        m_outputPanel->runOutput()->stopProgram();
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
    bool ok;
    QString fileName = QInputDialog::getText(
        this,
        "New File",
        "Enter file name:",
        QLineEdit::Normal,
        "main.cpp",
        &ok
    );
    
    if (ok && !fileName.isEmpty()) {
        QString filePath = QDir(directory).filePath(fileName);
        
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "Error", "File already exists.");
            return;
        }
        
        if (m_fileManager->createNewFile(filePath)) {
            m_editorTabs->openFile(filePath);
        } else {
            QMessageBox::warning(this, "Error", "Failed to create file.");
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
    m_outputPanel->showBuildTab();
    m_outputPanel->buildOutput()->appendError(message);
    m_statusLabel->setText("Build error");
}
