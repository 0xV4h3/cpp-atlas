#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QDateTime>

struct GitFileStatus {
    enum Status { Untracked, Modified, Staged, Added, Deleted, Renamed, Ignored, Clean };
    QString filePath;
    Status indexStatus = Clean;
    Status workTreeStatus = Clean;
};

struct GitCommitInfo {
    QString hash;
    QString shortHash;
    QString author;
    QString message;
    QDateTime date;
};

class GitManager : public QObject {
    Q_OBJECT
public:
    explicit GitManager(QObject* parent = nullptr);
    
    void setWorkingDirectory(const QString& path);
    QString workingDirectory() const { return m_workDir; }
    
    bool isGitRepository() const;
    QString currentBranch() const;
    
    QList<GitFileStatus> status() const;
    int stagedCount() const;
    int modifiedCount() const;
    int untrackedCount() const;
    
    bool stageFile(const QString& filePath);
    bool stageAll();
    bool unstageFile(const QString& filePath);
    bool unstageAll();
    bool commit(const QString& message);
    bool discardChanges(const QString& filePath);
    
    QString diffFile(const QString& filePath) const;
    QList<GitCommitInfo> log(int maxCount = 20) const;

signals:
    void statusChanged();
    void branchChanged(const QString& branch);
    void errorOccurred(const QString& error);

private:
    QString runGit(const QStringList& args) const;
    bool runGitBool(const QStringList& args);
    
    QString m_workDir;
};

#endif // GITMANAGER_H
