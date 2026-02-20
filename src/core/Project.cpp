#include "core/Project.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

const QString Project::FILE_EXTENSION = QStringLiteral(".cppatlas");
const QString Project::FORMAT_VERSION = QStringLiteral("1.0");

Project::Project(QObject *parent)
    : QObject(parent)
{
}

Project::LoadResult Project::load(const QString& filePath) {
    QFileInfo fileInfo(filePath);
    
    if (!fileInfo.exists()) {
        return LoadResult::FileNotFound;
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return LoadResult::PermissionDenied;
    }
    
    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return LoadResult::InvalidFormat;
    }
    
    QJsonObject root = doc.object();
    
    // Check version
    QString version = root["version"].toString();
    if (version.isEmpty()) {
        // Legacy format (no version field) - try to load anyway
        m_name = root["name"].toString();
        m_directory = fileInfo.absolutePath();
        m_projectFilePath = fileInfo.absoluteFilePath();
        m_compilerId = root["compilerId"].toString();
        m_standard = root["standard"].toString("c++17");
        m_activeFile = root["activeFile"].toString();
        
        m_openFiles.clear();
        QJsonArray filesArray = root["openFiles"].toArray();
        for (const QJsonValue& value : filesArray) {
            m_openFiles.append(value.toString());
        }
        
        m_compilerFlags.clear();
        QJsonArray flagsArray = root["compilerFlags"].toArray();
        for (const QJsonValue& value : flagsArray) {
            m_compilerFlags.append(value.toString());
        }
    } else if (version == "1.0") {
        if (!parseVersion1(root)) {
            return LoadResult::InvalidFormat;
        }
        m_directory = fileInfo.absolutePath();
        m_projectFilePath = fileInfo.absoluteFilePath();
    } else {
        return LoadResult::VersionMismatch;
    }
    
    updateLastOpened();
    m_modified = false;
    
    emit projectLoaded();
    emit projectChanged();
    return LoadResult::Success;
}

bool Project::parseVersion1(const QJsonObject& root) {
    m_name = root["name"].toString();
    m_description = root["description"].toString();
    m_createdAt = QDateTime::fromString(root["created"].toString(), Qt::ISODate);
    m_lastOpenedAt = QDateTime::fromString(root["lastOpened"].toString(), Qt::ISODate);
    
    // Build configuration
    QJsonObject build = root["build"].toObject();
    m_compilerId = build["compilerId"].toString();
    m_standard = build["standard"].toString("c++17");
    
    m_compilerFlags.clear();
    for (const QJsonValue& v : build["compilerFlags"].toArray()) {
        m_compilerFlags.append(v.toString());
    }
    
    m_includeDirectories.clear();
    for (const QJsonValue& v : build["includeDirectories"].toArray()) {
        m_includeDirectories.append(v.toString());
    }
    m_outputDirectory = build["outputDirectory"].toString("build");
    
    // Sources
    QJsonObject sources = root["sources"].toObject();
    m_sourceFiles.clear();
    for (const QJsonValue& v : sources["sourceFiles"].toArray()) {
        m_sourceFiles.append(v.toString());
    }
    m_headerFiles.clear();
    for (const QJsonValue& v : sources["headerFiles"].toArray()) {
        m_headerFiles.append(v.toString());
    }
    
    // Session
    QJsonObject session = root["session"].toObject();
    m_openFiles.clear();
    for (const QJsonValue& v : session["openFiles"].toArray()) {
        m_openFiles.append(v.toString());
    }
    m_activeFile = session["activeFile"].toString();
    m_expandedFolders.clear();
    for (const QJsonValue& v : session["expandedFolders"].toArray()) {
        m_expandedFolders.append(v.toString());
    }
    
    return true;
}

