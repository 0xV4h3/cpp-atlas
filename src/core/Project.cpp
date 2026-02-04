#include "core/Project.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

Project::Project(QObject *parent)
    : QObject(parent)
{
}

bool Project::load(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    m_name = root["name"].toString();
    m_directory = QFileInfo(filePath).absolutePath();
    m_compilerId = root["compilerId"].toString();
    m_standard = root["standard"].toString("c++17");
    m_activeFile = root["activeFile"].toString();
    
    // Load open files
    m_openFiles.clear();
    QJsonArray filesArray = root["openFiles"].toArray();
    for (const QJsonValue& value : filesArray) {
        m_openFiles.append(value.toString());
    }
    
    // Load compiler flags
    m_compilerFlags.clear();
    QJsonArray flagsArray = root["compilerFlags"].toArray();
    for (const QJsonValue& value : flagsArray) {
        m_compilerFlags.append(value.toString());
    }
    
    emit projectChanged();
    return true;
}

bool Project::save(const QString& filePath) {
    QJsonObject root;
    root["name"] = m_name;
    root["compilerId"] = m_compilerId;
    root["standard"] = m_standard;
    root["activeFile"] = m_activeFile;
    
    // Save open files
    QJsonArray filesArray;
    for (const QString& file : m_openFiles) {
        filesArray.append(file);
    }
    root["openFiles"] = filesArray;
    
    // Save compiler flags
    QJsonArray flagsArray;
    for (const QString& flag : m_compilerFlags) {
        flagsArray.append(flag);
    }
    root["compilerFlags"] = flagsArray;
    
    QJsonDocument doc(root);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

void Project::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        emit projectChanged();
    }
}

void Project::setDirectory(const QString& directory) {
    if (m_directory != directory) {
        m_directory = directory;
        emit projectChanged();
    }
}

void Project::setOpenFiles(const QStringList& files) {
    m_openFiles = files;
    emit projectChanged();
}

void Project::setActiveFile(const QString& file) {
    if (m_activeFile != file) {
        m_activeFile = file;
        emit projectChanged();
    }
}

void Project::setCompilerId(const QString& id) {
    if (m_compilerId != id) {
        m_compilerId = id;
        emit projectChanged();
    }
}

void Project::setStandard(const QString& standard) {
    if (m_standard != standard) {
        m_standard = standard;
        emit projectChanged();
    }
}

void Project::setCompilerFlags(const QStringList& flags) {
    m_compilerFlags = flags;
    emit projectChanged();
}
