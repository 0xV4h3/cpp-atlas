#ifndef BENCHMARKCHARTWIDGET_H
#define BENCHMARKCHARTWIDGET_H

#include <QWidget>
#include "tools/BenchmarkResult.h"

/**
 * @brief Visualizes BenchmarkResult data as interactive charts.
 *
 * When CPPATLAS_CHARTS_AVAILABLE is defined (Qt Charts found at CMake time):
 *   Bar chart    — real_time per benchmark entry (default view after Run).
 *   Line chart   — parametric benchmarks whose names contain "/" (BM_Sort/8,
 *                  BM_Sort/16, …).  Groups by base name; x-axis = parameter.
 *   Comparison   — multiple BenchmarkResult objects side-by-side as grouped
 *                  bar chart (up to MAX_COMPARE runs from BenchmarkWidget).
 *
 * When Qt Charts is NOT available:
 *   Shows a QLabel with platform-specific install instructions.
 *   The rest of the widget API is identical; calls to setResult() /
 *   compareResults() are silently ignored.
 *
 * Theme: connect to ThemeManager::themeChanged → onThemeChanged().
 */
class BenchmarkChartWidget : public QWidget {
    Q_OBJECT

public:
    enum class ChartType {
        Bar,          ///< One bar per BenchmarkEntry (real_time)
        Line,         ///< Parametric benchmarks, x = numeric suffix after "/"
        SpeedupRatio  ///< Normalised to first run (comparison mode)
    };

    explicit BenchmarkChartWidget(QWidget* parent = nullptr);
    ~BenchmarkChartWidget() override = default;

    /** Display results for a single benchmark run. */
    void setResult(const BenchmarkResult& result);

    /** Display two or more runs side-by-side.  Enables grouped bar chart. */
    void compareResults(const QList<BenchmarkResult>& results);

    void      setChartType(ChartType type);
    ChartType chartType() const { return m_chartType; }

public slots:
    /** Called by ThemeManager::themeChanged — updates chart colours. */
    void onThemeChanged(const QString& themeName);

private:
    void setupUi();

#ifdef CPPATLAS_CHARTS_AVAILABLE
    void buildBarChart       (const BenchmarkResult& result);
    void buildLineChart      (const BenchmarkResult& result);
    void buildComparisonChart(const QList<BenchmarkResult>& results);
    void applyChartTheme     (const QString& themeName);

    // m_chartView is only declared when Charts is available.
    class QChartView* m_chartView = nullptr;
#endif

    ChartType m_chartType = ChartType::Bar;
};

#endif // BENCHMARKCHARTWIDGET_H
