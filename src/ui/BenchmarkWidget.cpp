#include "ui/BenchmarkWidget.h"
#include "ui/BenchmarkChartWidget.h"
#include "ui/ThemeManager.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTemporaryFile>
#include <QTextStream>
#include <QToolButton>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────────────────────────
// Color palette for auto-assignment
// ─────────────────────────────────────────────────────────────────────────────

const QList<QColor>& BenchmarkWidget::recordPalette()
{
    static const QList<QColor> p = {
        QColor("#4E9AE8"), QColor("#4EC994"), QColor("#E87A4E"),
        QColor("#A97EE8"), QColor("#E8C94E"), QColor("#E84E7A"),
        QColor("#4EE8D0"), QColor("#E8A04E"),
    };
    return p;
}

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

BenchmarkWidget::BenchmarkWidget(QWidget* parent)
    : QWidget(parent)
    , m_runner(new BenchmarkRunner(this))
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
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

// ─────────────────────────────────────────────────────────────────────────────
// Public setters
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::setCompilerId(const QString& id) {
    m_compilerId = id;
    m_runner->setCompilerId(id);
}

void BenchmarkWidget::setStandard(const QString& standard) {
    m_standard = standard;
}

// ─────────────────────────────────────────────────────────────────────────────
// UI setup
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto* toolbar  = new QWidget(this);
    auto* tbLayout = new QHBoxLayout(toolbar);
    tbLayout->setContentsMargins(6, 4, 6, 4);
    setupToolbar(toolbar, tbLayout);
    mainLayout->addWidget(toolbar);

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
    tbLayout->addWidget(new QLabel(QStringLiteral("Opt:"), parent));
    m_optimizationCombo = new QComboBox(parent);
    m_optimizationCombo->addItems({ "O0", "O1", "O2", "O3" });
    m_optimizationCombo->setCurrentText(QStringLiteral("O2"));
    tbLayout->addWidget(m_optimizationCombo);

    tbLayout->addStretch();

    m_openFileButton = new QPushButton(QStringLiteral("Open..."), parent);
    m_openFileButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    m_openFileButton->setToolTip(QStringLiteral("Open benchmark file (Ctrl+Shift+O)"));
    connect(m_openFileButton, &QPushButton::clicked, this, &BenchmarkWidget::openBenchmarkFile);
    tbLayout->addWidget(m_openFileButton);

    m_saveFileButton = new QPushButton(QStringLiteral("Save"), parent);
    m_saveFileButton->setEnabled(false);
    m_saveFileButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    m_saveFileButton->setToolTip(QStringLiteral("Save benchmark file (Ctrl+Shift+S)"));
    connect(m_saveFileButton, &QPushButton::clicked, this, &BenchmarkWidget::saveBenchmarkFile);
    tbLayout->addWidget(m_saveFileButton);

    m_importButton = new QPushButton(QStringLiteral("Import..."), parent);
    m_importButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    m_importButton->setToolTip(QStringLiteral("Import results (Ctrl+Shift+I)"));
    connect(m_importButton, &QPushButton::clicked, this, &BenchmarkWidget::importResults);
    tbLayout->addWidget(m_importButton);

    m_runButton = new QPushButton(QStringLiteral("▶  Run"), parent);
    m_runButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F9));
    m_runButton->setToolTip(
        QStringLiteral("Run Benchmark (Ctrl+F9)\n\n"
                       "Educational tips:\n"
                       "  benchmark::DoNotOptimize(val)  — prevents the compiler from\n"
                       "    optimizing away the measured expression.\n"
                       "  benchmark::ClobberMemory()     — forces all pending writes\n"
                       "    to memory (use after writing to a buffer).\n\n"
                       "Reference: https://github.com/google/benchmark#preventing-optimisation"));
    connect(m_runButton, &QPushButton::clicked, this, &BenchmarkWidget::runBenchmark);
    tbLayout->addWidget(m_runButton);

    m_stopButton = new QPushButton(QStringLiteral("■ Stop"), parent);
    m_stopButton->setEnabled(false);
    m_stopButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F9));
    connect(m_stopButton, &QPushButton::clicked, this, &BenchmarkWidget::stopProcess);
    tbLayout->addWidget(m_stopButton);

    m_exportButton = new QPushButton(QStringLiteral("Export..."), parent);
    m_exportButton->setEnabled(false);
    m_exportButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_E));
    m_exportButton->setToolTip(QStringLiteral("Export results (Ctrl+Shift+E)"));
    connect(m_exportButton, &QPushButton::clicked, this, &BenchmarkWidget::exportResults);
    tbLayout->addWidget(m_exportButton);

    // "Compare" now just refreshes the Comparison tab with currently-checked records
    m_compareButton = new QPushButton(QStringLiteral("↺ Compare"), parent);
    m_compareButton->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_compareButton->setToolTip(
        QStringLiteral("Refresh Comparison chart (Ctrl+Shift+C)\n"
                       "Use the Results tab to select which runs to compare."));
    m_compareButton->setEnabled(false);
    connect(m_compareButton, &QPushButton::clicked, this, &BenchmarkWidget::onCompareClicked);
    tbLayout->addWidget(m_compareButton);

    m_statusLabel = new QLabel(QStringLiteral("Ready"), parent);
    m_statusLabel->setMinimumWidth(220);
    tbLayout->addWidget(m_statusLabel);
}

