#include "ui/BenchmarkChartWidget.h"

#include <QVBoxLayout>
#include <QPainter>

#ifdef CPPATLAS_CHARTS_AVAILABLE
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE
#else
#include <QPlainTextEdit>
#endif

BenchmarkChartWidget::BenchmarkChartWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef CPPATLAS_CHARTS_AVAILABLE
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(m_chartView);
#else
    m_textView = new QPlainTextEdit(this);
    m_textView->setReadOnly(true);
    layout->addWidget(m_textView);
#endif
}

void BenchmarkChartWidget::setResult(const BenchmarkResult& result) {
    compareResults({result});
}

void BenchmarkChartWidget::compareResults(const QList<BenchmarkResult>& results) {
    if (results.isEmpty()) return;
    buildChart(results);
}

void BenchmarkChartWidget::buildChart(const QList<BenchmarkResult>& results) {
#ifdef CPPATLAS_CHARTS_AVAILABLE
    auto* chart = new QChart();
    chart->setTitle(QStringLiteral("Benchmark Results"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Collect all benchmark names across all results
    QStringList categories;
    for (const BenchmarkResult& r : results) {
        for (const BenchmarkEntry& e : r.benchmarks) {
            if (!categories.contains(e.name))
                categories << e.name;
        }
    }

    // Shared axes for all series
    auto* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    auto* axisY = new QValueAxis();
    axisY->setTitleText(QStringLiteral("Time"));

    for (const BenchmarkResult& r : results) {
        auto* barSet = new QBarSet(r.label.isEmpty() ? r.optimizationLevel : r.label);
        for (const QString& cat : categories) {
            double val = 0.0;
            for (const BenchmarkEntry& e : r.benchmarks) {
                if (e.name == cat) { val = e.realTimeNs; break; }
            }
            *barSet << val;
        }
        auto* series = new QBarSeries();
        series->append(barSet);
        chart->addSeries(series);
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for (QAbstractSeries* s : chart->series()) {
        s->attachAxis(axisX);
        s->attachAxis(axisY);
    }

    m_chartView->setChart(chart);
#else
    buildFallback(results);
#endif
}

void BenchmarkChartWidget::buildFallback(const QList<BenchmarkResult>& results) {
#ifndef CPPATLAS_CHARTS_AVAILABLE
    QString text;
    for (const BenchmarkResult& r : results) {
        const QString label = r.label.isEmpty() ? r.optimizationLevel : r.label;
        text += QStringLiteral("=== %1 ===\n").arg(label);
        for (const BenchmarkEntry& e : r.benchmarks) {
            text += QStringLiteral("  %1: %2 %3 (CPU: %4 %5), %6 iters\n")
                        .arg(e.name)
                        .arg(e.realTimeNs, 0, 'f', 2)
                        .arg(e.timeUnit)
                        .arg(e.cpuTimeNs, 0, 'f', 2)
                        .arg(e.timeUnit)
                        .arg(e.iterations);
        }
        text += QLatin1Char('\n');
    }
    m_textView->setPlainText(text);
#else
    Q_UNUSED(results)
#endif
}

void BenchmarkChartWidget::onThemeChanged(const QString& themeName) {
    Q_UNUSED(themeName)
#ifdef CPPATLAS_CHARTS_AVAILABLE
    if (m_chartView && m_chartView->chart()) {
        const bool dark = (themeName != QStringLiteral("light"));
        m_chartView->chart()->setTheme(
            dark ? QChart::ChartThemeDark : QChart::ChartThemeLight);
    }
#endif
}