QJsonObject Project::toJson() const {
    QJsonObject root;
    root["version"] = FORMAT_VERSION;
    root["name"] = m_name;
    root["description"] = m_description;
    root["created"] = m_createdAt.toString(Qt::ISODate);
    root["lastOpened"] = m_lastOpenedAt.toString(Qt::ISODate);
    
    // Build configuration
    QJsonObject build;
    build["compilerId"] = m_compilerId;
    build["standard"] = m_standard;
    
    QJsonArray flagsArray;
    for (const QString& flag : m_compilerFlags) {
        flagsArray.append(flag);
    }
    build["compilerFlags"] = flagsArray;
    
    QJsonArray includeDirsArray;
    for (const QString& dir : m_includeDirectories) {
        includeDirsArray.append(dir);
    }
    build["includeDirectories"] = includeDirsArray;
    build["outputDirectory"] = m_outputDirectory;
    
    root["build"] = build;
    
    // Sources
    QJsonObject sources;
    sources["rootDirectory"] = ".";
    
    QJsonArray sourceFilesArray;
    for (const QString& f : m_sourceFiles) {
        sourceFilesArray.append(f);
    }
    sources["sourceFiles"] = sourceFilesArray;
    
    QJsonArray headerFilesArray;
    for (const QString& f : m_headerFiles) {
        headerFilesArray.append(f);
    }
    sources["headerFiles"] = headerFilesArray;
    
    QJsonArray excludeArray;
    excludeArray.append("build/*");
    excludeArray.append("*.o");
    excludeArray.append("*.exe");
    sources["excludePatterns"] = excludeArray;
    
    root["sources"] = sources;
    
    // Session
    QJsonObject session;
    QJsonArray openFilesArray;
    for (const QString& f : m_openFiles) {
        openFilesArray.append(f);
    }
    session["openFiles"] = openFilesArray;
    session["activeFile"] = m_activeFile;
    
    QJsonArray expandedArray;
    for (const QString& f : m_expandedFolders) {
        expandedArray.append(f);
    }
    session["expandedFolders"] = expandedArray;
    
    root["session"] = session;
    
    return root;
}

bool Project::save(const QString& filePath) {
    QString savePath = filePath.isEmpty() ? m_projectFilePath : filePath;
    if (savePath.isEmpty()) {
        return false;
    }
    
    QJsonDocument doc(toJson());
    
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    m_projectFilePath = savePath;
    m_modified = false;
    
    emit projectSaved();
    return true;
}

bool Project::isValid() const {
    return !m_name.isEmpty() && !m_directory.isEmpty();
}

bool Project::isModified() const {
    return m_modified;
}

void Project::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setDescription(const QString& description) {
    if (m_description != description) {
        m_description = description;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setDirectory(const QString& directory) {
    if (m_directory != directory) {
        m_directory = directory;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setOpenFiles(const QStringList& files) {
    m_openFiles = files;
    m_modified = true;
    emit projectChanged();
}

void Project::setActiveFile(const QString& file) {
    if (m_activeFile != file) {
        m_activeFile = file;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setCompilerId(const QString& id) {
    if (m_compilerId != id) {
        m_compilerId = id;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setStandard(const QString& standard) {
    if (m_standard != standard) {
        m_standard = standard;
        m_modified = true;
        emit projectChanged();
    }
}

void Project::setCompilerFlags(const QStringList& flags) {
    m_compilerFlags = flags;
    m_modified = true;
    emit projectChanged();
}

void Project::addSourceFile(const QString& path) {
    if (!m_sourceFiles.contains(path)) {
        m_sourceFiles.append(path);
        m_modified = true;
        emit projectChanged();
    }
}

void Project::removeSourceFile(const QString& path) {
    if (m_sourceFiles.removeAll(path) > 0) {
        m_modified = true;
        emit projectChanged();
    }
}

void Project::saveSession(const QStringList& openFiles,
                          const QString& activeFile,
                          const QStringList& expandedFolders) {
    m_openFiles = openFiles;
    m_activeFile = activeFile;
    m_expandedFolders = expandedFolders;
    m_modified = true;
}

void Project::updateLastOpened() {
    m_lastOpenedAt = QDateTime::currentDateTime();
    m_modified = true;
}
