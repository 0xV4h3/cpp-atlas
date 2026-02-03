#include "core/FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QDir>

FileManager::FileManager(QObject *parent)
    : QObject(parent)
{
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, &QFileSystemWatcher::fileChanged, 
            this, &FileManager::onFileChanged);
    
    loadRecentFiles();
}

bool FileManager::createNewFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out << getTemplate();
    file.close();
    
    return true;
}

bool FileManager::fileExists(const QString& filePath) const {
    return QFile::exists(filePath);
}

QString FileManager::getTemplate() const {
    return R"(#include <iostream>

int main() {
    std::cout << "Hello, CppAtlas!" << std::endl;
    return 0;
}
)";
}

void FileManager::addRecentFile(const QString& filePath) {
    QString absolutePath = QFileInfo(filePath).absoluteFilePath();
    
    // Remove if already in list
    m_recentFiles.removeAll(absolutePath);
    
    // Add to front
    m_recentFiles.prepend(absolutePath);
    
    // Keep only MAX_RECENT_FILES
    while (m_recentFiles.size() > MAX_RECENT_FILES) {
        m_recentFiles.removeLast();
    }
    
    saveRecentFiles();
    emit recentFilesChanged();
}

QStringList FileManager::recentFiles() const {
    // Filter out files that no longer exist
    QStringList existing;
    for (const QString& file : m_recentFiles) {
        if (QFile::exists(file)) {
            existing.append(file);
        }
    }
    return existing;
}

void FileManager::clearRecentFiles() {
    m_recentFiles.clear();
    saveRecentFiles();
    emit recentFilesChanged();
}

void FileManager::watchFile(const QString& filePath) {
    if (!m_watcher->files().contains(filePath)) {
        m_watcher->addPath(filePath);
    }
}

void FileManager::unwatchFile(const QString& filePath) {
    if (m_watcher->files().contains(filePath)) {
        m_watcher->removePath(filePath);
    }
}

void FileManager::onFileChanged(const QString& path) {
    emit fileChangedExternally(path);
}

void FileManager::loadRecentFiles() {
    QSettings settings("CppAtlas", "CppAtlas");
    m_recentFiles = settings.value("recentFiles").toStringList();
}

void FileManager::saveRecentFiles() {
    QSettings settings("CppAtlas", "CppAtlas");
    settings.setValue("recentFiles", m_recentFiles);
}
