#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <QTabWidget>
#include <QMap>

class CodeEditor;

/**
 * @brief Tab widget for managing multiple code editors
 */
class EditorTabWidget : public QTabWidget {
    Q_OBJECT
    
public:
    explicit EditorTabWidget(QWidget *parent = nullptr);
    ~EditorTabWidget() override = default;
    
    /**
     * @brief Create new empty file tab
     * @return Pointer to new editor
     */
    CodeEditor* newFile();
    
    /**
     * @brief Open file in new tab
     * @param filePath Path to file
     * @return Pointer to editor, or nullptr if failed
     */
    CodeEditor* openFile(const QString& filePath);
    
    /**
     * @brief Close tab at index
     * @param index Tab index
     * @return true if closed, false if cancelled
     */
    bool closeTab(int index);
    
    /**
     * @brief Get current active editor
     * @return Pointer to current editor, or nullptr if none
     */
    CodeEditor* currentEditor() const;
    
    /**
     * @brief Get editor at index
     * @param index Tab index
     * @return Pointer to editor, or nullptr if invalid index
     */
    CodeEditor* editorAt(int index) const;
    
    /**
     * @brief Save current editor
     * @return true if successful
     */
    bool saveCurrentFile();
    
    /**
     * @brief Save current editor with new name
     * @return true if successful
     */
    bool saveCurrentFileAs();
    
    /**
     * @brief Close all tabs
     * @return true if all closed, false if any cancelled
     */
    bool closeAll();
    
    /**
     * @brief Close tab for a specific file path
     * @param filePath File path to close
     */
    void closeFileTab(const QString& filePath);
    
    /**
     * @brief Update file path for an open tab (e.g., after rename)
     * @param oldPath Old file path
     * @param newPath New file path
     */
    void updateFilePath(const QString& oldPath, const QString& newPath);
    
signals:
    void editorChanged(CodeEditor* editor);
    void fileOpened(const QString& filePath);
    void fileSaved(const QString& filePath);
    
private slots:
    void onTabChanged(int index);
    void onTabCloseRequested(int index);
    void onEditorModificationChanged(bool modified);
    
private:
    int m_newFileCounter = 1;
    
    /**
     * @brief Update tab title for editor
     * @param editor Editor widget
     */
    void updateTabTitle(CodeEditor* editor);
    
    /**
     * @brief Get short filename from path
     * @param filePath Full file path
     * @return Short filename
     */
    QString shortFileName(const QString& filePath) const;
};

#endif // EDITORTABWIDGET_H
