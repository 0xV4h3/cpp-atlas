#ifndef BENCHMARKWIDGET_H
#define BENCHMARKWIDGET_H

#include <QColor>
#include <QEvent>
#include <QScopedPointer>
#include <QWidget>
#include <QTemporaryFile>
#include "tools/BenchmarkRunner.h"
#include "tools/BenchmarkResult.h"

class QsciScintilla;
class QComboBox;
class QPushButton;
class QLabel;
class QTabWidget;
class QTableWidget;
class QScrollArea;
class QVBoxLayout;
class QPlainTextEdit;
class BenchmarkChartWidget;

// ── Result record ─────────────────────────────────────────────────────────────

/**
 * @brief One accumulated benchmark result with user-assigned display metadata.
 *
 * Stored in m_records; the Results Manager tab lets the user edit name/color
 * and choose which record is shown in Charts/Table/JSON and which records
 * participate in Comparison.
 */
struct BenchmarkResultRecord {
    BenchmarkResult result;

    // User-editable display metadata
    QString userLabel;
    QColor  color;

    // Selection state
    bool showInView   = false;
    bool inComparison = false;
};

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

    void setCompilerId(const QString& id);
    void setStandard(const QString& standard);

public slots:
    void runBenchmark();
    void exportResults();
    void loadTemplate();
    void onThemeChanged(const QString& themeName);
    void openBenchmarkFile();
    void saveBenchmarkFile();
    void importResults();
    void applyEditorSettings(const QFont& font, bool showLineNumbers, bool wordWrap);

signals:
    void benchmarkCompleted(const BenchmarkResult& result);

private slots:
    void onBenchmarkResultReady(const BenchmarkResult& result);
    void onCompilationFinished(bool success, const QString& error);
    void onProgressMessage(const QString& msg);
    void onCompareClicked();
    void stopProcess();

    // Results Manager slots
    void onResultColorClicked(int row);
    void onDeleteResultClicked(int row);
    void onExportResultClicked(int row);
    void onClearAllResults();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    // ── Layout ────────────────────────────────────────────────────────────────
    void setupUi();
    void setupToolbar(QWidget* parent, class QHBoxLayout* layout);
    void setupCodeEditor();
    void setupResultsTabs();
    void setupResultsManagerTab();

    // ── Helpers ───────────────────────────────────────────────────────────────
    void addRecord(const BenchmarkResult& result,
                   const QString& userLabel = QString(),
                   const QColor&  color     = QColor());

    /** Rebuild the Results Manager rows from m_records. */
    void refreshResultsTable();

    /** Show the single record marked showInView in Charts / Table / Raw JSON. */
    void refreshDisplayedResult();

    /** Push all records with inComparison=true to the Comparison chart. */
    void refreshComparison();

    /** Apply display metadata from m_records[row] to the BenchmarkResult
     *  passed to buildComparisonChart so bar colors and labels are correct. */
    BenchmarkResult decoratedResult(int recordIndex) const;

    void updateResultsView(const BenchmarkResult& result);
    void applyThemeToEditor(const QString& themeName);

    // ── Code editor helpers ───────────────────────────────────────────────────
    QsciScintilla* currentBenchEditor() const;
    QString        currentBenchFilePath() const;
    void addBenchTab(const QString& title, const QString& filePath, const QString& content);
    bool closeBenchTab(int index);
    void loadTemplateInCurrentTab();

    // ── Toolbar widgets ───────────────────────────────────────────────────────
    QComboBox*   m_optimizationCombo = nullptr;
    QPushButton* m_openFileButton    = nullptr;
    QPushButton* m_saveFileButton    = nullptr;
    QPushButton* m_importButton      = nullptr;
    QPushButton* m_runButton         = nullptr;
    QPushButton* m_stopButton        = nullptr;
    QPushButton* m_exportButton      = nullptr;
    QPushButton* m_compareButton     = nullptr;
    QLabel*      m_statusLabel       = nullptr;

    // ── Code editor tabs ──────────────────────────────────────────────────────
    QTabWidget* m_editorTabs      = nullptr;
    int         m_newBenchCounter = 1;

    // ── Results tabs ──────────────────────────────────────────────────────────
    QTabWidget*           m_resultsTabs            = nullptr;
    BenchmarkChartWidget* m_chartWidget            = nullptr;
    BenchmarkChartWidget* m_comparisonChartWidget  = nullptr;
    QTableWidget*         m_tableWidget            = nullptr;
    QPlainTextEdit*       m_rawJsonView            = nullptr;

    // Results Manager tab (index 4) — scroll area of QFrame rows, no QTableWidget
    QScrollArea*  m_recordsScrollArea = nullptr;
    QWidget*      m_recordsContainer  = nullptr;
    QVBoxLayout*  m_recordsLayout     = nullptr;
    QPushButton*  m_clearAllBtn       = nullptr;

    // ── Backend ───────────────────────────────────────────────────────────────
    BenchmarkRunner* m_runner = nullptr;

    // ── State ─────────────────────────────────────────────────────────────────
    QString m_compilerId;
    QString m_standard = QStringLiteral("c++17");
    QScopedPointer<QTemporaryFile> m_tempBenchSource;

    QList<BenchmarkResultRecord> m_records;

    // Palette for auto-assigning colors to new records
    static const QList<QColor>& recordPalette();
};

#endif // BENCHMARKWIDGET_H
