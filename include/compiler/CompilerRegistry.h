#ifndef COMPILERREGISTRY_H
#define COMPILERREGISTRY_H

#include "ICompiler.h"
#include <QObject>
#include <QMap>
#include <QSharedPointer>

/**
 * @brief Registry for managing available compilers
 */
class CompilerRegistry : public QObject {
    Q_OBJECT
    
public:
    static CompilerRegistry& instance();
    
    /**
     * @brief Register a compiler
     * @param compiler Shared pointer to compiler
     */
    void registerCompiler(QSharedPointer<ICompiler> compiler);
    
    /**
     * @brief Unregister a compiler by ID
     * @param compilerId Compiler ID
     */
    void unregisterCompiler(const QString& compilerId);
    
    /**
     * @brief Get compiler by ID
     * @param compilerId Compiler ID
     * @return Shared pointer to compiler, or nullptr if not found
     */
    QSharedPointer<ICompiler> getCompiler(const QString& compilerId) const;
    
    /**
     * @brief Get list of all registered compilers
     * @return List of compilers
     */
    QList<QSharedPointer<ICompiler>> getCompilers() const;
    
    /**
     * @brief Get list of available (installed) compilers
     * @return List of available compilers
     */
    QList<QSharedPointer<ICompiler>> getAvailableCompilers() const;
    
    /**
     * @brief Auto-scan system for installed compilers
     */
    void autoScanCompilers();
    
    /**
     * @brief Load configuration from JSON file
     * @param filePath Path to compilers.json
     * @return true if successful
     */
    bool loadConfiguration(const QString& filePath);
    
    /**
     * @brief Save configuration to JSON file
     * @param filePath Path to compilers.json
     * @return true if successful
     */
    bool saveConfiguration(const QString& filePath);
    
    /**
     * @brief Get default compiler ID
     * @return Default compiler ID
     */
    QString defaultCompilerId() const;
    
    /**
     * @brief Set default compiler ID
     * @param compilerId Compiler ID
     */
    void setDefaultCompilerId(const QString& compilerId);
    
    /**
     * @brief Get default C++ standard
     * @return Default standard (e.g., "c++17")
     */
    QString defaultStandard() const;
    
    /**
     * @brief Set default C++ standard
     * @param standard Standard string
     */
    void setDefaultStandard(const QString& standard);
    
signals:
    void compilersChanged();
    
private:
    CompilerRegistry() = default;
    ~CompilerRegistry() = default;
    CompilerRegistry(const CompilerRegistry&) = delete;
    CompilerRegistry& operator=(const CompilerRegistry&) = delete;
    
    QMap<QString, QSharedPointer<ICompiler>> m_compilers;
    QString m_defaultCompilerId;
    QString m_defaultStandard = "c++17";
    
    /**
     * @brief Try to find a compiler executable in system PATH
     * @param name Executable name (e.g., "g++", "clang++")
     * @return Full path if found, empty string otherwise
     */
    QString findInPath(const QString& name) const;
};

#endif // COMPILERREGISTRY_H
