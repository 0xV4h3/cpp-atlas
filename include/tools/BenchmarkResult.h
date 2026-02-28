#ifndef BENCHMARKRESULT_H
#define BENCHMARKRESULT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>

/**
 * @brief One entry from Google Benchmark JSON output.
 *
 * JSON reference:
 *   https://github.com/google/benchmark#output-formats
 *
 * Fields map directly to the per-benchmark object in the
 * "benchmarks" array of --benchmark_format=json output.
 */
struct BenchmarkEntry {
    QString name;            // e.g. "BM_VectorFill/8"
    double  realTimeNs = 0;  // "real_time" (time_unit applied)
    double  cpuTimeNs  = 0;  // "cpu_time"
    qint64  iterations = 0;
    QString timeUnit;        // "ns", "us", "ms"

    // Custom counters declared via benchmark::Counter
    QMap<QString, QVariant> counters;
};

/**
 * @brief Full result of one benchmark binary execution.
 */
struct BenchmarkResult {
    bool    success = false;
    QString errorMessage;

    QString date;            // ISO timestamp from JSON "context.date"
    QString executablePath;  // Temporary binary path (informational)

    QList<BenchmarkEntry> benchmarks;

    // Raw JSON string from --benchmark_format=json stdout
    QString rawJson;

    // Metadata used by the Compare view
    QString compilerId;
    QString standard;
    QString optimizationLevel;
    QString label;           // User-visible label for side-by-side chart
};

#endif // BENCHMARKRESULT_H
