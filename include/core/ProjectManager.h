#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QStringList>
#include "core/Project.h"

struct ProjectSettings {
    QString compilerId;
    QString standard = "c++17";
    QStringList compilerFlags;
    bool createMainCpp = true;
    bool createCMakeLists = false;
    bool createProjectFolder = true;
};

class ProjectManager : public QObject {
    Q_OBJECT
    
public:
    static ProjectManager* instance();
    
    // Current project
    Project* currentProject() const;
    bool hasOpenProject() const;
    
    // Project operations
    bool createProject(const QString& name,
                       const QString& location,
                       const ProjectSettings& settings);
    Project::LoadResult openProject(const QString& filePath);
    bool saveCurrentProject();
    bool closeCurrentProject();
    
    // Recent projects
    QStringList recentProjects() const;
    void addToRecentProjects(const QString& path);
    void clearRecentProjects();
    
    // Get the path of the last created/opened project file
    QString lastProjectFilePath() const { return m_lastProjectFilePath; }
    
signals:
    void projectOpened(Project* project);
    void projectClosed();
    void projectCreated(Project* project);
    void recentProjectsChanged();
    
private:
    ProjectManager();
    static ProjectManager* s_instance;
    
    Project* m_currentProject = nullptr;
    QString m_lastProjectFilePath;
};

#endif // PROJECTMANAGER_H
