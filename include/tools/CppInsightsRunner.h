#ifndef CPPINSIGHTSRUNNER_H
#define CPPINSIGHTSRUNNER_H

#include "tools/IToolRunner.h"
#include <QProcess>

/**
 * @brief Runs the `insights` binary on a C++ source file and returns
 * the transformed (desugared) C++ output.
 *
 * Invocation pattern (from https://github.com/andreasfertig/cppinsights#usage):
 *   insights <source_file> -- [compiler_flags]
 *   e.g.: insights main.cpp -- -std=c++17 -I/usr/include
 *
 * The binary path is read from ToolsConfig::instance().cppInsightsPath().
 * It can be overridden at runtime via setExecutablePath().
 *
 * Async: emits started(), finished(), progressMessage() from IToolRunner.
 */
class CppInsightsRunner : public IToolRunner {
    Q_OBJECT

public:
    explicit CppInsightsRunner(QObject* parent = nullptr);
    ~CppInsightsRunner() override;

    // IToolRunner interface
    bool isAvailable() const override;
    QString toolName() const override { return QStringLiteral("C++ Insights"); }
    void run(const QString& sourceFile, const QStringList& flags) override;
    void cancel() override;

    // Path management — overrides ToolsConfig default
    void setExecutablePath(const QString& path);
    QString executablePath() const;

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onProcessError(QProcess::ProcessError error);
    void onProcessStarted();

private:
    QString m_execPath;   // Resolved from ToolsConfig if empty
    QProcess* m_process = nullptr;
};

#endif // CPPINSIGHTSRUNNER_H
