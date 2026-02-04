#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QProcess>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File menu
    void onFileNew();
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
    
    // Main widgets
    EditorTabWidget* m_editorTabs;
    OutputPanel* m_outputPanel;
    FileTreeWidget* m_fileTree;
    FileManager* m_fileManager;
    Project* m_project;
    
    // Dock widgets
    QDockWidget* m_fileTreeDock;
    QDockWidget* m_outputPanelDock;
    
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
    
    void setupUi();
    void setupMenus();
    void setupToolbar();
    void setupDockWidgets();
    void setupStatusBar();
    void setupConnections();
    void loadCompilers();
    void updateStatusBar();
    void updateWindowTitle();
    
    QString getCurrentSourceFile();
    QString getExecutablePath(const QString& sourceFile);
    void showBuildError(const QString& message);
};

#endif // MAINWINDOW_H
