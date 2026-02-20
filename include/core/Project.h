#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

class QJsonObject;

/**
 * @brief Represents a CppAtlas project/workspace
 */
class Project : public QObject {
    Q_OBJECT
    
public:
    static const QString FILE_EXTENSION;
    static const QString FORMAT_VERSION;
    
    enum class LoadResult {
        Success,
        FileNotFound,
        InvalidFormat,
        VersionMismatch,
        PermissionDenied
    };
    
    explicit Project(QObject *parent = nullptr);
    ~Project() override = default;
    
    // Core operations
    LoadResult load(const QString& filePath);
    bool save(const QString& filePath = QString());
    bool isValid() const;
    bool isModified() const;
    
    // Project info
    QString name() const { return m_name; }
    QString description() const { return m_description; }
    QString projectFilePath() const { return m_projectFilePath; }
    QString projectDirectory() const { return m_directory; }
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime lastOpenedAt() const { return m_lastOpenedAt; }
    
    // Backward compatibility
    QString directory() const { return m_directory; }
    
    // Build configuration
    QString compilerId() const { return m_compilerId; }
    QString standard() const { return m_standard; }
    QStringList compilerFlags() const { return m_compilerFlags; }
    QStringList includeDirectories() const { return m_includeDirectories; }
    QString outputDirectory() const { return m_outputDirectory; }
    
    // Source management
    QStringList sourceFiles() const { return m_sourceFiles; }
    QStringList headerFiles() const { return m_headerFiles; }
    void addSourceFile(const QString& path);
    void removeSourceFile(const QString& path);
    
    // Session
    QStringList openFiles() const { return m_openFiles; }
    QString activeFile() const { return m_activeFile; }
    QStringList expandedFolders() const { return m_expandedFolders; }
    void saveSession(const QStringList& openFiles,
                     const QString& activeFile,
                     const QStringList& expandedFolders);
    
    // Setters with change tracking
    void setName(const QString& name);
    void setDescription(const QString& description);
    void setDirectory(const QString& directory);
    void setCompilerId(const QString& id);
    void setStandard(const QString& standard);
    void setCompilerFlags(const QStringList& flags);
    void setOpenFiles(const QStringList& files);
    void setActiveFile(const QString& file);
    
signals:
    void projectChanged();
    void projectSaved();
    void projectLoaded();
    
private:
    void updateLastOpened();
    bool parseVersion1(const QJsonObject& root);
    QJsonObject toJson() const;
    
    // Project info
    QString m_name;
    QString m_description;
    QString m_directory;
    QString m_projectFilePath;
    QDateTime m_createdAt;
    QDateTime m_lastOpenedAt;
    
    // Build configuration
    QString m_compilerId;
    QString m_standard = "c++17";
    QStringList m_compilerFlags;
    QStringList m_includeDirectories;
    QString m_outputDirectory = "build";
    
    // Source management
    QStringList m_sourceFiles;
    QStringList m_headerFiles;
    
    // Session
    QStringList m_openFiles;
    QString m_activeFile;
    QStringList m_expandedFolders;
    
    bool m_modified = false;
};

#endif // PROJECT_H
