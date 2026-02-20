#include "core/FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QRandomGenerator>

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

QString FileManager::loadTemplate(const QString& resourcePath) const {
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream stream(&file);
    return stream.readAll();
}

QString FileManager::generateHeaderGuard(const QString& fileName) const {
    QString guard = fileName.toUpper();
    guard.replace('.', '_');
    guard.replace('-', '_');
    guard.replace(' ', '_');
    quint32 suffix = QRandomGenerator::global()->generate();
    guard += QString("_%1").arg(suffix);
    return guard;
}

bool FileManager::createSourceFile(const QString& filePath) {
    QString tmpl = loadTemplate(":/templates/source.cpp.template");
    if (tmpl.isEmpty()) {
        tmpl = getTemplate();
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << tmpl;
    file.close();
    return true;
}

bool FileManager::createHeaderFile(const QString& filePath) {
    QString tmpl = loadTemplate(":/templates/header.hpp.template");
    if (tmpl.isEmpty()) {
        tmpl = "#ifndef ${HEADER_GUARD}\n#define ${HEADER_GUARD}\n\n// TODO: Add declarations\n\n#endif // ${HEADER_GUARD}\n";
    }
    QString guard = generateHeaderGuard(QFileInfo(filePath).fileName());
    tmpl.replace("${HEADER_GUARD}", guard);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << tmpl;
    file.close();
    return true;
}

QStringList FileManager::createClassFiles(const QString& directory, const QString& className) {
    QStringList created;
    QString hppPath = QDir(directory).filePath(className + ".hpp");
    QString cppPath = QDir(directory).filePath(className + ".cpp");

    // Header
    QString hppTmpl = loadTemplate(":/templates/class.hpp.template");
    if (hppTmpl.isEmpty()) {
        return created;
    }
    QString guard = generateHeaderGuard(className + ".hpp");
    hppTmpl.replace("${HEADER_GUARD}", guard);
    hppTmpl.replace("${CLASS_NAME}", className);
    QFile hppFile(hppPath);
    if (!hppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return created;
    }
    QTextStream hppOut(&hppFile);
    hppOut << hppTmpl;
    hppFile.close();
    created << hppPath;

    // Source
    QString cppTmpl = loadTemplate(":/templates/class.cpp.template");
    if (cppTmpl.isEmpty()) {
        return created;
    }
    cppTmpl.replace("${CLASS_NAME}", className);
    QFile cppFile(cppPath);
    if (!cppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return created;
    }
    QTextStream cppOut(&cppFile);
    cppOut << cppTmpl;
    cppFile.close();
    created << cppPath;

    return created;
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
