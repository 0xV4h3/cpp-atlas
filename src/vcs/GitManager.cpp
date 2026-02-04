#include "vcs/GitManager.h"
#include <QProcess>
#include <QDebug>

GitManager::GitManager(QObject* parent)
    : QObject(parent)
{
}

void GitManager::setWorkingDirectory(const QString& path) {
    m_workDir = path;
    emit statusChanged();
    emit branchChanged(currentBranch());
}

bool GitManager::isGitRepository() const {
    QDir dir(m_workDir);
    while (!dir.isRoot()) {
        if (dir.exists(".git")) {
            return true;
        }
        dir.cdUp();
    }
    return false;
}

QString GitManager::currentBranch() const {
    if (!isGitRepository()) return QString();
    QString output = runGit({"rev-parse", "--abbrev-ref", "HEAD"});
    return output.trimmed();
}

QString GitManager::runGit(const QStringList& args) const {
    QProcess proc;
    proc.setWorkingDirectory(m_workDir);
    proc.start("git", args);
    proc.waitForFinished(GIT_TIMEOUT_MS);
    return QString::fromUtf8(proc.readAllStandardOutput());
}

bool GitManager::runGitBool(const QStringList& args) {
    QProcess proc;
    proc.setWorkingDirectory(m_workDir);
    proc.start("git", args);
    proc.waitForFinished(GIT_TIMEOUT_MS);
    return proc.exitCode() == 0;
}

QList<GitFileStatus> GitManager::status() const {
    QList<GitFileStatus> result;
    if (!isGitRepository()) return result;
    
    QString output = runGit({"status", "--porcelain", "-uall"});
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString& line : lines) {
        if (line.length() < 4) continue;
        
        GitFileStatus fs;
        QChar indexChar = line[0];
        QChar workChar = line[1];
        fs.filePath = line.mid(3).trimmed();
        
        // Parse index status
        if (indexChar == 'A') fs.indexStatus = GitFileStatus::Added;
        else if (indexChar == 'M') fs.indexStatus = GitFileStatus::Modified;
        else if (indexChar == 'D') fs.indexStatus = GitFileStatus::Deleted;
        else if (indexChar == 'R') fs.indexStatus = GitFileStatus::Renamed;
        
        // Parse worktree status
        if (workChar == 'M') fs.workTreeStatus = GitFileStatus::Modified;
        else if (workChar == 'D') fs.workTreeStatus = GitFileStatus::Deleted;
        else if (workChar == '?') fs.workTreeStatus = GitFileStatus::Untracked;
        
        result.append(fs);
    }
    
    return result;
}

int GitManager::stagedCount() const {
    int count = 0;
    for (const auto& fs : status()) {
        if (fs.indexStatus != GitFileStatus::Clean) count++;
    }
    return count;
}

int GitManager::modifiedCount() const {
    int count = 0;
    for (const auto& fs : status()) {
        if (fs.workTreeStatus == GitFileStatus::Modified) count++;
    }
    return count;
}

int GitManager::untrackedCount() const {
    int count = 0;
    for (const auto& fs : status()) {
        if (fs.workTreeStatus == GitFileStatus::Untracked) count++;
    }
    return count;
}

bool GitManager::stageFile(const QString& filePath) {
    bool ok = runGitBool({"add", filePath});
    if (ok) emit statusChanged();
    return ok;
}

bool GitManager::stageAll() {
    bool ok = runGitBool({"add", "-A"});
    if (ok) emit statusChanged();
    return ok;
}

bool GitManager::unstageFile(const QString& filePath) {
    bool ok = runGitBool({"reset", "HEAD", "--", filePath});
    if (ok) emit statusChanged();
    return ok;
}

bool GitManager::unstageAll() {
    bool ok = runGitBool({"reset", "HEAD"});
    if (ok) emit statusChanged();
    return ok;
}

bool GitManager::commit(const QString& message) {
    bool ok = runGitBool({"commit", "-m", message});
    if (ok) emit statusChanged();
    return ok;
}

bool GitManager::discardChanges(const QString& filePath) {
    // Use git restore (modern) or checkout (fallback) to discard changes
    bool ok = runGitBool({"restore", "--", filePath});
    if (!ok) {
        // Fallback to checkout for older Git versions
        ok = runGitBool({"checkout", "--", filePath});
    }
    if (ok) emit statusChanged();
    return ok;
}

QString GitManager::diffFile(const QString& filePath) const {
    return runGit({"diff", "--", filePath});
}

QList<GitCommitInfo> GitManager::log(int maxCount) const {
    QList<GitCommitInfo> result;
    if (!isGitRepository()) return result;
    
    QString output = runGit({"log", QString("-%1").arg(maxCount), 
                             "--pretty=format:%H|%h|%an|%s|%ct"});
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString& line : lines) {
        QStringList parts = line.split('|');
        if (parts.size() >= 5) {
            GitCommitInfo info;
            info.hash = parts[0];
            info.shortHash = parts[1];
            info.author = parts[2];
            info.message = parts[3];
            info.date = QDateTime::fromSecsSinceEpoch(parts[4].toLongLong());
            result.append(info);
        }
    }
    
    return result;
}
