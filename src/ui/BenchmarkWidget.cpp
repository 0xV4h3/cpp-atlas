#include "ui/BenchmarkWidget.h"
#include "ui/BenchmarkChartWidget.h"
#include "ui/ThemeManager.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTemporaryFile>
#include <QTextStream>
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
    splitter->addWidget(m_editorTabs);
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

    m_openFileButton = new QPushButton(QStringLiteral("Open..."), parent);
    m_openFileButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    m_openFileButton->setToolTip(QStringLiteral("Open benchmark file (Ctrl+Shift+O)"));
    connect(m_openFileButton, &QPushButton::clicked,
        this, &BenchmarkWidget::openBenchmarkFile);
    tbLayout->addWidget(m_openFileButton);

    m_saveFileButton = new QPushButton(QStringLiteral("Save"), parent);
    m_saveFileButton->setEnabled(false);
    m_saveFileButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    m_saveFileButton->setToolTip(QStringLiteral("Save benchmark file (Ctrl+Shift+S)"));
    connect(m_saveFileButton, &QPushButton::clicked,
        this, &BenchmarkWidget::saveBenchmarkFile);
    tbLayout->addWidget(m_saveFileButton);

    m_importButton = new QPushButton(QStringLiteral("Import..."), parent);
    m_importButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    m_importButton->setToolTip(QStringLiteral("Import results (Ctrl+Shift+I)"));
    connect(m_importButton, &QPushButton::clicked,
        this, &BenchmarkWidget::importResults);
    tbLayout->addWidget(m_importButton);

    // Run button — educational tooltip about DoNotOptimize
    m_runButton = new QPushButton(QStringLiteral("▶  Run"), parent);
    m_runButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F9));
    m_runButton->setToolTip(
        QStringLiteral(
            "Run Benchmark (Ctrl+F9)\n\n"
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

    m_stopButton = new QPushButton(QStringLiteral("■ Stop"), parent);
    m_stopButton->setEnabled(false);
    m_stopButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F9));
    connect(m_stopButton, &QPushButton::clicked,
        this, &BenchmarkWidget::stopProcess);
    tbLayout->addWidget(m_stopButton);

    m_exportButton = new QPushButton(QStringLiteral("Export..."), parent);
    m_exportButton->setEnabled(false);
    m_exportButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_E));
    m_exportButton->setToolTip(QStringLiteral("Export results (Ctrl+Shift+E)"));
    connect(m_exportButton, &QPushButton::clicked,
        this, &BenchmarkWidget::exportResults);
    tbLayout->addWidget(m_exportButton);

    m_compareButton = new QPushButton(QStringLiteral("Compare"), parent);
    m_compareButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_compareButton->setToolTip(
        QStringLiteral("Compare saved results (Ctrl+Shift+C) — max %1 runs.")
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
    m_editorTabs = new QTabWidget(this);
    m_editorTabs->setTabsClosable(true);
    m_editorTabs->setMovable(true);

    // "+" button to add a new benchmark tab
    auto* addTabBtn = new QPushButton(QStringLiteral("+"), m_editorTabs);
    addTabBtn->setFixedSize(24, 24);
    addTabBtn->setToolTip(QStringLiteral("New benchmark file"));
    m_editorTabs->setCornerWidget(addTabBtn, Qt::TopRightCorner);
    connect(addTabBtn, &QPushButton::clicked, this, [this]() {
        addBenchTab(
            QStringLiteral("benchmark-%1.cpp").arg(m_newBenchCounter++),
            QString(), QString());
        loadTemplateInCurrentTab();
    });

    connect(m_editorTabs, &QTabWidget::tabCloseRequested,
            this, &BenchmarkWidget::closeBenchTab);
    connect(m_editorTabs, &QTabWidget::currentChanged,
            this, [this](int) {
        m_saveFileButton->setEnabled(!currentBenchFilePath().isEmpty());
    });

    // Create the first tab
    addBenchTab(QStringLiteral("benchmark-1.cpp"), QString(), QString());
    m_newBenchCounter = 2; // next "+" will create benchmark-2.cpp
}