void BenchmarkWidget::setupCodeEditor() {
    m_editorTabs = new QTabWidget(this);
    m_editorTabs->setTabsClosable(true);
    m_editorTabs->setMovable(true);

    auto* addTabBtn = new QPushButton(QStringLiteral("+"), m_editorTabs);
    addTabBtn->setFixedSize(24, 24);
    addTabBtn->setToolTip(QStringLiteral("New benchmark file"));
    m_editorTabs->setCornerWidget(addTabBtn, Qt::TopRightCorner);
    connect(addTabBtn, &QPushButton::clicked, this, [this]() {
        addBenchTab(QStringLiteral("benchmark-%1.cpp").arg(m_newBenchCounter++),
                    QString(), QString());
        loadTemplateInCurrentTab();
    });

    connect(m_editorTabs, &QTabWidget::tabCloseRequested,
            this, &BenchmarkWidget::closeBenchTab);
    connect(m_editorTabs, &QTabWidget::currentChanged,
            this, [this](int) {
                m_saveFileButton->setEnabled(!currentBenchFilePath().isEmpty());
            });

    addBenchTab(QStringLiteral("benchmark-1.cpp"), QString(), QString());
    m_newBenchCounter = 2;
}

void BenchmarkWidget::setupResultsTabs() {
    m_resultsTabs = new QTabWidget(this);

    // Tab 0: Charts
    m_chartWidget = new BenchmarkChartWidget(m_resultsTabs);
    m_resultsTabs->addTab(m_chartWidget, QStringLiteral("Charts"));

    // Tab 1: Table
    m_tableWidget = new QTableWidget(0, 4, m_resultsTabs);
    m_tableWidget->setHorizontalHeaderLabels({
        QStringLiteral("Name"), QStringLiteral("Real Time"),
        QStringLiteral("CPU Time"), QStringLiteral("Iterations")
    });
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setStretchLastSection(false);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTabs->addTab(m_tableWidget, QStringLiteral("Table"));

    // Tab 2: Raw JSON
    m_rawJsonView = new QPlainTextEdit(m_resultsTabs);
    m_rawJsonView->setReadOnly(true);
    m_rawJsonView->setFont(QFont(QStringLiteral("Monospace"), 9));
    m_resultsTabs->addTab(m_rawJsonView, QStringLiteral("Raw JSON"));

    // Tab 3: Comparison
    m_comparisonChartWidget = new BenchmarkChartWidget(m_resultsTabs);
    m_resultsTabs->addTab(m_comparisonChartWidget, QStringLiteral("Comparison"));

    // Tab 4: Results Manager
    setupResultsManagerTab();
}

