#include "ui/AssemblyWidget.h"
#include "ui/ThemeManager.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTextStream>
#include <QUuid>
#include <QVBoxLayout>

AssemblyWidget::AssemblyWidget(QWidget* parent)
    : QWidget(parent)
    , m_runner(new AssemblyRunner(this))
{
    setupUi();

    connect(m_runner, &AssemblyRunner::started,
            this, &AssemblyWidget::onRunnerStarted);
    connect(m_runner, &AssemblyRunner::finished,
            this, &AssemblyWidget::onRunnerFinished);
    connect(m_runner, &AssemblyRunner::progressMessage,
            this, &AssemblyWidget::onProgressMessage);
    connect(m_runner, &AssemblyRunner::lineMapReady,
            this, &AssemblyWidget::onLineMapReady);

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &AssemblyWidget::onThemeChanged);

    onThemeChanged(ThemeManager::instance()->currentThemeName());
}

// ── UI setup ──────────────────────────────────────────────────────────────────

void AssemblyWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- Toolbar ---
    auto* toolbar = new QWidget(this);
    auto* tbLayout = new QHBoxLayout(toolbar);
    tbLayout->setContentsMargins(6, 4, 6, 4);

    // Optimisation selector — assembly-specific, not duplicated in MainWindow
    tbLayout->addWidget(new QLabel(QStringLiteral("Opt:"), toolbar));
    m_optimizationCombo = new QComboBox(toolbar);
    m_optimizationCombo->addItems({ "O0", "O1", "O2", "O3", "Os" });
    m_optimizationCombo->setCurrentText(QStringLiteral("O0"));
    tbLayout->addWidget(m_optimizationCombo);

    tbLayout->addSpacing(8);

    // Syntax selector (AT&T / Intel)
    tbLayout->addWidget(new QLabel(QStringLiteral("Syntax:"), toolbar));
    m_syntaxCombo = new QComboBox(toolbar);
    m_syntaxCombo->addItems({ "AT&T", "Intel" });
    tbLayout->addWidget(m_syntaxCombo);

    tbLayout->addStretch();

    // Run button
    m_runButton = new QPushButton(QStringLiteral("▶  Generate Assembly"), toolbar);
    m_runButton->setToolTip(QStringLiteral("Compile current file to assembly"));
    connect(m_runButton, &QPushButton::clicked, this, &AssemblyWidget::runAssembly);
    tbLayout->addWidget(m_runButton);

    // Stop button
    m_stopButton = new QPushButton(QStringLiteral("■ Stop"), toolbar);
    m_stopButton->setEnabled(false);
    connect(m_stopButton, &QPushButton::clicked, this, &AssemblyWidget::stopProcess);
    tbLayout->addWidget(m_stopButton);

    // Status label
    m_statusLabel = new QLabel(QStringLiteral("Ready"), toolbar);
    m_statusLabel->setMinimumWidth(260);
    tbLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(toolbar);

    // --- Splitter with two editors ---
    auto* splitter = new QSplitter(Qt::Horizontal, this);

    m_sourceLexer  = new QsciLexerCPP(this);
    m_sourceEditor = new QsciScintilla(splitter);
    m_sourceEditor->setReadOnly(true);
    m_sourceEditor->setToolTip(QStringLiteral("Source code (read-only mirror)"));
    setupLexer(m_sourceEditor, m_sourceLexer);
    splitter->addWidget(m_sourceEditor);

    m_asmLexer  = new QsciLexerCPP(this); // re-use CPP lexer for basic highlighting
    m_asmEditor = new QsciScintilla(splitter);
    m_asmEditor->setReadOnly(true);
    m_asmEditor->setToolTip(QStringLiteral("Assembly output"));
    setupLexer(m_asmEditor, m_asmLexer);
    splitter->addWidget(m_asmEditor);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    mainLayout->addWidget(splitter, 1);

    // Define highlight markers (background colour set in applyThemeToEditor)
    m_srcHighlightMarker = m_sourceEditor->markerDefine(QsciScintilla::Background);
    m_asmHighlightMarker = m_asmEditor->markerDefine(QsciScintilla::Background);

    // Connect asm editor cursor changes for asm→source highlighting
    connect(m_asmEditor, &QsciScintilla::cursorPositionChanged,
            this, &AssemblyWidget::onAsmCursorPositionChanged);
}