QsciScintilla* BenchmarkWidget::currentBenchEditor() const {
    return qobject_cast<QsciScintilla*>(m_editorTabs->currentWidget());
}

QString BenchmarkWidget::currentBenchFilePath() const {
    int idx = m_editorTabs->currentIndex();
    if (idx < 0) return QString();
    return m_editorTabs->tabBar()->tabData(idx).toString();
}

void BenchmarkWidget::addBenchTab(const QString& title,
                                   const QString& filePath,
                                   const QString& content) {
    auto* editor = new QsciScintilla(m_editorTabs);
    auto* lexer  = new QsciLexerCPP(editor);
    QFont font(QStringLiteral("Monospace"), 10);
    lexer->setDefaultFont(font);
    editor->setLexer(lexer);
    editor->setTabWidth(4);
    editor->setIndentationsUseTabs(false);
    editor->setAutoIndent(true);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setMarginType(0, QsciScintilla::NumberMargin);
    editor->setMarginWidth(0, QStringLiteral("00000"));
    editor->setWrapMode(QsciScintilla::WrapWord);
    editor->SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0);

    if (!content.isEmpty())
        editor->setText(content);

    int idx = m_editorTabs->addTab(editor, title);
    m_editorTabs->setCurrentIndex(idx);
    m_editorTabs->tabBar()->setTabData(idx, filePath);

    connect(editor, &QsciScintilla::textChanged, this, [this, editor]() {
        int i = m_editorTabs->indexOf(editor);
        if (i >= 0) {
            QString t = m_editorTabs->tabText(i);
            if (!t.endsWith(QLatin1Char('*')))
                m_editorTabs->setTabText(i, t + QLatin1Char('*'));
        }
        if (m_saveFileButton)
            m_saveFileButton->setEnabled(true);
    });

    applyThemeToEditor(ThemeManager::instance()->currentThemeName());
}

bool BenchmarkWidget::closeBenchTab(int index) {
    if (m_editorTabs->count() <= 1)
        return false; // keep at least one tab
    m_editorTabs->removeTab(index);
    return true;
}

void BenchmarkWidget::loadTemplateInCurrentTab() {
    auto* editor = currentBenchEditor();
    if (!editor) return;
    QFile tmpl(QStringLiteral(":/templates/benchmark_template.cpp"));
    if (tmpl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        editor->setText(QString::fromUtf8(tmpl.readAll()));
        return;
    }
    editor->setText(QStringLiteral(
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

    // Comparison tab — dedicated to side-by-side multi-result chart
    m_comparisonChartWidget = new BenchmarkChartWidget(m_resultsTabs);
    m_resultsTabs->addTab(m_comparisonChartWidget, QStringLiteral("Comparison"));
    m_resultsTabs->setTabEnabled(3, false); // disabled until Compare is clicked
}

// ── Template loading ──────────────────────────────────────────────────────────

void BenchmarkWidget::loadTemplate() {
    loadTemplateInCurrentTab();
}

// ── Run / Export ──────────────────────────────────────────────────────────────

void BenchmarkWidget::runBenchmark() {
    auto* editor = currentBenchEditor();
    if (!editor) return;

    QString sourceToRun;
    const QString filePath = currentBenchFilePath();

    if (!filePath.isEmpty()) {
        // Save editor content to the current file path
        QFile f(filePath);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text))
            QTextStream(&f) << editor->text();
        sourceToRun = filePath;
    } else {
        // Use QTemporaryFile with auto-remove
        m_tempBenchSource.reset(new QTemporaryFile(
            QDir::tempPath() + QStringLiteral("/cppatlas_bench_XXXXXX.cpp")));
        m_tempBenchSource->setAutoRemove(true);
        if (!m_tempBenchSource->open()) {
            m_statusLabel->setText(QStringLiteral("Error: cannot write temp file."));
            return;
        }
        QTextStream(m_tempBenchSource.get()) << editor->text();
        m_tempBenchSource->flush();
        sourceToRun = m_tempBenchSource->fileName();
    }

    // Forward current compiler ID to runner
    m_runner->setCompilerId(m_compilerId);

    // Build compiler flags from current standard + selected optimisation
    QStringList flags;
    flags << (QStringLiteral("-std=") + m_standard);
    flags << (QStringLiteral("-") + m_optimizationCombo->currentText());

    m_runButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_statusLabel->setText(QStringLiteral("Compiling..."));
    m_runner->run(sourceToRun, flags);
}