// ─────────────────────────────────────────────────────────────────────────────
// Results Manager tab
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::setupResultsManagerTab()
{
    auto* page   = new QWidget(m_resultsTabs);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(4);

    // ── Header ────────────────────────────────────────────────────────────────
    auto* hdrRow   = new QHBoxLayout();
    auto* hdrLabel = new QLabel(
        QString::fromUtf8(
            "<b>Results</b> \xe2\x80\x94 "
            "<i>Name</i> is shown in the Comparison legend \xc2\xb7 "
            "Double-click a name to rename \xc2\xb7 "
            "<b>View</b> selects what Charts/Table/JSON show \xc2\xb7 "
            "<b>Cmp</b> includes the run in the Comparison chart"),
        page);
    hdrLabel->setWordWrap(true);
    hdrRow->addWidget(hdrLabel, 1);

    m_clearAllBtn = new QPushButton(QStringLiteral("Clear All"), page);
    m_clearAllBtn->setToolTip(QStringLiteral("Remove all stored results"));
    connect(m_clearAllBtn, &QPushButton::clicked, this, &BenchmarkWidget::onClearAllResults);
    hdrRow->addWidget(m_clearAllBtn);
    layout->addLayout(hdrRow);

    // ── Scroll area ───────────────────────────────────────────────────────────
    m_recordsScrollArea = new QScrollArea(page);
    m_recordsScrollArea->setWidgetResizable(true);
    m_recordsScrollArea->setFrameShape(QFrame::StyledPanel);

    m_recordsContainer = new QWidget();
    m_recordsLayout    = new QVBoxLayout(m_recordsContainer);
    m_recordsLayout->setContentsMargins(2, 2, 2, 2);
    m_recordsLayout->setSpacing(2);
    m_recordsLayout->addStretch();

    m_recordsScrollArea->setWidget(m_recordsContainer);

    // Apply theme background immediately — re-applied in refreshResultsTable on theme change
    const Theme initTheme = ThemeManager::instance()->currentTheme();
    const QString bgCss = QString("background: %1;").arg(initTheme.panelBackground.name());
    m_recordsScrollArea->setStyleSheet(QString("QScrollArea { %1 border: none; }").arg(bgCss));
    if (m_recordsScrollArea->viewport())
        m_recordsScrollArea->viewport()->setStyleSheet(bgCss);
    m_recordsContainer->setStyleSheet(bgCss);

    layout->addWidget(m_recordsScrollArea, 1);

    m_resultsTabs->addTab(page, QStringLiteral("Results"));
}

// ─────────────────────────────────────────────────────────────────────────────
// Record management
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::addRecord(const BenchmarkResult& result,
                                const QString& userLabel,
                                const QColor&  color)
{
    BenchmarkResultRecord rec;
    rec.result = result;

    if (userLabel.isEmpty()) {
        const QString opt = result.optimizationLevel.isEmpty()
        ? m_optimizationCombo->currentText()
        : result.optimizationLevel;
        const QString std = result.standard.isEmpty() ? m_standard : result.standard;
        const QString cid = result.compilerId.isEmpty() ? m_compilerId : result.compilerId;
        const QString shortCid = cid.contains('-') ? cid.section('-', -1) : cid;
        rec.userLabel = QString("%1 / %2 / -%3").arg(shortCid.toUpper(), std, opt);
    } else {
        rec.userLabel = userLabel;
    }

    rec.color = color.isValid()
                    ? color
                    : recordPalette().value(m_records.size() % recordPalette().size());

    for (auto& r : m_records) r.showInView = false;
    rec.showInView   = true;
    rec.inComparison = false;

    m_records.append(rec);

    refreshResultsTable();
    refreshDisplayedResult();

    const int cmpCount = std::count_if(m_records.cbegin(), m_records.cend(),
                                       [](const BenchmarkResultRecord& r){ return r.inComparison; });
    m_compareButton->setEnabled(cmpCount >= 2 || m_records.size() >= 2);
}

