#include "core/RecentProjectsManager.h"
#include <QFileInfo>

RecentProjectsManager* RecentProjectsManager::s_instance = nullptr;

RecentProjectsManager::RecentProjectsManager(QObject *parent)
    : QObject(parent)
{
    load();
}

RecentProjectsManager* RecentProjectsManager::instance() {
    if (!s_instance) {
        s_instance = new RecentProjectsManager();
    }
    return s_instance;
}

QStringList RecentProjectsManager::recentProjects() const {
    return m_recentProjects;
}

void RecentProjectsManager::addRecentProject(const QString& path) {
    // Normalize path
    QString normalizedPath = QFileInfo(path).absoluteFilePath();
    
    // Remove if already exists (will re-add at top)
    m_recentProjects.removeAll(normalizedPath);
    
    // Add to top
    m_recentProjects.prepend(normalizedPath);
    
    // Trim to max
    while (m_recentProjects.size() > m_maxRecent) {
        m_recentProjects.removeLast();
    }
    
    save();
    emit recentProjectsChanged();
}

void RecentProjectsManager::removeRecentProject(const QString& path) {
    m_recentProjects.removeAll(path);
    save();
    emit recentProjectsChanged();
}

void RecentProjectsManager::clearRecent() {
    m_recentProjects.clear();
    save();
    emit recentProjectsChanged();
}

void RecentProjectsManager::setMaxRecentProjects(int max) {
    m_maxRecent = qMax(1, max);
    while (m_recentProjects.size() > m_maxRecent) {
        m_recentProjects.removeLast();
    }
    save();
}

void RecentProjectsManager::load() {
    QSettings settings("CppAtlas", "CppAtlas");
    m_recentProjects = settings.value("recentProjects").toStringList();
    
    // Validate paths exist
    QStringList validProjects;
    for (const QString& path : m_recentProjects) {
        if (QFileInfo::exists(path)) {
            validProjects.append(path);
        }
    }
    m_recentProjects = validProjects;
}

void RecentProjectsManager::save() {
    QSettings settings("CppAtlas", "CppAtlas");
    settings.setValue("recentProjects", m_recentProjects);
}
