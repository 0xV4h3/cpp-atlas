#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileSystemWatcher>

/**
 * @brief Manages file operations and recent files
 */
class FileManager : public QObject {
    Q_OBJECT
    
public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager() override = default;
    
    /**
     * @brief Create a new file with C++ template
     * @param filePath Path for new file
     * @return true if successful
     */
    bool createNewFile(const QString& filePath);
    
    /**
     * @brief Check if file exists
     * @param filePath File path
     * @return true if file exists
     */
    bool fileExists(const QString& filePath) const;
    
    /**
     * @brief Get C++ file template content
     * @return Template content
     */
    QString getTemplate() const;
    
    /**
     * @brief Add file to recent files list
     * @param filePath File path
     */
    void addRecentFile(const QString& filePath);
    
    /**
     * @brief Get recent files list
     * @return List of recent file paths
     */
    QStringList recentFiles() const;
    
    /**
     * @brief Clear recent files list
     */
    void clearRecentFiles();
    
    /**
     * @brief Watch a file for external changes
     * @param filePath File path
     */
    void watchFile(const QString& filePath);
    
    /**
     * @brief Stop watching a file
     * @param filePath File path
     */
    void unwatchFile(const QString& filePath);
    
signals:
    void fileChangedExternally(const QString& filePath);
    void recentFilesChanged();
    
private slots:
    void onFileChanged(const QString& path);
    
private:
    static const int MAX_RECENT_FILES = 10;
    QStringList m_recentFiles;
    QFileSystemWatcher* m_watcher;
    
    void loadRecentFiles();
    void saveRecentFiles();
};

#endif // FILEMANAGER_H
