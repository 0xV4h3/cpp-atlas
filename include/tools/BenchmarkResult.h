#ifndef BENCHMARKRESULT_H
#define BENCHMARKRESULT_H

#include <QColor>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>

/**
 * @brief One entry from Google Benchmark JSON output.
 */
struct BenchmarkEntry {
    QString name;
    double  realTimeNs = 0;
    double  cpuTimeNs  = 0;
    qint64  iterations = 0;
    QString timeUnit;
    QMap<QString, QVariant> counters;
};

/**
 * @brief Full result of one benchmark binary execution.
 */
struct BenchmarkResult {
    bool    success = false;
    QString errorMessage;

    QString date;
    QString executablePath;

    QList<BenchmarkEntry> benchmarks;
    QString rawJson;

    // Metadata used by the Compare view
    QString compilerId;
    QString standard;
    QString optimizationLevel;
    QString label;          ///< User-visible label for side-by-side chart legend
    QColor  displayColor;   ///< Bar color in Comparison chart (assigned by BenchmarkWidget)
};

#endif // BENCHMARKRESULT_H
