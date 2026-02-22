#include "ui/BenchmarkWidget.h"
#include "ui/BenchmarkChartWidget.h"
#include "ui/ThemeManager.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

// ── Construction ─────────────────────────────────────────────────────────────

BenchmarkWidget::BenchmarkWidget(QWidget* parent)
    : QWidget(parent)
    , m_runner(new BenchmarkRunner(this))
{
    setupUi();
    loadTemplate();

    connect(m_runner, &BenchmarkRunner::benchmarkResultReady,
        this, &BenchmarkWidget::onBenchmarkResultReady);
    connect(m_runner, &BenchmarkRunner::compilationFinished,
        this, &BenchmarkWidget::onCompilationFinished);
    connect(m_runner, &BenchmarkRunner::progressMessage,
        this, &BenchmarkWidget::onProgressMessage);
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
        this, &BenchmarkWidget::onThemeChanged);

    onThemeChanged(ThemeManager::instance()->currentThemeName());
}

// ── Public setters (called by MainWindow) ─────────────────────────────────────

void BenchmarkWidget::setCompilerId(const QString& id) {
    m_compilerId = id;
    m_runner->setCompilerId(id);
}

void BenchmarkWidget::setStandard(const QString& standard) {
    m_standard = standard;
}

// ── UI setup ──────────────────────────────────────────────────────────────────

void BenchmarkWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Toolbar
    auto* toolbar = new QWidget(this);
    auto* tbLayout = new QHBoxLayout(toolbar);
    tbLayout->setContentsMargins(6, 4, 6, 4);
    setupToolbar(toolbar, tbLayout);
    mainLayout->addWidget(toolbar);

    // QSplitter: code editor (top 2/3) / results (bottom 1/3)
    auto* splitter = new QSplitter(Qt::Vertical, this);
    setupCodeEditor();
    splitter->addWidget(m_codeEditor);
    setupResultsTabs();
    splitter->addWidget(m_resultsTabs);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter, 1);
}

void BenchmarkWidget::setupToolbar(QWidget* parent, QHBoxLayout* tbLayout) {
    // Optimization level — the only tool-specific combo in this widget
    tbLayout->addWidget(new QLabel(QStringLiteral("Opt:"), parent));
    m_optimizationCombo = new QComboBox(parent);
    m_optimizationCombo->addItems({ "O0", "O1", "O2", "O3" });
    m_optimizationCombo->setCurrentText(QStringLiteral("O2"));
    tbLayout->addWidget(m_optimizationCombo);

    tbLayout->addStretch();

    // Run button — educational tooltip about DoNotOptimize
    m_runButton = new QPushButton(QStringLiteral("▶  Run"), parent);
    m_runButton->setToolTip(
        QStringLiteral(
            "Compile and run the benchmark.\n\n"
            "Educational tips:\n"
            "  benchmark::DoNotOptimize(val)  — prevents the compiler from\n"
            "    optimizing away the measured expression.\n"
            "  benchmark::ClobberMemory()     — forces all pending writes\n"
            "    to memory (use after writing to a buffer).\n\n"
            "Reference: https://github.com/google/benchmark"
            "#preventing-optimisation"));
    connect(m_runButton, &QPushButton::clicked,
        this, &BenchmarkWidget::runBenchmark);
    tbLayout->addWidget(m_runButton);

    m_exportButton = new QPushButton(QStringLiteral("Export..."), parent);
    m_exportButton->setEnabled(false);
    connect(m_exportButton, &QPushButton::clicked,
        this, &BenchmarkWidget::exportResults);
    tbLayout->addWidget(m_exportButton);

    m_compareButton = new QPushButton(QStringLiteral("Compare"), parent);
    m_compareButton->setToolTip(
        QStringLiteral("Show side-by-side chart of saved runs (max %1).")
        .arg(MAX_COMPARE));
    m_compareButton->setEnabled(false);
    connect(m_compareButton, &QPushButton::clicked,
        this, &BenchmarkWidget::onCompareClicked);
    tbLayout->addWidget(m_compareButton);

    m_statusLabel = new QLabel(QStringLiteral("Ready"), parent);
    m_statusLabel->setMinimumWidth(220);
    tbLayout->addWidget(m_statusLabel);
}