void BenchmarkWidget::refreshResultsTable()
{
    // Remove all row widgets except the trailing stretch
    while (m_recordsLayout->count() > 1) {
        QLayoutItem* item = m_recordsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Pull theme colors for styling
    const Theme theme = ThemeManager::instance()->currentTheme();
    const QString rowBg0 = theme.panelBackground.name();
    const QString rowBg1 = theme.sidebarBackground.name();
    const QString fg     = theme.textPrimary.name();
    const QString fgDim  = theme.textSecondary.name();
    const QString border = theme.border.name();

    // Keep scroll area background in sync with theme
    const QString bgCss = QString("background: %1;").arg(rowBg0);
    m_recordsScrollArea->setStyleSheet(
        QString("QScrollArea { background: %1; border: none; }").arg(rowBg0));
    if (m_recordsScrollArea->viewport())
        m_recordsScrollArea->viewport()->setStyleSheet(bgCss);
    m_recordsContainer->setStyleSheet(bgCss);

    for (int idx = 0; idx < m_records.size(); ++idx) {
        const BenchmarkResultRecord& rec = m_records[idx];

        auto* row    = new QFrame(m_recordsContainer);
        row->setFrameShape(QFrame::StyledPanel);
        row->setStyleSheet(QString(
                               "QFrame { background: %1; border: 1px solid %2; border-radius: 4px; }"
                               "QLabel { color: %3; background: transparent; border: none; }"
                               "QCheckBox { color: %3; background: transparent; }"
                               "QCheckBox::indicator { width: 14px; height: 14px; }"
                               ).arg(idx % 2 == 0 ? rowBg0 : rowBg1, border, fg));

        auto* rowLay = new QHBoxLayout(row);
        rowLay->setContentsMargins(6, 4, 6, 4);
        rowLay->setSpacing(8);

        row->setProperty("recIdx", idx);

        // Color swatch — click to change
        auto* colorBtn = new QToolButton(row);
        colorBtn->setFixedSize(22, 22);
        colorBtn->setStyleSheet(QString(
                                    "QToolButton { background: %1; border: 1px solid %2; border-radius: 3px; }"
                                    "QToolButton:hover { border: 2px solid %3; }"
                                    ).arg(rec.color.name(), border, fg));
        colorBtn->setToolTip(QStringLiteral("Click to change color"));
        connect(colorBtn, &QToolButton::clicked, this, [this, row]() {
            const int i = row->property("recIdx").toInt();
            if (i >= 0 && i < m_records.size()) onResultColorClicked(i);
        });
        rowLay->addWidget(colorBtn);

        // Name label — click to rename
        auto* nameLbl = new QLabel(rec.userLabel, row);
        nameLbl->setMinimumWidth(80);
        nameLbl->setToolTip(QStringLiteral("Double-click to rename"));
        nameLbl->setCursor(Qt::PointingHandCursor);
        nameLbl->setStyleSheet(QString(
                                   "QLabel { color: %1; font-weight: bold; padding: 2px 4px;"
                                   "  border-radius: 3px; }"
                                   "QLabel:hover { background: %2; }"
                                   ).arg(fg, theme.accent.darker(150).name()));
        // Use mousePressEvent via event filter on the label
        connect(nameLbl, &QLabel::destroyed, [](){});  // keep moc happy
        nameLbl->installEventFilter(this);
        nameLbl->setProperty("recIdx", idx);
        rowLay->addWidget(nameLbl, 3);

        // Metadata
        const QString cid = rec.result.compilerId.isEmpty() ? m_compilerId : rec.result.compilerId;
        const QString std = rec.result.standard.isEmpty()   ? m_standard   : rec.result.standard;
        const QString opt = rec.result.optimizationLevel.isEmpty()
                                ? m_optimizationCombo->currentText()
                                : rec.result.optimizationLevel;

        auto makeMeta = [&](const QString& text, const QString& tip = QString()) {
            auto* lbl = new QLabel(text, row);
            lbl->setStyleSheet(QString("QLabel { color: %1; font-size: 11px; }").arg(fgDim));
            if (!tip.isEmpty()) lbl->setToolTip(tip);
            return lbl;
        };

        rowLay->addWidget(makeMeta(cid, cid));
        rowLay->addWidget(makeMeta(std));
        rowLay->addWidget(makeMeta(opt.isEmpty() ? QString("-") : "-" + opt));
        auto* cntLbl = makeMeta(QString::number(rec.result.benchmarks.size()));
        cntLbl->setToolTip(QStringLiteral("Number of benchmarks"));
        rowLay->addWidget(cntLbl);

        rowLay->addSpacing(4);

        // View checkbox
        auto* viewChk = new QCheckBox(QStringLiteral("View"), row);
        viewChk->setChecked(rec.showInView);
        viewChk->setToolTip(QStringLiteral("Show in Charts / Table / JSON"));
        connect(viewChk, &QCheckBox::toggled, this, [this, row](bool checked) {
            const int i = row->property("recIdx").toInt();
            if (i < 0 || i >= m_records.size()) return;
            for (int j = 0; j < m_records.size(); ++j)
                m_records[j].showInView = (j == i && checked);
            // Update View checkboxes in-place
            const int cnt = m_recordsLayout->count() - 1;
            for (int j = 0; j < cnt; ++j) {
                QLayoutItem* li = m_recordsLayout->itemAt(j);
                if (!li || !li->widget()) continue;
                const int recJ = li->widget()->property("recIdx").toInt();
                // View checkbox is at layout index 7 (after: colorBtn, name, cid, std, opt, cnt, spacing)
                QLayout* rl = li->widget()->layout();
                if (!rl) continue;
                for (int k = 0; k < rl->count(); ++k) {
                    if (auto* chk = qobject_cast<QCheckBox*>(rl->itemAt(k)->widget())) {
                        if (chk->text() == QStringLiteral("View")) {
                            chk->blockSignals(true);
                            chk->setChecked(recJ < m_records.size() && m_records[recJ].showInView);
                            chk->blockSignals(false);
                            break;
                        }
                    }
                }
            }
            refreshDisplayedResult();
        });
        rowLay->addWidget(viewChk);

        // Compare checkbox
        auto* cmpChk = new QCheckBox(QStringLiteral("Cmp"), row);
        cmpChk->setChecked(rec.inComparison);
        cmpChk->setToolTip(QStringLiteral("Include in Comparison chart"));
        connect(cmpChk, &QCheckBox::toggled, this, [this, row](bool checked) {
            const int i = row->property("recIdx").toInt();
            if (i >= 0 && i < m_records.size()) m_records[i].inComparison = checked;
            const int n = std::count_if(m_records.cbegin(), m_records.cend(),
                                        [](const BenchmarkResultRecord& r){ return r.inComparison; });
            m_compareButton->setEnabled(n >= 2);
        });
        rowLay->addWidget(cmpChk);

        rowLay->addSpacing(4);

        // Export button
        auto* expBtn = new QToolButton(row);
        expBtn->setText(QStringLiteral("Export"));
        expBtn->setFixedHeight(22);
        expBtn->setToolTip(QStringLiteral("Export to JSON / CSV"));
        expBtn->setStyleSheet(QString(
                                  "QToolButton { color: %1; border: 1px solid %2; border-radius: 3px; padding: 0 4px; }"
                                  "QToolButton:hover { background: %3; }"
                                  ).arg(fg, border, theme.accent.darker(150).name()));
        connect(expBtn, &QToolButton::clicked, this, [this, row]() {
            const int i = row->property("recIdx").toInt();
            if (i >= 0 && i < m_records.size()) onExportResultClicked(i);
        });
        rowLay->addWidget(expBtn);

        // Delete button
        auto* delBtn = new QToolButton(row);
        delBtn->setText(QStringLiteral("Del"));
        delBtn->setFixedHeight(22);
        delBtn->setToolTip(QStringLiteral("Remove this result"));
        delBtn->setStyleSheet(QString(
                                  "QToolButton { color: %1; border: 1px solid %2; border-radius: 3px; padding: 0 4px; }"
                                  "QToolButton:hover { background: %3; }"
                                  ).arg(theme.error.name(), border, theme.error.darker(150).name()));
        connect(delBtn, &QToolButton::clicked, this, [this, row]() {
            const int i = row->property("recIdx").toInt();
            if (i >= 0 && i < m_records.size()) onDeleteResultClicked(i);
        });
        rowLay->addWidget(delBtn);

        m_recordsLayout->insertWidget(m_recordsLayout->count() - 1, row);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Results Manager slots
// ─────────────────────────────────────────────────────────────────────────────

bool BenchmarkWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (auto* lbl = qobject_cast<QLabel*>(obj)) {
            const int i = lbl->property("recIdx").toInt();
            if (i >= 0 && i < m_records.size()) {
                bool ok = false;
                const QString newName = QInputDialog::getText(
                    this, QStringLiteral("Rename"),
                    QStringLiteral("New name:"),
                    QLineEdit::Normal,
                    m_records[i].userLabel, &ok);
                if (ok && !newName.trimmed().isEmpty()) {
                    m_records[i].userLabel = newName.trimmed();
                    refreshResultsTable();
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void BenchmarkWidget::onResultColorClicked(int row)
{
    if (row < 0 || row >= m_records.size()) return;
    const QColor chosen = QColorDialog::getColor(
        m_records[row].color, this,
        QStringLiteral("Choose color for \"%1\"").arg(m_records[row].userLabel));
    if (!chosen.isValid()) return;
    m_records[row].color = chosen;
    refreshResultsTable();
    // If this record is in comparison, refresh it immediately
    const int n = std::count_if(m_records.cbegin(), m_records.cend(),
                                [](const BenchmarkResultRecord& r){ return r.inComparison; });
    if (n >= 2) refreshComparison();
}

void BenchmarkWidget::onDeleteResultClicked(int row)
{
    if (row < 0 || row >= m_records.size()) return;
    const bool wasViewed = m_records[row].showInView;
    m_records.removeAt(row);

    // If the deleted record was the viewed one, auto-select the last record
    if (wasViewed && !m_records.isEmpty())
        m_records.last().showInView = true;

    refreshResultsTable();
    refreshDisplayedResult();

    const int n = std::count_if(m_records.cbegin(), m_records.cend(),
                                [](const BenchmarkResultRecord& r){ return r.inComparison; });
    m_compareButton->setEnabled(n >= 2);
    m_exportButton->setEnabled(!m_records.isEmpty());
}

void BenchmarkWidget::onClearAllResults()
{
    m_records.clear();
    refreshResultsTable();
    m_chartWidget->setResult(BenchmarkResult{});
    m_tableWidget->setRowCount(0);
    m_rawJsonView->clear();
    m_compareButton->setEnabled(false);
    m_exportButton->setEnabled(false);
}

void BenchmarkWidget::onExportResultClicked(int row)
{
    if (row < 0 || row >= m_records.size()) return;
    // Temporarily make the runner aware of this result by exposing it for export.
    // We use a direct file-write approach since m_runner->exportToJson/Csv uses its
    // own m_lastResult. Instead we write the JSON directly here.
    const BenchmarkResult& res = m_records[row].result;

    QString path = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("Export \"%1\"").arg(m_records[row].userLabel),
        QString(),
        QStringLiteral("JSON (*.json);;CSV (*.csv);;All Files (*)"));
    if (path.isEmpty()) return;

    if (!path.endsWith(QStringLiteral(".csv"),  Qt::CaseInsensitive) &&
        !path.endsWith(QStringLiteral(".json"), Qt::CaseInsensitive))
        path += QStringLiteral(".json");

    bool ok = false;
    if (path.endsWith(QStringLiteral(".csv"), Qt::CaseInsensitive)) {
        QFile f(path);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            out << "name,real_time,cpu_time,iterations,time_unit\n";
            for (const BenchmarkEntry& e : res.benchmarks)
                out << e.name << "," << e.realTimeNs << "," << e.cpuTimeNs
                    << "," << e.iterations << "," << e.timeUnit << "\n";
            ok = true;
        }
    } else {
        QJsonArray arr;
        for (const BenchmarkEntry& e : res.benchmarks) {
            QJsonObject obj;
            obj["name"]       = e.name;
            obj["real_time"]  = e.realTimeNs;
            obj["cpu_time"]   = e.cpuTimeNs;
            obj["iterations"] = static_cast<qint64>(e.iterations);
            obj["time_unit"]  = e.timeUnit;
            arr.append(obj);
        }
        QJsonObject meta;
        meta["compilerId"]        = res.compilerId;
        meta["standard"]          = res.standard;
        meta["optimizationLevel"] = res.optimizationLevel;
        meta["label"]             = m_records[row].userLabel;
        QJsonObject root;
        root["date"]       = res.date;
        root["metadata"]   = meta;
        root["benchmarks"] = arr;

        QFile f(path);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            ok = true;
        }
    }
    m_statusLabel->setText(ok ? QStringLiteral("Exported to %1").arg(path)
                              : QStringLiteral("Export failed."));
}

// ─────────────────────────────────────────────────────────────────────────────
// Display refresh helpers
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::refreshDisplayedResult()
{
    for (const auto& rec : m_records) {
        if (rec.showInView) {
            BenchmarkResult r = rec.result;
            r.displayColor = rec.color;   // so buildBarChart applies the user color
            updateResultsView(r);
            return;
        }
    }
    // Nothing selected — clear views
    m_chartWidget->setResult(BenchmarkResult{});
    m_tableWidget->setRowCount(0);
    m_rawJsonView->clear();
}

BenchmarkResult BenchmarkWidget::decoratedResult(int idx) const
{
    if (idx < 0 || idx >= m_records.size()) return {};
    BenchmarkResult r  = m_records[idx].result;
    r.label        = m_records[idx].userLabel;
    r.displayColor = m_records[idx].color;
    return r;
}

void BenchmarkWidget::refreshComparison()
{
    QList<BenchmarkResult> toCompare;
    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records[i].inComparison)
            toCompare << decoratedResult(i);
    }
    if (toCompare.size() < 2) return;

    m_comparisonChartWidget->compareResults(toCompare);
    m_resultsTabs->setCurrentIndex(3); // switch to Comparison tab
}

// ─────────────────────────────────────────────────────────────────────────────
// Toolbar slots
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::onCompareClicked()
{
    refreshComparison();
}

// ─────────────────────────────────────────────────────────────────────────────
// Run
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::runBenchmark() {
    auto* editor = currentBenchEditor();
    if (!editor) return;

    QString sourceToRun;
    const QString filePath = currentBenchFilePath();

    if (!filePath.isEmpty()) {
        QFile f(filePath);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text))
            QTextStream(&f) << editor->text();
        sourceToRun = filePath;
    } else {
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

    m_runner->setCompilerId(m_compilerId);

    QStringList flags;
    flags << (QStringLiteral("-std=") + m_standard);
    flags << (QStringLiteral("-") + m_optimizationCombo->currentText());

    m_runButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_statusLabel->setText(QStringLiteral("Compiling..."));
    m_runner->run(sourceToRun, flags);
}

