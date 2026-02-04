#include "ui/FileTreeWidget.h"
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>

FileTreeWidget::FileTreeWidget(QWidget *parent)
    : QTreeView(parent)
{
    setupModel();
    setupContextMenu();
}

void FileTreeWidget::setupModel() {
    m_model = new QFileSystemModel(this);
    // DON'T call m_model->setRootPath(QDir::currentPath()) - let user open folder explicitly
    
    // Set name filters for relevant files
    QStringList filters;
    filters << "*.cpp" << "*.h" << "*.hpp" << "*.c" << "*.cc" << "*.cxx"
            << "*.txt" << "*.json" << "*.md" << "*.cmake" << "CMakeLists.txt";
    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);
    
    setModel(m_model);
    
    // Hide unnecessary columns
    hideColumn(1);  // Size
    hideColumn(2);  // Type
    hideColumn(3);  // Date Modified
    
    // Set header
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    
    // Initially hide the widget until folder is opened
    setVisible(false);
}

void FileTreeWidget::setupContextMenu() {
    m_contextMenu = new QMenu(this);
    
    m_newFileAction = m_contextMenu->addAction("New File...");
    m_contextMenu->addSeparator();
    m_deleteAction = m_contextMenu->addAction("Delete");
    m_renameAction = m_contextMenu->addAction("Rename...");
    m_contextMenu->addSeparator();
    m_openInExplorerAction = m_contextMenu->addAction("Open in File Explorer");
    
    connect(m_newFileAction, &QAction::triggered, this, &FileTreeWidget::onNewFileAction);
    connect(m_deleteAction, &QAction::triggered, this, &FileTreeWidget::onDeleteAction);
    connect(m_renameAction, &QAction::triggered, this, &FileTreeWidget::onRenameAction);
    connect(m_openInExplorerAction, &QAction::triggered, this, &FileTreeWidget::onOpenInExplorerAction);
}

void FileTreeWidget::setRootPath(const QString& path) {
    QModelIndex index = m_model->setRootPath(path);
    setRootIndex(index);
}

QString FileTreeWidget::rootPath() const {
    return m_model->rootPath();
}

void FileTreeWidget::openFolder(const QString& path) {
    QModelIndex index = m_model->setRootPath(path);
    setRootIndex(index);
    setVisible(true);
    emit folderOpened(path);
}

void FileTreeWidget::closeFolder() {
    setRootIndex(QModelIndex());
    setVisible(false);
    emit folderClosed();
}

void FileTreeWidget::contextMenuEvent(QContextMenuEvent *event) {
    m_contextMenuIndex = indexAt(event->pos());
    
    if (m_contextMenuIndex.isValid()) {
        QFileInfo info(m_model->filePath(m_contextMenuIndex));
        
        // Enable/disable actions based on selection
        m_newFileAction->setEnabled(info.isDir());
        m_deleteAction->setEnabled(true);
        m_renameAction->setEnabled(true);
        m_openInExplorerAction->setEnabled(true);
    } else {
        // Clicked on empty space
        m_newFileAction->setEnabled(true);
        m_deleteAction->setEnabled(false);
        m_renameAction->setEnabled(false);
        m_openInExplorerAction->setEnabled(true);
    }
    
    m_contextMenu->exec(event->globalPos());
}

void FileTreeWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        QString filePath = m_model->filePath(index);
        QFileInfo info(filePath);
        
        if (info.isFile()) {
            emit fileDoubleClicked(filePath);
            return;
        }
    }
    
    QTreeView::mouseDoubleClickEvent(event);
}

void FileTreeWidget::onNewFileAction() {
    QString directory;
    
    if (m_contextMenuIndex.isValid()) {
        QString path = m_model->filePath(m_contextMenuIndex);
        QFileInfo info(path);
        directory = info.isDir() ? path : info.absolutePath();
    } else {
        directory = m_model->rootPath();
    }
    
    emit newFileRequested(directory);
}

void FileTreeWidget::onDeleteAction() {
    if (!m_contextMenuIndex.isValid()) {
        return;
    }
    
    QString filePath = m_model->filePath(m_contextMenuIndex);
    QFileInfo info(filePath);
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        QString("Are you sure you want to delete '%1'?").arg(info.fileName()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        emit deleteFileRequested(filePath);
    }
}

void FileTreeWidget::onRenameAction() {
    if (!m_contextMenuIndex.isValid()) {
        return;
    }
    
    QString filePath = m_model->filePath(m_contextMenuIndex);
    emit renameFileRequested(filePath);
}

void FileTreeWidget::onOpenInExplorerAction() {
    QString path;
    
    if (m_contextMenuIndex.isValid()) {
        path = m_model->filePath(m_contextMenuIndex);
        QFileInfo info(path);
        if (info.isFile()) {
            path = info.absolutePath();
        }
    } else {
        path = m_model->rootPath();
    }
    
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

QString FileTreeWidget::selectedFilePath() const {
    QModelIndex index = currentIndex();
    if (index.isValid()) {
        return m_model->filePath(index);
    }
    return QString();
}
