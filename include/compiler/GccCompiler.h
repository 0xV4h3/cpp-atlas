#ifndef GCCCOMPILER_H
#define GCCCOMPILER_H

#include "ICompiler.h"
#include <QObject>

/**
 * @brief GCC/G++ compiler implementation
 */
class GccCompiler : public QObject, public ICompiler {
    Q_OBJECT
    
public:
    explicit GccCompiler(const QString& execPath = "g++", const QString& compilerId = "gcc-system");
    ~GccCompiler() override = default;
    
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
     * @brief Parse GCC diagnostic output
     * @param output Compiler output (stdout/stderr)
     * @return List of diagnostic messages
     */
    QList<DiagnosticMessage> parseDiagnostics(const QString& output);
    
    /**
     * @brief Convert optimization level to GCC flag
     */
    QString optimizationFlag(OptimizationLevel level) const;
};

#endif // GCCCOMPILER_H