void BenchmarkWidget::exportResults() {
    // Find the currently viewed record
    int viewedIdx = -1;
    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records[i].showInView) { viewedIdx = i; break; }
    }
    if (viewedIdx < 0) {
        m_statusLabel->setText(QStringLiteral("No result selected in View — use Results tab."));
        return;
    }
    onExportResultClicked(viewedIdx);
}

// ─────────────────────────────────────────────────────────────────────────────
// Runner result slots
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::onCompilationFinished(bool success, const QString& error) {
    if (!success) {
        m_tempBenchSource.reset();
        m_runButton->setEnabled(true);
        m_stopButton->setEnabled(false);
        m_statusLabel->setText(QStringLiteral("Compilation failed."));
        m_rawJsonView->setPlainText(QStringLiteral("// Compilation error:\n") + error);
        m_resultsTabs->setCurrentWidget(m_rawJsonView);
    } else {
        m_statusLabel->setText(QStringLiteral("Compiled — running..."));
    }
}

void BenchmarkWidget::onProgressMessage(const QString& msg) {
    m_statusLabel->setText(msg);
}

void BenchmarkWidget::onBenchmarkResultReady(const BenchmarkResult& result) {
    m_tempBenchSource.reset();
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_exportButton->setEnabled(true);

    // Store with full metadata
    BenchmarkResult stored = result;
    stored.compilerId        = m_compilerId;
    stored.standard          = m_standard;
    stored.optimizationLevel = m_optimizationCombo->currentText();

    addRecord(stored);

    m_statusLabel->setText(
        QStringLiteral("Done — %1 benchmark(s)  ·  %2 total stored")
            .arg(result.benchmarks.size()).arg(m_records.size()));

    // Switch to Results tab so user sees the new entry
    m_resultsTabs->setCurrentIndex(4);

    emit benchmarkCompleted(result);
}

