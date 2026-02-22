#ifndef ANALYSISPANEL_H
#define ANALYSISPANEL_H

#include <QTabWidget>

class InsightsWidget;
class AssemblyWidget;
class BenchmarkWidget;

/**
 * @brief Unified QTabWidget hosting InsightsWidget, AssemblyWidget,
 *        and BenchmarkWidget.
 *
 * MainWindow owns one AnalysisPanel inside AnalysisDock (right side,
 * hidden by default).  All synchronisation with EditorTabWidget passes
 * through this class.
 *
 * API contract:
 *   setSourceCode(code, path) — propagates to InsightsWidget + AssemblyWidget
 *   setCompilerId(id)         — propagates to AssemblyWidget + BenchmarkWidget
 *   setStandard(std)          — propagates to AssemblyWidget + BenchmarkWidget
 *
 * Signals forwarded to MainWindow:
 *   sourceLineActivated(int line) — from AssemblyWidget; navigates editor
 */
class AnalysisPanel : public QTabWidget {
    Q_OBJECT

public:
    explicit AnalysisPanel(QWidget* parent = nullptr);
    ~AnalysisPanel() override = default;

    // ── Accessors (for MainWindow if direct access is needed) ────
    InsightsWidget*  insightsWidget()  const { return m_insights;   }
    AssemblyWidget*  assemblyWidget()  const { return m_assembly;   }
    BenchmarkWidget* benchmarkWidget() const { return m_benchmark;  }

    // ── Synchronisation API (called by MainWindow) ───────────────

    /**
     * @brief Forward active editor source to InsightsWidget and AssemblyWidget.
     * Does NOT auto-run either tool.
     */
    void setSourceCode(const QString& code, const QString& filePath);

    /**
     * @brief Forward compiler ID to AssemblyWidget and BenchmarkWidget.
     * Called when MainWindow toolbar compiler combo changes.
     */
    void setCompilerId(const QString& id);

    /**
     * @brief Forward C++ standard to AssemblyWidget and BenchmarkWidget.
     * Called when MainWindow toolbar standard combo changes.
     */
    void setStandard(const QString& standard);

    // ── Tab indices (for programmatic tab switching) ──────────────
    static constexpr int TabInsights  = 0;
    static constexpr int TabAssembly  = 1;
    static constexpr int TabBenchmark = 2;

signals:
    /**
     * Forwarded from AssemblyWidget::sourceLineActivated.
     * MainWindow connects this to editor->gotoLine().
     */
    void sourceLineActivated(int line);

private:
    InsightsWidget*  m_insights  = nullptr;
    AssemblyWidget*  m_assembly  = nullptr;
    BenchmarkWidget* m_benchmark = nullptr;
};

#endif // ANALYSISPANEL_H
