#include "ui/FileTreeWidget.h"
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>

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
    
    // New submenu
    QMenu* newMenu = m_contextMenu->addMenu("New");
    QAction* newSourceAction = newMenu->addAction("Source File");
    QAction* newHeaderAction = newMenu->addAction("Header File");
    QAction* newClassAction = newMenu->addAction("Class...");
    QAction* newFolderAction = newMenu->addAction("Folder");

    connect(newSourceAction, &QAction::triggered, this, [this]() {
        QString directory;
        if (m_contextMenuIndex.isValid()) {
            QString path = m_model->filePath(m_contextMenuIndex);
            QFileInfo info(path);
            directory = info.isDir() ? path : info.absolutePath();
        } else {
            directory = m_model->rootPath();
        }
        bool ok;
        QString name = QInputDialog::getText(this, "New Source File",
            "File name:", QLineEdit::Normal, "main.cpp", &ok);
        if (ok && !name.isEmpty()) {
            QString filePath = QDir(directory).filePath(name);
            if (QFile::exists(filePath)) {
                QMessageBox::warning(this, "Error", "File already exists.");
                return;
            }
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream << "#include <iostream>\n\nint main() {\n    std::cout << \"Hello, CppAtlas!\" << std::endl;\n    return 0;\n}\n";
                file.close();
                emit newFileRequested(filePath);
            }
        }
    });

    connect(newHeaderAction, &QAction::triggered, this, [this]() {
        QString directory;
        if (m_contextMenuIndex.isValid()) {
            QString path = m_model->filePath(m_contextMenuIndex);
            QFileInfo info(path);
            directory = info.isDir() ? path : info.absolutePath();
        } else {
            directory = m_model->rootPath();
        }
        bool ok;
        QString name = QInputDialog::getText(this, "New Header File",
            "File name:", QLineEdit::Normal, "header.hpp", &ok);
        if (ok && !name.isEmpty()) {
            QString filePath = QDir(directory).filePath(name);
            if (QFile::exists(filePath)) {
                QMessageBox::warning(this, "Error", "File already exists.");
                return;
            }
            QString guard = name.toUpper().replace('.', '_').replace('-', '_');
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream << "#ifndef " << guard << "\n#define " << guard
                       << "\n\n// TODO: Add declarations\n\n#endif // " << guard << "\n";
                file.close();
                emit newFileRequested(filePath);
            }
        }
    });

    connect(newClassAction, &QAction::triggered, this, [this]() {
        QString directory;
        if (m_contextMenuIndex.isValid()) {
            QString path = m_model->filePath(m_contextMenuIndex);
            QFileInfo info(path);
            directory = info.isDir() ? path : info.absolutePath();
        } else {
            directory = m_model->rootPath();
        }
        emit newFileRequested(directory);
    });

    connect(newFolderAction, &QAction::triggered, this, [this]() {
        QString directory;
        if (m_contextMenuIndex.isValid()) {
            QString path = m_model->filePath(m_contextMenuIndex);
            QFileInfo info(path);
            directory = info.isDir() ? path : info.absolutePath();
        } else {
            directory = m_model->rootPath();
        }
        bool ok;
        QString name = QInputDialog::getText(this, "New Folder",
            "Folder name:", QLineEdit::Normal, "NewFolder", &ok);
        if (ok && !name.isEmpty()) {
            QDir(directory).mkdir(name);
        }
    });

    m_renameAction = m_contextMenu->addAction("Rename");
    m_renameAction->setShortcut(Qt::Key_F2);
    m_deleteAction = m_contextMenu->addAction("Delete");
    m_deleteAction->setShortcut(Qt::Key_Delete);
    m_contextMenu->addSeparator();
    m_openInExplorerAction = m_contextMenu->addAction("Open in File Explorer");
    
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
        // Enable/disable actions based on selection
        m_deleteAction->setEnabled(true);
        m_renameAction->setEnabled(true);
        m_openInExplorerAction->setEnabled(true);
    } else {
        // Clicked on empty space
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
        "Delete",
        QString("Are you sure you want to delete '%1'?").arg(info.fileName()),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (info.isDir()) {
            QDir(filePath).removeRecursively();
        } else {
            QFile::remove(filePath);
        }
        emit fileDeleted(filePath);
    }
}

void FileTreeWidget::onRenameAction() {
    if (!m_contextMenuIndex.isValid()) {
        return;
    }
    
    QString oldPath = m_model->filePath(m_contextMenuIndex);
    QFileInfo info(oldPath);
    
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename",
        "New name:", QLineEdit::Normal, info.fileName(), &ok);
    
    if (ok && !newName.isEmpty()) {
        QString newPath = info.dir().filePath(newName);
        if (QFile::rename(oldPath, newPath)) {
            emit fileRenamed(oldPath, newPath);
        }
    }
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