// ─────────────────────────────────────────────────────────────────────────────
// View update (Charts / Table / Raw JSON)
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::updateResultsView(const BenchmarkResult& result) {
    m_chartWidget->setResult(result);

    m_tableWidget->setRowCount(result.benchmarks.size());
    for (int i = 0; i < result.benchmarks.size(); ++i) {
        const BenchmarkEntry& e    = result.benchmarks[i];
        const QString         unit = e.timeUnit.isEmpty() ? QStringLiteral("ns") : e.timeUnit;
        m_tableWidget->setItem(i, 0, new QTableWidgetItem(e.name));
        m_tableWidget->setItem(i, 1, new QTableWidgetItem(
                                         QStringLiteral("%1 %2").arg(e.realTimeNs, 0, 'f', 2).arg(unit)));
        m_tableWidget->setItem(i, 2, new QTableWidgetItem(
                                         QStringLiteral("%1 %2").arg(e.cpuTimeNs, 0, 'f', 2).arg(unit)));
        m_tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(e.iterations)));
    }

    QString raw = QStringLiteral("// %1 benchmark(s)  date: %2\n\n")
                      .arg(result.benchmarks.size()).arg(result.date);
    if (!result.rawJson.isEmpty()) {
        raw += result.rawJson;
    } else {
        QJsonArray arr;
        for (const BenchmarkEntry& e : result.benchmarks) {
            QJsonObject obj;
            obj["name"]       = e.name;
            obj["real_time"]  = e.realTimeNs;
            obj["cpu_time"]   = e.cpuTimeNs;
            obj["iterations"] = static_cast<qint64>(e.iterations);
            obj["time_unit"]  = e.timeUnit;
            arr.append(obj);
        }
        QJsonObject root;
        root["date"]       = result.date;
        root["benchmarks"] = arr;
        raw += QString::fromUtf8(QJsonDocument(root).toJson(QJsonDocument::Indented));
    }
    m_rawJsonView->setPlainText(raw);
}