void BenchmarkWidget::exportResults() {
    QString path = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("Export Benchmark Results"),
        QString(),
        QStringLiteral("JSON (*.json);;CSV (*.csv);;All Files (*)"));
    if (path.isEmpty()) return;

    if (!path.endsWith(QStringLiteral(".csv"), Qt::CaseInsensitive) &&
        !path.endsWith(QStringLiteral(".json"), Qt::CaseInsensitive)) {
        path += QStringLiteral(".json");
    }

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
        m_tempBenchSource.reset(); // cleanup on compile failure
        m_runButton->setEnabled(true);
        m_stopButton->setEnabled(false);
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
    m_tempBenchSource.reset(); // cleanup temp source file
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
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

    // Raw JSON tab
    QString rawContent;
    rawContent += QStringLiteral("// Benchmark completed: %1\n// %2 benchmark(s)\n\n")
        .arg(result.date)
        .arg(result.benchmarks.size());
    if (!result.rawJson.isEmpty()) {
        rawContent += result.rawJson;
    } else {
        QJsonArray arr;
        for (const BenchmarkEntry& e : result.benchmarks) {
            QJsonObject obj;
            obj[QStringLiteral("name")]       = e.name;
            obj[QStringLiteral("real_time")]  = e.realTimeNs;
            obj[QStringLiteral("cpu_time")]   = e.cpuTimeNs;
            obj[QStringLiteral("iterations")] = static_cast<qint64>(e.iterations);
            obj[QStringLiteral("time_unit")]  = e.timeUnit;
            arr.append(obj);
        }
        QJsonObject meta;
        meta[QStringLiteral("compilerId")]        = result.compilerId;
        meta[QStringLiteral("standard")]          = result.standard;
        meta[QStringLiteral("optimizationLevel")] = result.optimizationLevel;
        QJsonObject root;
        root[QStringLiteral("date")]       = result.date;
        root[QStringLiteral("metadata")]   = meta;
        root[QStringLiteral("benchmarks")] = arr;
        rawContent += QString::fromUtf8(
            QJsonDocument(root).toJson(QJsonDocument::Indented));
    }
    m_rawJsonView->setPlainText(rawContent);
}

void BenchmarkWidget::onCompareClicked() {
    if (m_savedResults.size() >= 2) {
        m_comparisonChartWidget->compareResults(m_savedResults);
        m_resultsTabs->setTabEnabled(3, true);
        m_resultsTabs->setCurrentIndex(3); // Switch to Comparison tab
    }
}

// ── File Open / Save ──────────────────────────────────────────────────────────

void BenchmarkWidget::openBenchmarkFile() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Open Benchmark File"),
        QString(), QStringLiteral("C++ Files (*.cpp);;All Files (*)"));
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    const QString content = QTextStream(&f).readAll();
    addBenchTab(QFileInfo(path).fileName(), path, content);
    m_saveFileButton->setEnabled(false);
}

void BenchmarkWidget::saveBenchmarkFile() {
    auto* editor = currentBenchEditor();
    if (!editor) return;

    int idx = m_editorTabs->currentIndex();
    QString path = m_editorTabs->tabBar()->tabData(idx).toString();
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(
            this, QStringLiteral("Save Benchmark File"),
            QString(), QStringLiteral("C++ Files (*.cpp)"));
        if (path.isEmpty()) return;
        if (!path.endsWith(QStringLiteral(".cpp"), Qt::CaseInsensitive))
            path += QStringLiteral(".cpp");
        m_editorTabs->tabBar()->setTabData(idx, path);
        m_editorTabs->setTabText(idx, QFileInfo(path).fileName());
    }
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream(&f) << editor->text();
    // Remove the modification asterisk
    QString title = m_editorTabs->tabText(idx);
    if (title.endsWith(QLatin1Char('*')))
        m_editorTabs->setTabText(idx, title.chopped(1));
    m_saveFileButton->setEnabled(false);
}

