#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QMenu>

/**
 * @brief File tree widget for browsing project files
 */
class FileTreeWidget : public QTreeView {
    Q_OBJECT
    
public:
    explicit FileTreeWidget(QWidget *parent = nullptr);
    ~FileTreeWidget() override = default;
    
    /**
     * @brief Set root directory to display
     * @param path Directory path
     */
    void setRootPath(const QString& path);
    
    /**
     * @brief Get current root directory
     * @return Directory path
     */
    QString rootPath() const;
    
    /**
     * @brief Open folder and show file tree
     * @param path Directory path
     */
    void openFolder(const QString& path);
    
    /**
     * @brief Close folder and hide file tree
     */
    void closeFolder();
    
signals:
    void fileDoubleClicked(const QString& filePath);
    void newFileRequested(const QString& directory);
    void deleteFileRequested(const QString& filePath);
    void renameFileRequested(const QString& filePath);
    void fileRenamed(const QString& oldPath, const QString& newPath);
    void fileDeleted(const QString& filePath);
    void folderOpened(const QString& path);
    void folderClosed();
    
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    
private slots:
    void onNewFileAction();
    void onDeleteAction();
    void onRenameAction();
    void onOpenInExplorerAction();
    
private:
    QFileSystemModel* m_model;
    QMenu* m_contextMenu;
    QAction* m_deleteAction;
    QAction* m_renameAction;
    QAction* m_openInExplorerAction;
    QModelIndex m_contextMenuIndex;
    
    void setupModel();
    void setupContextMenu();
    QString selectedFilePath() const;
};

#endif // FILETREEWIDGET_H
