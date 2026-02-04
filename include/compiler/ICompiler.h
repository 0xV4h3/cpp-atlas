#ifndef ICOMPILER_H
#define ICOMPILER_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include "CompileRequest.h"
#include "CompileResult.h"

/**
 * @brief Abstract interface for C++ compilers
 */
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    /**
     * @brief Unique identifier for this compiler instance
     * @return ID string (e.g., "gcc-13", "clang-17")
     */
    virtual QString id() const = 0;
    
    /**
     * @brief Human-readable name of the compiler
     * @return Name string (e.g., "GCC 13.2.0")
     */
    virtual QString name() const = 0;
    
    /**
     * @brief Path to the compiler executable
     * @return Executable path
     */
    virtual QString executablePath() const = 0;
    
    /**
     * @brief List of C++ standards supported by this compiler
     * @return List of standard strings (e.g., "c++17", "c++20", "c++23")
     */
    virtual QStringList supportedStandards() const = 0;
    
    /**
     * @brief Default C++ standard for this compiler
     * @return Default standard string
     */
    virtual QString defaultStandard() const = 0;
    
    /**
     * @brief Check if the compiler is available on the system
     * @return true if compiler executable exists and is accessible
     */
    virtual bool isAvailable() const = 0;
    
    /**
     * @brief Query the compiler version
     * @return Version string
     */
    virtual QString version() const = 0;
    
    /**
     * @brief Compile source code
     * @param request Compilation request with all parameters
     * @return Compilation result with diagnostics
     */
    virtual CompileResult compile(const CompileRequest& request) = 0;
    
    /**
     * @brief Run a compiled executable
     * @param exePath Path to the executable
     * @param args Command-line arguments
     * @return QProcess pointer (caller takes ownership)
     */
    virtual QProcess* runExecutable(const QString& exePath, const QStringList& args) = 0;
};

#endif // ICOMPILER_H