void BenchmarkWidget::setupCodeEditor() {
    m_codeEditor = new QsciScintilla(this);
    auto* lexer = new QsciLexerCPP(m_codeEditor);
    QFont font(QStringLiteral("Monospace"), 10);
    lexer->setDefaultFont(font);
    m_codeEditor->setLexer(lexer);
    m_codeEditor->setTabWidth(4);
    m_codeEditor->setIndentationsUseTabs(false);
    m_codeEditor->setAutoIndent(true);
    m_codeEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    m_codeEditor->setMarginType(0, QsciScintilla::NumberMargin);
    m_codeEditor->setMarginWidth(0, QStringLiteral("00000"));
}

void BenchmarkWidget::setupResultsTabs() {
    m_resultsTabs = new QTabWidget(this);

    // Charts tab
    m_chartWidget = new BenchmarkChartWidget(m_resultsTabs);
    m_resultsTabs->addTab(m_chartWidget, QStringLiteral("Charts"));

    // Table tab
    m_tableWidget = new QTableWidget(0, 4, m_resultsTabs);
    m_tableWidget->setHorizontalHeaderLabels({
        QStringLiteral("Name"),
        QStringLiteral("Real Time"),
        QStringLiteral("CPU Time"),
        QStringLiteral("Iterations")
        });
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setStretchLastSection(false);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTabs->addTab(m_tableWidget, QStringLiteral("Table"));

    // Raw JSON tab
    m_rawJsonView = new QPlainTextEdit(m_resultsTabs);
    m_rawJsonView->setReadOnly(true);
    QFont mono(QStringLiteral("Monospace"), 9);
    m_rawJsonView->setFont(mono);
    m_resultsTabs->addTab(m_rawJsonView, QStringLiteral("Raw JSON"));
}

// ── Template loading ──────────────────────────────────────────────────────────

void BenchmarkWidget::loadTemplate() {
    QFile tmpl(QStringLiteral(":/templates/benchmark_template.cpp"));
    if (tmpl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_codeEditor->setText(QString::fromUtf8(tmpl.readAll()));
        return;
    }
    // Inline fallback — always compiles; teaches DoNotOptimize
    m_codeEditor->setText(QStringLiteral(
        "#include <benchmark/benchmark.h>\n\n"
        "// TODO: replace with the code you want to measure.\n"
        "static void BM_Example(benchmark::State& state) {\n"
        "    for (auto _ : state) {\n"
        "        int x = 42;\n"
        "        benchmark::DoNotOptimize(x);\n"
        "    }\n"
        "}\n"
        "BENCHMARK(BM_Example);\n\n"
        "BENCHMARK_MAIN();\n"));
}

// ── Run / Export ──────────────────────────────────────────────────────────────

void BenchmarkWidget::runBenchmark() {
    // Write editor content to a temp .cpp file
    const QString tmpPath =
        QDir::tempPath() + QStringLiteral("/cppatlas_bench_input.cpp");
    QFile f(tmpPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_statusLabel->setText(QStringLiteral("Error: cannot write temp file."));
        return;
    }
    f.write(m_codeEditor->text().toUtf8());
    f.close();

    // Forward current compiler ID to runner
    m_runner->setCompilerId(m_compilerId);

    // Build compiler flags from current standard + selected optimisation
    QStringList flags;
    flags << (QStringLiteral("-std=") + m_standard);
    flags << (QStringLiteral("-") + m_optimizationCombo->currentText());

    m_runButton->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("Compiling..."));
    m_runner->run(tmpPath, flags);
}

void BenchmarkWidget::exportResults() {
    const QString path = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("Export Benchmark Results"),
        QString(),
        QStringLiteral("JSON (*.json);;CSV (*.csv);;All Files (*)"));
    if (path.isEmpty()) return;

    const bool ok = path.endsWith(QStringLiteral(".csv"), Qt::CaseInsensitive)
        ? m_runner->exportToCsv(path)
        : m_runner->exportToJson(path);

    m_statusLabel->setText(
        ok ? QStringLiteral("Exported to %1").arg(path)
        : QStringLiteral("Export failed."));
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void BenchmarkWidget::onCompilationFinished(bool success,
    const QString& error) {
    if (!success) {
        m_runButton->setEnabled(true);
        m_statusLabel->setText(QStringLiteral("Compilation failed."));
        m_rawJsonView->setPlainText(
            QStringLiteral("// Compilation error:\n") + error);
        m_resultsTabs->setCurrentWidget(m_rawJsonView);
    }
    else {
        m_statusLabel->setText(QStringLiteral("Compiled — running..."));
    }
}

void BenchmarkWidget::onProgressMessage(const QString& msg) {
    m_statusLabel->setText(msg);
}

