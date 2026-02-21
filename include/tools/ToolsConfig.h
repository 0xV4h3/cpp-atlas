#ifndef TOOLSCONFIG_H
#define TOOLSCONFIG_H

#include <QObject>
#include <QString>

/**
 * @brief Singleton configuration manager for external analysis tools.
 *
 * Manages paths and settings for:
 *   - CppInsights binary  (https://github.com/andreasfertig/cppinsights)
 *   - Google Benchmark    (https://github.com/google/benchmark)
 *
 * Configuration is persisted to resources/config/tools.json and can be
 * overridden at runtime via the Tools > Settings dialog (future PR).
 *
 * Pattern mirrors CompilerRegistry (include/compiler/CompilerRegistry.h):
 *   - Singleton via instance()
 *   - loadConfiguration() / saveConfiguration()
 *   - configChanged() signal for change notification
 *
 * Usage:
 * @code
 *   auto& cfg = ToolsConfig::instance();
 *   cfg.loadConfiguration(":/config/tools.json");
 *   if (cfg.isCppInsightsAvailable()) {
 *       QString path = cfg.cppInsightsPath();
 *   }
 * @endcode
 */
class ToolsConfig : public QObject {
    Q_OBJECT

public:
    static ToolsConfig& instance();

    // ----------------------------------------------------------------
    // CppInsights
    // ----------------------------------------------------------------

    /**
     * @brief Path to the `insights` executable.
     * Falls back to compile-time CPPINSIGHTS_DEFAULT_PATH if not set in JSON.
     */
    QString cppInsightsPath() const;

    /**
     * @brief Set the path to the `insights` executable.
     * Does NOT persist; call saveConfiguration() afterwards.
     */
    void setCppInsightsPath(const QString& path);

    /**
     * @brief Check whether the configured insights binary exists and is executable.
     */
    bool isCppInsightsAvailable() const;

    // ----------------------------------------------------------------
    // Google Benchmark
    // ----------------------------------------------------------------

    /**
     * @brief Directory containing Google Benchmark headers (benchmark/benchmark.h).
     * Defaults to the FetchContent-provided path set via CMake.
     */
    QString benchmarkIncludeDir() const;
    void setBenchmarkIncludeDir(const QString& dir);

    /**
     * @brief Name of the benchmark library target or path to libbenchmark.a.
     * Used when compiling user benchmark code via BenchmarkRunner.
     */
    QString benchmarkLibrary() const;
    void setBenchmarkLibrary(const QString& lib);

    /**
     * @brief Check whether Google Benchmark headers are accessible.
     */
    bool isBenchmarkAvailable() const;

    // ----------------------------------------------------------------
    // Benchmark build defaults
    // ----------------------------------------------------------------

    QString defaultBenchmarkCompilerId() const;
    void setDefaultBenchmarkCompilerId(const QString& id);

    QString defaultBenchmarkStandard() const;
    void setDefaultBenchmarkStandard(const QString& standard);

    /** Returns optimization level string: "O0", "O1", "O2", "O3", "Os" */
    QString defaultBenchmarkOptimization() const;
    void setDefaultBenchmarkOptimization(const QString& opt);

    // ----------------------------------------------------------------
    // Persistence — mirrors CompilerRegistry::loadConfiguration()
    // ----------------------------------------------------------------

    /**
     * @brief Load configuration from a JSON file.
     * @param filePath  Path to tools.json (may be a Qt resource ":/config/tools.json").
     * @return true on success
     *
     * JSON schema (resources/config/tools.json):
     * @code
     * {
     *   "tools": {
     *     "cppinsights": { "path": "insights", "autoDetect": true },
     *     "benchmark":   { "includeDir": "", "library": "", "autoDetect": true }
     *   },
     *   "benchmarkDefaults": {
     *     "compiler": "gcc-system",
     *     "standard": "c++17",
     *     "optimizationLevel": "O2"
     *   }
     * }
     * @endcode
     */
    bool loadConfiguration(const QString& filePath);

    /**
     * @brief Save current configuration to a JSON file.
     * @param filePath  Writable file path (NOT a Qt resource path).
     * @return true on success
     */
    bool saveConfiguration(const QString& filePath) const;

    /** Return the file path passed to the last successful loadConfiguration(). */
    QString configFilePath() const;

signals:
    /** Emitted whenever any configuration value changes. */
    void configChanged();

private:
    ToolsConfig();
    ~ToolsConfig() override = default;
    ToolsConfig(const ToolsConfig&) = delete;
    ToolsConfig& operator=(const ToolsConfig&) = delete;

    QString autoDetectCppInsights() const;
    QString autoDetectBenchmarkInclude() const;

    QString m_cppInsightsPath;
    QString m_benchmarkIncludeDir;
    QString m_benchmarkLibrary;
    QString m_defaultCompilerId   = "gcc-system";
    QString m_defaultStandard     = "c++17";
    QString m_defaultOptimization = "O2";
    QString m_configFilePath;

    bool m_insightsAutoDetect  = true;
    bool m_benchmarkAutoDetect = true;
};

#endif // TOOLSCONFIG_H