// ─────────────────────────────────────────────────────────────────────────────
// Import
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::importResults() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Import Benchmark Results"),
        QString(), QStringLiteral("JSON (*.json)"));
    if (path.isEmpty()) return;

    BenchmarkResult result = m_runner->loadFromJson(path);
    if (result.benchmarks.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("Import failed or no benchmarks found."));
        return;
    }

    // Ask user for a label; default to filename
    const QString defaultLabel = QFileInfo(path).completeBaseName();
    const QString label = QInputDialog::getText(
        this, QStringLiteral("Name imported result"),
        QStringLiteral("Label for \"%1\":").arg(QFileInfo(path).fileName()),
        QLineEdit::Normal, defaultLabel);
    // Accept empty (user pressed OK without typing) or cancelled
    const QString finalLabel = label.trimmed().isEmpty() ? defaultLabel : label.trimmed();

    addRecord(result, finalLabel, QColor());

    m_exportButton->setEnabled(true);
    m_statusLabel->setText(
        QStringLiteral("Imported \"%1\"  \xc2\xb7  %2 total stored")
            .arg(finalLabel).arg(m_records.size()));

    m_resultsTabs->setCurrentIndex(4); // switch to Results tab
}

// ─────────────────────────────────────────────────────────────────────────────
// Stop
// ─────────────────────────────────────────────────────────────────────────────