void AssemblyWidget::setupLexer(QsciScintilla* editor, QsciLexerCPP* lexer) {
    QFont font(QStringLiteral("Monospace"), 10);
    lexer->setDefaultFont(font);
    editor->setLexer(lexer);
    editor->setTabWidth(4);
    editor->setIndentationsUseTabs(false);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setMarginType(0, QsciScintilla::NumberMargin);
    editor->setMarginWidth(0, QStringLiteral("00000"));
}

// ── Public interface ──────────────────────────────────────────────────────────

void AssemblyWidget::setCompilerId(const QString& id) {
    m_compilerId = id;
    if (m_runner) m_runner->setCompilerId(id);
}

void AssemblyWidget::setStandard(const QString& standard) {
    m_standard = standard;
}

void AssemblyWidget::setSourceCode(const QString& code, const QString& filePath) {
    m_currentSourceCode = code;
    m_currentFilePath   = filePath;
    m_sourceEditor->setText(code);
    m_asmEditor->clear();
    m_asmLineToSrcLine.clear();
    m_srcLineToFirstAsm.clear();
    m_statusLabel->setText(QStringLiteral("Ready — press Generate Assembly"));
}

void AssemblyWidget::highlightSourceLine(int sourceLine) {
    // Highlight in source mirror
    m_sourceEditor->markerDeleteAll(m_srcHighlightMarker);
    if (sourceLine > 0) {
        m_sourceEditor->markerAdd(sourceLine - 1, m_srcHighlightMarker);
        m_sourceEditor->ensureLineVisible(sourceLine - 1);
    }

    // Scroll asm pane to first asm line for this source line
    if (m_srcLineToFirstAsm.contains(sourceLine)) {
        int asmLine = m_srcLineToFirstAsm.value(sourceLine);
        m_asmEditor->ensureLineVisible(asmLine - 1);
    }
}

// ── Run ──────────────────────────────────────────────────────────────────────

void AssemblyWidget::runAssembly() {
    if (m_currentSourceCode.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("No source code loaded."));
        return;
    }

    if (!m_runner->isAvailable()) {
        m_statusLabel->setText(QStringLiteral("No compiler available — configure in toolbar."));
        return;
    }

    // Write current source to a temp file
    QString uuid = QUuid::createUuid().toString().remove('{').remove('}').remove('-');
    QString tmpPath = QDir::tempPath()
        + QStringLiteral("/cppatlas_asmrun_")
        + uuid
        + QStringLiteral(".cpp");

    QFile tmpFile(tmpPath);
    if (!tmpFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_statusLabel->setText(QStringLiteral("Failed to create temp file."));
        return;
    }
    {
        QTextStream out(&tmpFile);
        out << m_currentSourceCode;
    }
    tmpFile.close();

    // Build flags
    QStringList flags;
    flags << QStringLiteral("-std=") + m_standard;
    flags << QStringLiteral("-") + m_optimizationCombo->currentText();

    // Configure runner
    m_runner->setIntelSyntax(m_syntaxCombo->currentText() == QStringLiteral("Intel"));
    m_runner->setCompilerId(m_compilerId);

    m_runButton->setEnabled(false);
    m_asmEditor->clear();
    m_asmLineToSrcLine.clear();
    m_srcLineToFirstAsm.clear();

    m_runner->run(tmpPath, flags);
}

// ── Slots — runner ────────────────────────────────────────────────────────────

void AssemblyWidget::onRunnerStarted() {
    m_statusLabel->setText(QStringLiteral("Generating assembly…"));
    m_stopButton->setEnabled(true);
    m_runButton->setEnabled(false);
}

void AssemblyWidget::onProgressMessage(const QString& msg) {
    m_statusLabel->setText(msg);
}

void AssemblyWidget::onRunnerFinished(bool success, const QString& output,
                                      const QString& error) {
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);

    if (success) {
        m_asmEditor->setText(output);
        m_statusLabel->setText(QStringLiteral("Done."));
    } else {
        m_asmEditor->setText(
            QStringLiteral("; Assembly error:\n;\n")
            + error.split('\n').join(QStringLiteral("\n; ")));
        m_statusLabel->setText(QStringLiteral("Error: ") + error.section('\n', 0, 0));
    }
}

