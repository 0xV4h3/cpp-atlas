#ifndef RECENTPROJECTSMANAGER_H
#define RECENTPROJECTSMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSettings>

class RecentProjectsManager : public QObject
{
    Q_OBJECT

public:
    static RecentProjectsManager* instance();
    
    QStringList recentProjects() const;
    void addRecentProject(const QString& path);
    void removeRecentProject(const QString& path);
    void clearRecent();
    
    int maxRecentProjects() const { return m_maxRecent; }
    void setMaxRecentProjects(int max);

signals:
    void recentProjectsChanged();

private:
    explicit RecentProjectsManager(QObject *parent = nullptr);
    void load();
    void save();
    
    static RecentProjectsManager* s_instance;
    QStringList m_recentProjects;
    int m_maxRecent = 10;
};

#endif // RECENTPROJECTSMANAGER_H
