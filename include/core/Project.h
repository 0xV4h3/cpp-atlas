#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * @brief Represents a CppAtlas project/workspace
 */
class Project : public QObject {
    Q_OBJECT
    
public:
    explicit Project(QObject *parent = nullptr);
    ~Project() override = default;
    
    /**
     * @brief Load project from file
     * @param filePath Path to .cppatlas project file
     * @return true if successful
     */
    bool load(const QString& filePath);
    
    /**
     * @brief Save project to file
     * @param filePath Path to .cppatlas project file
     * @return true if successful
     */
    bool save(const QString& filePath);
    
    /**
     * @brief Get project name
     * @return Project name
     */
    QString name() const { return m_name; }
    
    /**
     * @brief Set project name
     * @param name Project name
     */
    void setName(const QString& name);
    
    /**
     * @brief Get project directory
     * @return Project directory path
     */
    QString directory() const { return m_directory; }
    
    /**
     * @brief Set project directory
     * @param directory Project directory path
     */
    void setDirectory(const QString& directory);
    
    /**
     * @brief Get list of open files
     * @return List of file paths
     */
    QStringList openFiles() const { return m_openFiles; }
    
    /**
     * @brief Set list of open files
     * @param files List of file paths
     */
    void setOpenFiles(const QStringList& files);
    
    /**
     * @brief Get active file
     * @return Active file path
     */
    QString activeFile() const { return m_activeFile; }
    
    /**
     * @brief Set active file
     * @param file File path
     */
    void setActiveFile(const QString& file);
    
    /**
     * @brief Get compiler ID
     * @return Compiler ID
     */
    QString compilerId() const { return m_compilerId; }
    
    /**
     * @brief Set compiler ID
     * @param id Compiler ID
     */
    void setCompilerId(const QString& id);
    
    /**
     * @brief Get C++ standard
     * @return C++ standard string
     */
    QString standard() const { return m_standard; }
    
    /**
     * @brief Set C++ standard
     * @param standard C++ standard string
     */
    void setStandard(const QString& standard);
    
    /**
     * @brief Get compiler flags
     * @return List of compiler flags
     */
    QStringList compilerFlags() const { return m_compilerFlags; }
    
    /**
     * @brief Set compiler flags
     * @param flags List of compiler flags
     */
    void setCompilerFlags(const QStringList& flags);
    
signals:
    void projectChanged();
    
private:
    QString m_name;
    QString m_directory;
    QStringList m_openFiles;
    QString m_activeFile;
    QString m_compilerId;
    QString m_standard = "c++17";
    QStringList m_compilerFlags;
};

#endif // PROJECT_H
