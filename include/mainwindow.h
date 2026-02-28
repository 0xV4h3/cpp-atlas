#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QPoint>
#include <QStackedWidget>
#include "core/Project.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class EditorTabWidget;
class OutputPanel;
class FileTreeWidget;
class FileManager;
class Project;
class ICompiler;
class WelcomeScreen;
class NewFileDialog;
class NewProjectDialog;
class AnalysisPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif

private slots:
    // File menu
    void onFileNew();
    void onFileNewProject();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileOpenFolder();
    void onFileOpenProject();
    void onFileExit();
    
    // Edit menu
    void onEditUndo();
    void onEditRedo();
    void onEditCut();
    void onEditCopy();
    void onEditPaste();
    void onEditFind();
    void onEditReplace();
    void onEditGotoLine();
    
    // Build menu
    void onBuildCompile();
    void onBuildRun();
    void onBuildCompileAndRun();
    void onBuildStop();
    void onBuildClean();
    
    // View menu
    void onViewToggleFileTree();
    void onViewToggleOutputPanel();
    void onViewFullscreen();
    
    // Toolbar
    void onCompilerChanged(int index);
    void onStandardChanged(int index);
    
    // File tree
    void onFileTreeDoubleClicked(const QString& filePath);
    void onNewFileRequested(const QString& directory);
    
    // Editor
    void onEditorChanged(class CodeEditor* editor);
    
    // Problems
    void onDiagnosticClicked(const QString& file, int line, int column);
    
private:
    Ui::MainWindow *ui;
    
    // Title bar constants
    static constexpr int TITLE_BAR_HEIGHT = 32;
    static constexpr int WINDOW_BUTTON_WIDTH = 46;
    static constexpr int WINDOW_BUTTON_COUNT = 3;
    static constexpr int RESIZE_BORDER_WIDTH = 8;
    
    // Custom title bar widgets
    QWidget* m_titleBar = nullptr;
    QLabel* m_titleLabel = nullptr;
    QLabel* m_iconLabel = nullptr;
    QPushButton* m_minimizeBtn = nullptr;
    QPushButton* m_maximizeBtn = nullptr;
    QPushButton* m_closeBtn = nullptr;
    
    // Drag handling
    QPoint m_dragPosition;
    bool m_dragging = false;
    
    // Main widgets
    EditorTabWidget* m_editorTabs;
    OutputPanel* m_outputPanel;
    FileTreeWidget* m_fileTree;
    FileManager* m_fileManager;
    Project* m_project;
    AnalysisPanel* m_analysisPanel = nullptr;
    
    // Welcome screen
    WelcomeScreen* m_welcomeScreen = nullptr;
    QStackedWidget* m_centralStack = nullptr;
    bool m_analysisDockWasVisible = false;
    
    // Dock widgets
    QDockWidget* m_fileTreeDock;
    QDockWidget* m_outputPanelDock;
    QDockWidget* m_analysisDock = nullptr;
    
    // Toolbar widgets
    QComboBox* m_compilerCombo;
    QComboBox* m_standardCombo;
    
    // Status bar
    QLabel* m_statusLabel;
    QLabel* m_cursorPosLabel;
    QLabel* m_standardLabel;
    QLabel* m_compilerLabel;
    
    // Current compilation
    QString m_currentExecutable;
    
    // Menus
    QMenu* m_buildMenu = nullptr;
    QMenu* m_editMenu = nullptr;
    QMenu* m_viewMenu = nullptr;
    QMenu* m_toolsMenu = nullptr;
    QAction* m_runAction = nullptr;
    QAction* m_toggleFileTreeAction = nullptr;
    QAction* m_toggleOutputAction = nullptr;
    QAction* m_toggleAnalysisAction = nullptr;
    
    // Edit menu actions that should be disabled in welcome screen
    QAction* m_findAction = nullptr;
    QAction* m_replaceAction = nullptr;
    QAction* m_gotoLineAction = nullptr;
    
    // Main toolbar
    QToolBar* m_mainToolbar = nullptr;
    
    void setupUi();
    void setupCustomTitleBar();
    void setupMenus();
    void setupToolbar();
    void setupDockWidgets();
    void setupStatusBar();
    void setupConnections();
    void setupWelcomeScreen();
    void showWelcomeScreen();
    void hideWelcomeScreen();
    void updateMenuState(bool isWelcomeVisible);
    void loadCompilers();
    void updateStatusBar();
    void updateWindowTitle();
    void updateCustomTitleLabel(const QString& title);
    void saveCurrentSession();
    void showProjectLoadError(Project::LoadResult result);
    void restoreProjectSession(Project* project);
    
    QString getCurrentSourceFile();
    QString getExecutablePath(const QString& sourceFile);
    void showBuildError(const QString& message);
};

#endif // MAINWINDOW_H
