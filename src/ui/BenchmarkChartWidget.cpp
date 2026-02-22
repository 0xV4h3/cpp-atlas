#include "ui/BenchmarkChartWidget.h"
#include "ui/ThemeManager.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

#ifdef CPPATLAS_CHARTS_AVAILABLE
// Qt Charts includes — only compiled when the module is available
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Qt5 only: QT_CHARTS_USE_NAMESPACE
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
QT_CHARTS_USE_NAMESPACE
#endif

#endif

// ── Construction ─────────────────────────────────────────────────────────────

BenchmarkChartWidget::BenchmarkChartWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &BenchmarkChartWidget::onThemeChanged);
}

// ── UI setup ──────────────────────────────────────────────────────────────────

void BenchmarkChartWidget::setupUi() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef CPPATLAS_CHARTS_AVAILABLE
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(m_chartView);
#else
    auto* label = new QLabel(
        QStringLiteral(
            "Qt Charts not available.\n\n"
            "Install it to enable chart visualization:\n\n"
            "  Ubuntu/Debian (Qt 6):\n"
            "    sudo apt install qt6-charts-dev\n\n"
            "  Ubuntu/Debian (Qt 5):\n"
            "    sudo apt install libqt5charts5-dev\n\n"
            "  macOS (Homebrew):\n"
            "    brew install qt\n\n"
            "After installation, re-run CMake and rebuild."),
        this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);
#endif
}

// ── Public API ────────────────────────────────────────────────────────────────

void BenchmarkChartWidget::setResult(const BenchmarkResult& result) {
#ifdef CPPATLAS_CHARTS_AVAILABLE
    switch (m_chartType) {
        case ChartType::Bar:
        case ChartType::SpeedupRatio:
            buildBarChart(result);
            break;
        case ChartType::Line:
            buildLineChart(result);
            break;
    }
#else
    Q_UNUSED(result);
#endif
}

void BenchmarkChartWidget::compareResults(
    const QList<BenchmarkResult>& results)
{
#ifdef CPPATLAS_CHARTS_AVAILABLE
    buildComparisonChart(results);
#else
    Q_UNUSED(results);
#endif
}

void BenchmarkChartWidget::setChartType(ChartType type) {
    m_chartType = type;
}

// ── Chart builders (only compiled with Qt Charts) ─────────────────────────────

#ifdef CPPATLAS_CHARTS_AVAILABLE

namespace {
/** Shorten a benchmark name to at most maxLen characters. */
QString shortName(const QString& name, int maxLen = 30) {
    return name.length() > maxLen
        ? name.left(maxLen - 3) + QStringLiteral("...")
        : name;
}

/** Apply theme colours to a single chart axis. */
void styleAxis(QAbstractAxis* axis, const Theme& theme) {
    axis->setLabelsBrush(QBrush(theme.textPrimary));
    axis->setTitleBrush(QBrush(theme.textPrimary));
    axis->setLinePen(QPen(theme.border));
    axis->setGridLinePen(QPen(theme.border.lighter(130)));
}
} // namespace

void BenchmarkChartWidget::buildBarChart(const BenchmarkResult& result) {
    auto* barSet = new QBarSet(QStringLiteral("Real Time"));
    QStringList categories;

    for (const BenchmarkEntry& e : result.benchmarks) {
        *barSet << e.realTimeNs;
        categories << shortName(e.name);
    }

    auto* series = new QBarSeries();
    series->append(barSet);

    auto* chart = new QChart();
    chart->setTitle(QStringLiteral("Benchmark Results — Real Time (%1)")
                        .arg(result.benchmarks.isEmpty()
                                 ? QStringLiteral("ns")
                                 : result.benchmarks.first().timeUnit));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(series);

    auto* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis();
    axisY->setTitleText(
        result.benchmarks.isEmpty()
            ? QStringLiteral("Time (ns)")
            : QStringLiteral("Time (%1)").arg(
                  result.benchmarks.first().timeUnit));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    m_chartView->setChart(chart);
    applyChartTheme(ThemeManager::instance()->currentThemeName());
}

void BenchmarkChartWidget::buildLineChart(const BenchmarkResult& result) {
    // Detect parametric benchmarks by "/" in their name.
    // Group by base name; x-axis = numeric value after the last "/".
    QMap<QString, QLineSeries*> seriesMap;

    for (const BenchmarkEntry& e : result.benchmarks) {
        const int slashIdx = e.name.lastIndexOf(QLatin1Char('/'));
        const QString baseName =
            (slashIdx > 0) ? e.name.left(slashIdx) : e.name;
        bool ok = false;
        const double xVal =
            (slashIdx > 0) ? e.name.mid(slashIdx + 1).toDouble(&ok) : 0.0;

        if (!seriesMap.contains(baseName)) {
            auto* s = new QLineSeries();
            s->setName(baseName);
            seriesMap[baseName] = s;
        }
        seriesMap[baseName]->append(ok ? xVal : 0.0, e.realTimeNs);
    }

    auto* chart = new QChart();
    chart->setTitle(QStringLiteral("Parametric Benchmark — Real Time (ns)"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    for (auto* s : seriesMap.values()) chart->addSeries(s);
    chart->createDefaultAxes();

    m_chartView->setChart(chart);
    applyChartTheme(ThemeManager::instance()->currentThemeName());
}

void BenchmarkChartWidget::buildComparisonChart(
    const QList<BenchmarkResult>& results)
{
    if (results.isEmpty()) return;

    // Use benchmark names from the first result as x-axis categories.
    QStringList categories;
    for (const BenchmarkEntry& e : results.first().benchmarks)
        categories << shortName(e.name, 20);

    auto* series = new QBarSeries();
    for (const BenchmarkResult& r : results) {
        const QString label =
            r.label.isEmpty() ? r.optimizationLevel : r.label;
        auto* barSet = new QBarSet(label);
        for (const BenchmarkEntry& e : r.benchmarks)
            *barSet << e.realTimeNs;
        series->append(barSet);
    }

    auto* chart = new QChart();
    chart->setTitle(
        QStringLiteral("Benchmark Comparison — Real Time (ns)"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(series);

    auto* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis();
    axisY->setTitleText(QStringLiteral("Time (ns)"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chartView->setChart(chart);
    applyChartTheme(ThemeManager::instance()->currentThemeName());
}

void BenchmarkChartWidget::applyChartTheme(const QString& themeName) {
    if (!m_chartView || !m_chartView->chart()) return;

    Theme theme;
    if      (themeName == QStringLiteral("light"))   theme = ThemeManager::lightTheme();
    else if (themeName == QStringLiteral("dracula")) theme = ThemeManager::draculaTheme();
    else if (themeName == QStringLiteral("monokai")) theme = ThemeManager::monokaiTheme();
    else                                             theme = ThemeManager::darkTheme();

    QChart* chart = m_chartView->chart();
    chart->setBackgroundBrush(QBrush(theme.panelBackground));
    chart->setTitleBrush(QBrush(theme.textPrimary));
    chart->legend()->setLabelBrush(QBrush(theme.textPrimary));

    // Propagate to all axes
    for (QAbstractAxis* axis : chart->axes())
        styleAxis(axis, theme);

    m_chartView->setBackgroundBrush(QBrush(theme.panelBackground));
}

#endif // CPPATLAS_CHARTS_AVAILABLE

// ── Theme slot ────────────────────────────────────────────────────────────────

void BenchmarkChartWidget::onThemeChanged(const QString& themeName) {
#ifdef CPPATLAS_CHARTS_AVAILABLE
    applyChartTheme(themeName);
#else
    Q_UNUSED(themeName);
#endif
}
