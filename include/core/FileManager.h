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
     * @brief Load a template from Qt resources
     * @param resourcePath Resource path (e.g. ":/templates/source.cpp.template")
     * @return Template content, or empty string on failure
     */
    QString loadTemplate(const QString& resourcePath) const;
    
    /**
     * @brief Generate a header guard string from a filename
     * @param fileName Filename (e.g. "MyClass.hpp")
     * @return Header guard string
     */
    QString generateHeaderGuard(const QString& fileName) const;
    
    /**
     * @brief Create a source file from template
     * @param filePath Destination file path
     * @return true if successful
     */
    bool createSourceFile(const QString& filePath);
    
    /**
     * @brief Create a header file from template
     * @param filePath Destination file path
     * @return true if successful
     */
    bool createHeaderFile(const QString& filePath);
    
    /**
     * @brief Create a class (both .hpp and .cpp) from templates
     * @param directory Directory for files
     * @param className Class name
     * @return List of created file paths
     */
    QStringList createClassFiles(const QString& directory, const QString& className);
    
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