void BenchmarkWidget::stopProcess() {
    m_runner->cancel();
    m_tempBenchSource.reset();
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("Stopped."));
}

// ─────────────────────────────────────────────────────────────────────────────
// Code editor tab helpers (unchanged logic)
// ─────────────────────────────────────────────────────────────────────────────

QsciScintilla* BenchmarkWidget::currentBenchEditor() const {
    return qobject_cast<QsciScintilla*>(m_editorTabs->currentWidget());
}

QString BenchmarkWidget::currentBenchFilePath() const {
    const int idx = m_editorTabs->currentIndex();
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

    if (!content.isEmpty()) editor->setText(content);

    const int idx = m_editorTabs->addTab(editor, title);
    m_editorTabs->setCurrentIndex(idx);
    m_editorTabs->tabBar()->setTabData(idx, filePath);

    connect(editor, &QsciScintilla::textChanged, this, [this, editor]() {
        const int i = m_editorTabs->indexOf(editor);
        if (i >= 0) {
            const QString t = m_editorTabs->tabText(i);
            if (!t.endsWith(QLatin1Char('*')))
                m_editorTabs->setTabText(i, t + QLatin1Char('*'));
        }
        if (m_saveFileButton) m_saveFileButton->setEnabled(true);
    });

    applyThemeToEditor(ThemeManager::instance()->currentThemeName());
}

bool BenchmarkWidget::closeBenchTab(int index) {
    m_editorTabs->removeTab(index);
    return true;
}

void BenchmarkWidget::loadTemplate() { loadTemplateInCurrentTab(); }

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

void BenchmarkWidget::openBenchmarkFile() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Open Benchmark File"),
        QString(), QStringLiteral("C++ Files (*.cpp);;All Files (*)"));
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    addBenchTab(QFileInfo(path).fileName(), path, QTextStream(&f).readAll());
    m_saveFileButton->setEnabled(false);
}

void BenchmarkWidget::saveBenchmarkFile() {
    auto* editor = currentBenchEditor();
    if (!editor) return;
    const int idx  = m_editorTabs->currentIndex();
    QString   path = m_editorTabs->tabBar()->tabData(idx).toString();
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
    QString title = m_editorTabs->tabText(idx);
    if (title.endsWith(QLatin1Char('*')))
        m_editorTabs->setTabText(idx, title.chopped(1));
    m_saveFileButton->setEnabled(false);
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────

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
            for (int s = 0; s <= 128; ++s) lexer->setFont(font, s);
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
        editor->setFoldMarginColors(theme.sidebarBackground, theme.sidebarBackground);
        editor->recolor();
    }
}

void BenchmarkWidget::onThemeChanged(const QString& themeName) {
    applyThemeToEditor(themeName);
    m_chartWidget->onThemeChanged(themeName);
    m_comparisonChartWidget->onThemeChanged(themeName);
    // Refresh color swatches in results table — they use inline stylesheet
    refreshResultsTable();
}

void BenchmarkWidget::applyEditorSettings(const QFont& font, bool showLineNumbers, bool wordWrap) {
    for (int i = 0; i < m_editorTabs->count(); ++i) {
        auto* editor = qobject_cast<QsciScintilla*>(m_editorTabs->widget(i));
        if (!editor) continue;
        editor->setFont(font);
        editor->setMarginsFont(font);
        auto* lexer = qobject_cast<QsciLexerCPP*>(editor->lexer());
        if (lexer) {
            lexer->setDefaultFont(font);
            for (int style = 0; style < 128; ++style)
                lexer->setFont(font, style);
        }
        const int lineNumMargin = 0;
        editor->setMarginType(lineNumMargin, QsciScintilla::NumberMargin);
        editor->setMarginLineNumbers(lineNumMargin, showLineNumbers);
        editor->setMarginWidth(lineNumMargin, showLineNumbers
                                                  ? QStringLiteral("00000")
                                                  : QStringLiteral("0"));
        editor->setWrapMode(wordWrap ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
    }
}