// ── Import ────────────────────────────────────────────────────────────────────

void BenchmarkWidget::importResults() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Import Benchmark Results"),
        QString(), QStringLiteral("JSON (*.json)"));
    if (path.isEmpty()) return;
    BenchmarkResult result = m_runner->loadFromJson(path);
    if (!result.benchmarks.isEmpty()) {
        updateResultsView(result);
        if (m_savedResults.size() >= MAX_COMPARE)
            m_savedResults.removeFirst();
        m_savedResults << result;
        m_compareButton->setEnabled(m_savedResults.size() >= 2);
        m_exportButton->setEnabled(true);
        m_statusLabel->setText(
            QStringLiteral("Imported: %1").arg(QFileInfo(path).fileName()));
    }
}

// ── Theme ─────────────────────────────────────────────────────────────────────

void BenchmarkWidget::applyThemeToEditor(const QString& themeName) {
    Theme theme = ThemeManager::instance()->currentTheme();
    Q_UNUSED(themeName);

    for (int i = 0; i < m_editorTabs->count(); ++i) {
        auto* editor = qobject_cast<QsciScintilla*>(m_editorTabs->widget(i));
        if (!editor) continue;
        auto* lexer = qobject_cast<QsciLexerCPP*>(editor->lexer());
        if (lexer) {
            lexer->setDefaultPaper(theme.editorBackground);
            lexer->setDefaultColor(theme.editorForeground);
            for (int s = 0; s <= 128; ++s) {
                lexer->setPaper(theme.editorBackground, s);
                lexer->setColor(theme.editorForeground, s);
            }
            lexer->setColor(theme.syntaxKeyword,      QsciLexerCPP::Keyword);
            lexer->setColor(theme.syntaxType,         QsciLexerCPP::KeywordSet2);
            lexer->setColor(theme.syntaxString,       QsciLexerCPP::DoubleQuotedString);
            lexer->setColor(theme.syntaxString,       QsciLexerCPP::SingleQuotedString);
            lexer->setColor(theme.syntaxComment,      QsciLexerCPP::Comment);
            lexer->setColor(theme.syntaxComment,      QsciLexerCPP::CommentLine);
            lexer->setColor(theme.syntaxComment,      QsciLexerCPP::CommentDoc);
            lexer->setColor(theme.syntaxPreprocessor, QsciLexerCPP::PreProcessor);
            lexer->setColor(theme.syntaxNumber,       QsciLexerCPP::Number);
            lexer->setColor(theme.syntaxFunction,     QsciLexerCPP::Operator);
            QFont font(QStringLiteral("Monospace"), 10);
            lexer->setDefaultFont(font);
            for (int s = 0; s <= 128; ++s)
                lexer->setFont(font, s);
        }
        editor->setLexer(editor->lexer());
        editor->setPaper(theme.editorBackground);
        editor->setColor(theme.editorForeground);
        editor->setCaretLineVisible(true);
        editor->setCaretLineBackgroundColor(theme.editorCurrentLine);
        editor->setCaretWidth(2);
        editor->setCaretForegroundColor(theme.cursorColor);
        editor->setSelectionBackgroundColor(theme.accent);
        editor->setSelectionForegroundColor(theme.editorForeground);
        editor->setMarginsBackgroundColor(theme.sidebarBackground);
        editor->setMarginsForegroundColor(theme.textSecondary);
        // Fix: ensure fold margin doesn't leak white stripe on theme change
        editor->setFoldMarginColors(theme.sidebarBackground, theme.sidebarBackground);
        editor->recolor();
    }
}

void BenchmarkWidget::onThemeChanged(const QString& themeName) {
    applyThemeToEditor(themeName);
    m_chartWidget->onThemeChanged(themeName);
    m_comparisonChartWidget->onThemeChanged(themeName);
}

void BenchmarkWidget::stopProcess() {
    m_runner->cancel();
    m_tempBenchSource.reset(); // cleanup temp source file on stop
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("Stopped."));
}