void AssemblyWidget::onLineMapReady(const QMap<int, int>& asmLineToSrcLine) {
    m_asmLineToSrcLine = asmLineToSrcLine;

    // Build reverse map: source line → first asm line
    m_srcLineToFirstAsm.clear();
    for (auto it = asmLineToSrcLine.cbegin(); it != asmLineToSrcLine.cend(); ++it) {
        int asmLine = it.key();
        int srcLine = it.value();
        if (!m_srcLineToFirstAsm.contains(srcLine)) {
            m_srcLineToFirstAsm.insert(srcLine, asmLine);
        }
    }
}

void AssemblyWidget::onAsmCursorPositionChanged(int line, int col) {
    Q_UNUSED(col);
    int asmLine = line + 1; // QsciScintilla lines are 0-based

    if (m_asmLineToSrcLine.contains(asmLine)) {
        int srcLine = m_asmLineToSrcLine.value(asmLine);

        // Highlight in source mirror
        m_sourceEditor->markerDeleteAll(m_srcHighlightMarker);
        m_sourceEditor->markerAdd(srcLine - 1, m_srcHighlightMarker);
        m_sourceEditor->ensureLineVisible(srcLine - 1);

        emit sourceLineActivated(srcLine);
    }
}

// ── Theme ─────────────────────────────────────────────────────────────────────

void AssemblyWidget::applyThemeToEditor(QsciScintilla* editor, const QString& themeName) {
    Theme theme;
    if (themeName == QStringLiteral("light"))         theme = ThemeManager::lightTheme();
    else if (themeName == QStringLiteral("dracula"))   theme = ThemeManager::draculaTheme();
    else if (themeName == QStringLiteral("monokai"))   theme = ThemeManager::monokaiTheme();
    else                                               theme = ThemeManager::darkTheme();

    auto* lexer = qobject_cast<QsciLexerCPP*>(editor->lexer());
    if (lexer) {
        lexer->setDefaultPaper(theme.editorBackground);
        lexer->setDefaultColor(theme.editorForeground);
        for (int s = 0; s <= 128; ++s) {
            lexer->setPaper(theme.editorBackground, s);
            lexer->setColor(theme.editorForeground,  s);
        }
        lexer->setColor(theme.syntaxKeyword,      QsciLexerCPP::Keyword);
        lexer->setColor(theme.syntaxType,         QsciLexerCPP::KeywordSet2);
        lexer->setColor(theme.syntaxString,       QsciLexerCPP::DoubleQuotedString);
        lexer->setColor(theme.syntaxString,       QsciLexerCPP::SingleQuotedString);
        lexer->setColor(theme.syntaxComment,      QsciLexerCPP::Comment);
        lexer->setColor(theme.syntaxComment,      QsciLexerCPP::CommentLine);
        lexer->setColor(theme.syntaxPreprocessor, QsciLexerCPP::PreProcessor);
        lexer->setColor(theme.syntaxNumber,       QsciLexerCPP::Number);
    }
    editor->setPaper(theme.editorBackground);
    editor->setColor(theme.editorForeground);
    editor->setCaretLineVisible(true);
    editor->setCaretLineBackgroundColor(theme.editorCurrentLine);
    editor->setCaretForegroundColor(theme.cursorColor);
    editor->setMarginsBackgroundColor(theme.sidebarBackground);
    editor->setMarginsForegroundColor(theme.textSecondary);
    // FIX: set fold margin colors to match sidebar — prevents white strips
    editor->setFoldMarginColors(theme.sidebarBackground, theme.sidebarBackground);

    // Highlight marker colour (a semi-transparent accent)
    QColor highlightColour = theme.accent;
    highlightColour.setAlpha(80);

    int srcMarker = (editor == m_sourceEditor) ? m_srcHighlightMarker : m_asmHighlightMarker;
    editor->setMarkerBackgroundColor(highlightColour, srcMarker);

    editor->recolor();
}

void AssemblyWidget::onThemeChanged(const QString& themeName) {
    applyThemeToEditor(m_sourceEditor, themeName);
    applyThemeToEditor(m_asmEditor,    themeName);
}

void AssemblyWidget::clearHighlights() {
    m_sourceEditor->markerDeleteAll(m_srcHighlightMarker);
    m_asmEditor->markerDeleteAll(m_asmHighlightMarker);
}

void AssemblyWidget::stopProcess() {
    m_runner->cancel();
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("Stopped."));
}
