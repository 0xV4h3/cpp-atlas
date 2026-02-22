#ifndef ASSEMBLYRUNNER_H
#define ASSEMBLYRUNNER_H

#include "tools/IToolRunner.h"
#include <QMap>
#include <QProcess>

/**
 * @brief Generates assembly output for a C++ source file by invoking the
 * compiler with the \c -S flag (and \c -g for \c .loc directives) and parses
 * the resulting \c .s file to build a source-line ↔ assembly-line map.
 *
 * Invocation (GCC / Clang):
 *   <compiler> -S -g [-masm=intel] [-O<n>] -std=<std> <sourceFile> -o <tmp.s>
 *
 * The compiler is looked up from CompilerRegistry by the ID set via
 * setCompilerId().  Intel syntax is enabled via setIntelSyntax(true)
 * (GCC/Clang only — adds \c -masm=intel).
 *
 * Async: emits started(), finished(), progressMessage() from IToolRunner
 * plus lineMapReady() after successful assembly generation.
 */
class AssemblyRunner : public IToolRunner {
    Q_OBJECT

public:
    explicit AssemblyRunner(QObject* parent = nullptr);
    ~AssemblyRunner() override;

    // IToolRunner interface
    bool isAvailable() const override;
    QString toolName() const override { return QStringLiteral("Assembly"); }
    void run(const QString& sourceFile, const QStringList& flags) override;
    void cancel() override;

    // Configuration
    void setCompilerId(const QString& id);
    QString compilerId() const;

    void setIntelSyntax(bool intel);
    bool intelSyntax() const;

signals:
    /**
     * @brief Emitted after a successful run with the source-line ↔ asm-line map.
     *
     * Key   = 1-based line number in the assembly output text.
     * Value = 1-based source line number the instruction was generated from.
     *
     * Lines for which no \c .loc directive was seen have no entry in the map.
     */
    void lineMapReady(const QMap<int, int>& asmLineToSrcLine);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onProcessError(QProcess::ProcessError error);
    void onProcessStarted();

private:
    /**
     * @brief Parse \c .loc directives from assembly text to produce the map.
     * @param asmText Full content of the generated \c .s file.
     * @return Map from asm output line (1-based) → source line (1-based).
     */
    static QMap<int, int> parseLocDirectives(const QString& asmText);

    QString  m_compilerId;
    bool     m_intelSyntax  = false;
    QProcess* m_process     = nullptr;
    QString  m_tmpAsmFile;
};

#endif // ASSEMBLYRUNNER_H
