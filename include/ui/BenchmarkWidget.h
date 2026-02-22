#ifndef BENCHMARKWIDGET_H
#define BENCHMARKWIDGET_H

#include <QWidget>
#include "tools/BenchmarkRunner.h"
#include "tools/BenchmarkResult.h"

class QsciScintilla;
class QComboBox;
class QPushButton;
class QLabel;
class QTabWidget;
class QTableWidget;
class QPlainTextEdit;
class BenchmarkChartWidget;

/**
 * @brief Full benchmark authoring and results widget.
 *
 * Layout:
 *   ┌─ Toolbar: [Opt] [▶ Run] [Export...] [Compare] [status] ──────────┐
 *   │  (NO compiler / standard combo — received via setCompilerId /     │
 *   │   setStandard from MainWindow, exactly like AssemblyWidget)        │
 *   ├─ QsciScintilla code editor (pre-loaded with benchmark_template)  ─┤
 *   └─ QTabWidget results:                                              ─┘
 *       "Charts"   — BenchmarkChartWidget (bar / line / comparison)
 *       "Table"    — QTableWidget: Name | Real Time | CPU Time | Iters
 *       "Raw JSON" — QPlainTextEdit, raw --benchmark_format=json output
 *
 * Compare:
 *   Saves up to MAX_COMPARE (5) results.  "Compare" button enabled once
 *   ≥ 2 results are saved; passes them to BenchmarkChartWidget::compareResults().
 *
 * Theme:
 *   Reacts to ThemeManager::themeChanged; propagates to code editor and chart.
 *
 * Educational hints:
 *   Run button tooltip explains DoNotOptimize() / ClobberMemory() with link
 *   to https://github.com/google/benchmark#preventing-optimisation
 */
class BenchmarkWidget : public QWidget {
    Q_OBJECT

public:
    explicit BenchmarkWidget(QWidget* parent = nullptr);
    ~BenchmarkWidget() override = default;

    // ── Set from MainWindow ───────────────────────────────────────
    /**
     * @brief Called by MainWindow when the toolbar compiler changes.
     * Forwarded directly to BenchmarkRunner::setCompilerId().
     */
    void setCompilerId(const QString& id);

    /**
     * @brief Called by MainWindow when the toolbar standard changes.
     * Stored and passed as -std=<standard> flag on next Run.
     */
    void setStandard(const QString& standard);

public slots:
    void runBenchmark();
    void exportResults();
    void loadTemplate();
    void onThemeChanged(const QString& themeName);

signals:
    void benchmarkCompleted(const BenchmarkResult& result);

private slots:
    void onBenchmarkResultReady(const BenchmarkResult& result);
    void onCompilationFinished(bool success, const QString& error);
    void onProgressMessage(const QString& msg);
    void onCompareClicked();

private:
    void setupUi();
    void setupToolbar(QWidget* parent, class QHBoxLayout* layout);
    void setupCodeEditor();
    void setupResultsTabs();
    void updateResultsView(const BenchmarkResult& result);
    void applyThemeToEditor(const QString& themeName);

    // ── Toolbar ────────────────────────────────────────────────────
    QComboBox* m_optimizationCombo = nullptr;
    QPushButton* m_runButton = nullptr;
    QPushButton* m_exportButton = nullptr;
    QPushButton* m_compareButton = nullptr;
    QLabel* m_statusLabel = nullptr;

    // ── Code editor ────────────────────────────────────────────────
    QsciScintilla* m_codeEditor = nullptr;

    // ── Results ────────────────────────────────────────────────────
    QTabWidget* m_resultsTabs = nullptr;
    BenchmarkChartWidget* m_chartWidget = nullptr;
    QTableWidget* m_tableWidget = nullptr;
    QPlainTextEdit* m_rawJsonView = nullptr;

    // ── Backend ────────────────────────────────────────────────────
    BenchmarkRunner* m_runner = nullptr;

    // ── State ──────────────────────────────────────────────────────
    QString m_compilerId;
    QString m_standard = QStringLiteral("c++17");

    static constexpr int MAX_COMPARE = 5;
    QList<BenchmarkResult> m_savedResults;
};

#endif // BENCHMARKWIDGET_H