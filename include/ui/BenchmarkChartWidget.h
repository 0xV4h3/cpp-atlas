#ifndef BENCHMARKCHARTWIDGET_H
#define BENCHMARKCHARTWIDGET_H

#include "tools/BenchmarkResult.h"
#include <QWidget>

/**
 * @brief Widget that visualises Google Benchmark results as charts.
 *
 * When CPPATLAS_CHARTS_AVAILABLE is defined (Qt Charts present), renders
 * a bar chart of real-time values.  Otherwise falls back to a plain-text
 * summary so the rest of the UI still compiles and runs without Qt Charts.
 *
 * Public API used by BenchmarkWidget:
 *   setResult()       — display results from a single run
 *   compareResults()  — display side-by-side bars for multiple runs
 *   onThemeChanged()  — propagate theme colours into the chart
 */
class BenchmarkChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit BenchmarkChartWidget(QWidget* parent = nullptr);
    ~BenchmarkChartWidget() override = default;

    /** Display the results from one benchmark run. */
    void setResult(const BenchmarkResult& result);

    /** Display side-by-side bars for multiple runs (Compare view). */
    void compareResults(const QList<BenchmarkResult>& results);

public slots:
    void onThemeChanged(const QString& themeName);

private:
    void buildChart(const QList<BenchmarkResult>& results);
    void buildFallback(const QList<BenchmarkResult>& results);

#ifdef CPPATLAS_CHARTS_AVAILABLE
    class QChartView* m_chartView = nullptr;
#else
    class QPlainTextEdit* m_textView = nullptr;
#endif
};

#endif // BENCHMARKCHARTWIDGET_H
