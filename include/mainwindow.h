#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QLabel>
#include <QComboBox>
#include <QToolBar>
#include <QDockWidget>
#include <QMenu>
#include <QAction>
#include <QShortcut>
#include <QPointer>
#include <QPoint>
#include <QPushButton>

#include "core/Project.h"
#include "ui/FindReplaceDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EditorTabWidget;
class CodeEditor;
class OutputPanel;
class FileTreeWidget;
class AnalysisPanel;
class QuizModeWindow;
class WelcomeScreen;
class FileManager;
class SettingsDialog;
class QuizAdminPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStartupAdminRequested(bool enabled);

public slots:
    void tryOpenAdminPanelFromStartupRequest();
    void showWelcomeScreen();

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
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
    void onFileCloseProject();
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
    void onViewToggleFileTreeSide();
    void onViewToggleEditor();
    void onViewToggleToolbar();

    // Tools / Settings
    void onOpenSettings();
    void onSettingsChanged();

    // Toolbar
    void onCompilerChanged(int index);
    void onStandardChanged(int index);

    // File tree
    void onFileTreeDoubleClicked(const QString& filePath);
    void onNewFileRequested(const QString& directory);

    // Editor
    void onEditorChanged(CodeEditor* editor);
    void onActiveEditorTextChanged();

    // Problems
    void onDiagnosticClicked(const QString& file, int line, int column);

    // Quiz / Welcome
    void onQuizModeRequested();
    void onQuizModeExit();

    // Admin
    void showQuizAdminPanel();

    // Output panel full-height toggle
    void onToggleOutputFullHeight();

private:
    void setupUi();
    void setupCustomTitleBar();
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void setupConnections();
    void setupWelcomeScreen();

    void showIDE();
    void hideWelcomeScreen();
    void showQuizModeWindow();
    void hideQuizModeWindow();
    void updateMenuState(bool isWelcomeOrQuiz);
    void updateWindowTitle();
    void updateCustomTitleLabel(const QString& title);
    void updateTitlePosition();
    void updateStatusBar();
    void loadCompilers();

    QString getCurrentSourceFile();
    QString getExecutablePath(const QString& sourceFile);
    void showBuildError(const QString& message);
    void saveCurrentSession();
    void restoreProjectSession(Project* project);
    void showProjectLoadError(Project::LoadResult result);
    void showAboutDialog();
    void wireFindReplaceDialog(FindReplaceDialog* dialog, CodeEditor* editor);

    // Constants
    static constexpr int TITLE_BAR_HEIGHT    = 32;
    static constexpr int WINDOW_BUTTON_WIDTH = 46;
    static constexpr int WINDOW_BUTTON_COUNT = 3;
    static constexpr int RESIZE_BORDER_WIDTH = 8;

    // UI
    Ui::MainWindow *ui;

    // Custom title bar
    QWidget*      m_titleBar        = nullptr;
    QLabel*       m_iconLabel       = nullptr;
    QLabel*       m_titleLabel      = nullptr;
    QLabel*       m_titleLabelLeft  = nullptr;
    QPushButton*  m_minimizeBtn     = nullptr;
    QPushButton*  m_maximizeBtn     = nullptr;
    QPushButton*  m_closeBtn        = nullptr;

    // Main layout
    QStackedWidget*   m_modeStack       = nullptr;
    QSplitter*        m_mainSplitter    = nullptr;
    QSplitter*        m_vertSplitter    = nullptr;

    // Widgets
    EditorTabWidget*  m_editorTabs      = nullptr;
    AnalysisPanel*    m_analysisPanel   = nullptr;
    OutputPanel*      m_outputPanel     = nullptr;
    FileTreeWidget*   m_fileTree        = nullptr;
    WelcomeScreen*    m_welcomeScreen   = nullptr;
    QuizModeWindow*   m_quizModeWindow  = nullptr;

    // File tree
    QSplitter*        m_horizSplitter   = nullptr;

    // Toolbar
    QToolBar*   m_mainToolbar    = nullptr;
    QComboBox*  m_compilerCombo  = nullptr;
    QComboBox*  m_standardCombo  = nullptr;

    // Status bar
    QLabel* m_statusLabel     = nullptr;
    QLabel* m_cursorPosLabel  = nullptr;
    QLabel* m_standardLabel   = nullptr;
    QLabel* m_compilerLabel   = nullptr;

    // Menus
    QMenu*    m_fileMenu               = nullptr;
    QMenu*    m_editMenu               = nullptr;
    QMenu*    m_buildMenu              = nullptr;
    QMenu*    m_viewMenu               = nullptr;
    QMenu*    m_toolsMenu              = nullptr;
    QMenu*    m_settingsMenu           = nullptr;
    QMenu*    m_helpMenu               = nullptr;
    QAction*  m_toggleFileTreeAction   = nullptr;
    QAction*  m_toggleOutputAction     = nullptr;
    QAction*  m_toggleAnalysisAction   = nullptr;
    QAction*  m_findAction             = nullptr;
    QAction*  m_replaceAction          = nullptr;
    QAction*  m_gotoLineAction         = nullptr;
    QAction*  m_runAction              = nullptr;
    QAction*  m_closeProjectAction     = nullptr;
    QAction*  m_fileTreeSideAction     = nullptr;
    QAction*  m_outputFullHeightAction = nullptr;
    QAction*  m_toggleEditorAction     = nullptr;
    QAction*  m_toggleToolbarAction    = nullptr;

    // State
    FileManager*  m_fileManager      = nullptr;
    Project*      m_project          = nullptr;
    QString       m_currentExecutable;
    bool          m_dragging         = false;
    QPoint        m_dragPosition;
    bool          m_fileTreeOnLeft   = true;
    bool          m_outputFullHeight = false;
    int           m_lastOutputHeight  = 0;
    int           m_lastFileTreeWidth = 200;
    int           m_lastEditorWidth   = 0;
    QPointer<CodeEditor> m_previousEditor;
    bool          m_startupAdminRequested = false;

    // Dialogs
    SettingsDialog*  m_settingsDialog = nullptr;
    QuizAdminPanel*  m_adminPanel     = nullptr;
    QShortcut*       m_adminShortcut  = nullptr;
};
