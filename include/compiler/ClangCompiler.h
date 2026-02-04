#ifndef CLANGCOMPILER_H
#define CLANGCOMPILER_H

#include "ICompiler.h"
#include <QObject>

/**
 * @brief Clang/Clang++ compiler implementation
 */
class ClangCompiler : public QObject, public ICompiler {
    Q_OBJECT
    
public:
    explicit ClangCompiler(const QString& execPath = "clang++", const QString& compilerId = "clang-system");
    ~ClangCompiler() override = default;
    
    QString id() const override;
    QString name() const override;
    QString executablePath() const override;
    QStringList supportedStandards() const override;
    QString defaultStandard() const override;
    bool isAvailable() const override;
    QString version() const override;
    CompileResult compile(const CompileRequest& request) override;
    QProcess* runExecutable(const QString& exePath, const QStringList& args) override;
    
private:
    QString m_id;
    QString m_execPath;
    mutable QString m_version;
    mutable bool m_versionQueried = false;
    
    /**
     * @brief Parse Clang diagnostic output
     * @param output Compiler output (stdout/stderr)
     * @return List of diagnostic messages
     */
    QList<DiagnosticMessage> parseDiagnostics(const QString& output);
    
    /**
     * @brief Convert optimization level to Clang flag
     */
    QString optimizationFlag(OptimizationLevel level) const;
};

#endif // CLANGCOMPILER_H
