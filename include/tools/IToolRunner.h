#ifndef ITOOLRUNNER_H
#define ITOOLRUNNER_H

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * @brief Abstract interface for all code analysis tool runners.
 *
 * All runners operate asynchronously via QProcess and communicate
 * results through Qt signals. Mirrors the ICompiler pattern in
 * include/compiler/ICompiler.h.
 *
 * Lifecycle:
 *   1. Call isAvailable() to check if the underlying binary/library exists.
 *   2. Call run() with a source file path and compiler flags.
 *   3. Connect to finished() to receive results.
 *   4. Call cancel() to abort a running operation.
 *
 * References:
 *   - Qt QProcess: https://doc.qt.io/qt-6/qprocess.html
 *   - CppInsights: https://github.com/andreasfertig/cppinsights#usage
 *   - Google Benchmark: https://github.com/google/benchmark
 */
class IToolRunner : public QObject {
    Q_OBJECT

public:
    explicit IToolRunner(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IToolRunner() = default;

    /**
     * @brief Check if the underlying tool binary/library is accessible.
     * @return true if the tool can be invoked
     */
    virtual bool isAvailable() const = 0;

    /**
     * @brief Human-readable name of this tool (e.g., "C++ Insights").
     */
    virtual QString toolName() const = 0;

    /**
     * @brief Start the tool asynchronously.
     * @param sourceFile  Absolute path to the C++ source file to analyse.
     * @param flags       Additional compiler/tool flags (e.g., {"-std=c++17", "-O2"}).
     *
     * Emits started() when the process begins, finished() when it ends.
     * If isAvailable() returns false, finished() is emitted immediately
     * with success=false and a descriptive errorOutput.
     */
    virtual void run(const QString& sourceFile, const QStringList& flags) = 0;

    /**
     * @brief Abort a running tool process.
     * Emits finished() with success=false after cancellation.
     */
    virtual void cancel() = 0;

signals:
    /** Emitted when the underlying process has started. */
    void started();

    /**
     * @brief Emitted when the tool has finished (success or failure).
     * @param success     true if the tool exited with code 0
     * @param output      stdout content from the tool
     * @param errorOutput stderr content from the tool
     */
    void finished(bool success, const QString& output, const QString& errorOutput);

    /**
     * @brief Emitted during long-running operations for status bar updates.
     * @param message  Human-readable progress description
     */
    void progressMessage(const QString& message);
};

#endif // ITOOLRUNNER_H