void BenchmarkWidget::onBenchmarkResultReady(const BenchmarkResult& result) {
    m_runButton->setEnabled(true);
    m_exportButton->setEnabled(true);

    updateResultsView(result);

    // Save to compare history with metadata
    BenchmarkResult saved = result;
    saved.compilerId = m_compilerId;
    saved.standard = m_standard;
    saved.optimizationLevel = m_optimizationCombo->currentText();
    saved.label = saved.optimizationLevel;

    if (m_savedResults.size() >= MAX_COMPARE)
        m_savedResults.removeFirst();
    m_savedResults << saved;
    m_compareButton->setEnabled(m_savedResults.size() >= 2);

    m_statusLabel->setText(
        QStringLiteral("Done — %1 benchmark(s)").arg(result.benchmarks.size()));
    emit benchmarkCompleted(result);
}

void BenchmarkWidget::updateResultsView(const BenchmarkResult& result) {
    // Charts
    m_chartWidget->setResult(result);

    // Table
    m_tableWidget->setRowCount(result.benchmarks.size());
    for (int i = 0; i < result.benchmarks.size(); ++i) {
        const BenchmarkEntry& e = result.benchmarks[i];
        const QString unit =
            e.timeUnit.isEmpty() ? QStringLiteral("ns") : e.timeUnit;
        m_tableWidget->setItem(i, 0, new QTableWidgetItem(e.name));
        m_tableWidget->setItem(
            i, 1,
            new QTableWidgetItem(
                QStringLiteral("%1 %2").arg(e.realTimeNs, 0, 'f', 2).arg(unit)));
        m_tableWidget->setItem(
            i, 2,
            new QTableWidgetItem(
                QStringLiteral("%1 %2").arg(e.cpuTimeNs, 0, 'f', 2).arg(unit)));
        m_tableWidget->setItem(
            i, 3,
            new QTableWidgetItem(QString::number(e.iterations)));
    }

    // Raw JSON tab header comment
    m_rawJsonView->setPlainText(
        QStringLiteral("// Benchmark completed: %1\n// %2 benchmark(s)\n")
        .arg(result.date)
        .arg(result.benchmarks.size()));
}

void BenchmarkWidget::onCompareClicked() {
    if (m_savedResults.size() >= 2) {
        m_chartWidget->compareResults(m_savedResults);
        m_resultsTabs->setCurrentWidget(m_chartWidget);
    }
}

// ── Theme ─────────────────────────────────────────────────────────────────────

void BenchmarkWidget::applyThemeToEditor(const QString& themeName) {
    Theme theme;
    if (themeName == QStringLiteral("light"))   theme = ThemeManager::lightTheme();
    else if (themeName == QStringLiteral("dracula")) theme = ThemeManager::draculaTheme();
    else if (themeName == QStringLiteral("monokai")) theme = ThemeManager::monokaiTheme();
    else                                             theme = ThemeManager::darkTheme();

    auto* lexer = qobject_cast<QsciLexerCPP*>(m_codeEditor->lexer());
    if (lexer) {
        lexer->setDefaultPaper(theme.editorBackground);
        lexer->setDefaultColor(theme.editorForeground);
        for (int s = 0; s <= 128; ++s) {
            lexer->setPaper(theme.editorBackground, s);
            lexer->setColor(theme.editorForeground, s);
        }
        lexer->setColor(theme.syntaxKeyword, QsciLexerCPP::Keyword);
        lexer->setColor(theme.syntaxString, QsciLexerCPP::DoubleQuotedString);
        lexer->setColor(theme.syntaxComment, QsciLexerCPP::Comment);
        lexer->setColor(theme.syntaxComment, QsciLexerCPP::CommentLine);
        lexer->setColor(theme.syntaxPreprocessor, QsciLexerCPP::PreProcessor);
        lexer->setColor(theme.syntaxNumber, QsciLexerCPP::Number);
    }
    m_codeEditor->setPaper(theme.editorBackground);
    m_codeEditor->setColor(theme.editorForeground);
    m_codeEditor->setCaretLineVisible(true);
    m_codeEditor->setCaretLineBackgroundColor(theme.editorCurrentLine);
    m_codeEditor->setMarginsBackgroundColor(theme.sidebarBackground);
    m_codeEditor->setMarginsForegroundColor(theme.textSecondary);
    // Fix: ensure fold margin doesn't leak white stripe on theme change
    m_codeEditor->setFoldMarginColors(theme.sidebarBackground,
        theme.sidebarBackground);
    m_codeEditor->recolor();
}

void BenchmarkWidget::onThemeChanged(const QString& themeName) {
    applyThemeToEditor(themeName);
    m_chartWidget->onThemeChanged(themeName);
}