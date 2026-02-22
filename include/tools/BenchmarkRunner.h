#ifndef BENCHMARKRUNNER_H
#define BENCHMARKRUNNER_H

#include "tools/IToolRunner.h"
#include "tools/BenchmarkResult.h"
#include <QProcess>
#include <QScopedPointer>
#include <QTemporaryDir>

/**
 * @brief Compiles and runs a Google Benchmark C++ source file.
 *
 * Two-phase execution — mirrors CppInsightsRunner / AssemblyRunner pattern:
 *
 *   Phase 1 — Compile:
 *     <compiler> <source> -o <tmp_binary>
 *       -std=<std> -<opt>
 *       -I<benchmarkIncludeDir()>
 *       -L<benchmarkLibDir()> -lbenchmark -lbenchmark_main -lpthread
 *
 *   Phase 2 — Run:
 *     <tmp_binary> --benchmark_format=json
 *     stdout → parseJsonOutput() → BenchmarkResult
 *
 * Compiler is set externally via setCompilerId() — NOT chosen inside
 * this class.  This follows the same pattern as AssemblyRunner.
 *
 * Benchmark library paths come from ToolsConfig::instance().
 *
 * JSON format reference:
 *   https://github.com/google/benchmark#output-formats
 *
 * User code must include <benchmark/benchmark.h>, define benchmarks
 * with BENCHMARK() and use BENCHMARK_MAIN() (or link -lbenchmark_main).
 * DoNotOptimize() / ClobberMemory() prevent over-optimisation:
 *   https://github.com/google/benchmark#preventing-optimisation
 */
class BenchmarkRunner : public IToolRunner {
    Q_OBJECT

public:
    explicit BenchmarkRunner(QObject* parent = nullptr);
    ~BenchmarkRunner() override;

    // ── IToolRunner ──────────────────────────────────────────────
    bool    isAvailable()  const override;
    QString toolName()     const override { return QStringLiteral("Benchmark"); }

    /**
     * @param sourceFile  Absolute path to the benchmark .cpp file
     * @param flags       e.g. {"-std=c++17", "-O2"}
     *                    compilerId set via setCompilerId()
     */
    void run(const QString& sourceFile, const QStringList& flags) override;
    void cancel() override;

    // ── Configuration (set from MainWindow, not internally) ──────
    void    setCompilerId(const QString& id);
    QString compilerId()  const;

    // ── Results ──────────────────────────────────────────────────
    BenchmarkResult lastResult() const;

    // ── Export ───────────────────────────────────────────────────
    bool exportToJson(const QString& filePath) const;
    bool exportToCsv (const QString& filePath) const;

signals:
    /**
     * Emitted with the full parsed result after successful execution.
     * Also emits IToolRunner::finished(true, rawJson, "") on success.
     */
    void benchmarkResultReady(const BenchmarkResult& result);

    /**
     * Emitted after Phase 1 (compile step) completes.
     * On failure, finished() is also emitted.
     */
    void compilationFinished(bool success, const QString& errorOutput);

private slots:
    void onCompileFinished(int exitCode, QProcess::ExitStatus status);
    void onCompileError   (QProcess::ProcessError error);
    void onRunFinished    (int exitCode, QProcess::ExitStatus status);
    void onRunError       (QProcess::ProcessError error);

private:
    void            startRun(const QString& binaryPath);
    BenchmarkResult parseJsonOutput(const QString& json) const;

    QString  m_compilerId;
    QProcess* m_compileProcess = nullptr;
    QProcess* m_runProcess     = nullptr;
    BenchmarkResult m_lastResult;

    // Temp dir owns the output binary for the duration of compile+run.
    QScopedPointer<QTemporaryDir> m_tempDir;
    QString     m_tempBinaryPath;
    QString     m_sourceFilePath;
    QStringList m_compileFlags;
};

#endif // BENCHMARKRUNNER_H
