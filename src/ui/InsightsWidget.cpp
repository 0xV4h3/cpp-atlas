#include "ui/InsightsWidget.h"
#include "ui/ThemeManager.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTextStream>
#include <QVBoxLayout>
#include <QDir>
#include <QUuid>

InsightsWidget::InsightsWidget(QWidget* parent)
    : QWidget(parent)
    , m_runner(new CppInsightsRunner(this))
{
    setupUi();

    connect(m_runner, &CppInsightsRunner::started,
            this, &InsightsWidget::onInsightsStarted);
    connect(m_runner, &CppInsightsRunner::finished,
            this, &InsightsWidget::onInsightsFinished);
    connect(m_runner, &CppInsightsRunner::progressMessage,
            this, &InsightsWidget::onProgressMessage);
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &InsightsWidget::onThemeChanged);

    onThemeChanged(ThemeManager::instance()->currentThemeName());
}

void InsightsWidget::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- Toolbar ---
    auto* toolbar = new QWidget(this);
    auto* tbLayout = new QHBoxLayout(toolbar);
    tbLayout->setContentsMargins(6, 4, 6, 4);

    tbLayout->addStretch();

    m_runButton = new QPushButton(QStringLiteral("▶  Run Insights"), toolbar);
    m_runButton->setToolTip(QStringLiteral("Run C++ Insights on current file"));
    connect(m_runButton, &QPushButton::clicked, this, &InsightsWidget::runInsights);
    tbLayout->addWidget(m_runButton);

    m_stopButton = new QPushButton(QStringLiteral("■ Stop Insights"), toolbar);
    m_stopButton->setEnabled(false);
    connect(m_stopButton, &QPushButton::clicked, this, &InsightsWidget::stopProcess);
    tbLayout->addWidget(m_stopButton);

    m_statusLabel = new QLabel(QStringLiteral("Ready"), toolbar);
    m_statusLabel->setMinimumWidth(200);
    tbLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(toolbar);

    // --- Splitter with two editors ---
    auto* splitter = new QSplitter(Qt::Horizontal, this);

    m_sourceLexer = new QsciLexerCPP(this);
    m_sourceEditor = new QsciScintilla(splitter);
    m_sourceEditor->setReadOnly(true);
    m_sourceEditor->setToolTip(QStringLiteral("Source code (read-only mirror)"));
    setupLexer(m_sourceEditor, m_sourceLexer);
    splitter->addWidget(m_sourceEditor);

    m_outputLexer = new QsciLexerCPP(this);
    m_outputEditor = new QsciScintilla(splitter);
    m_outputEditor->setReadOnly(true);
    m_outputEditor->setToolTip(QStringLiteral("C++ Insights transformed output"));
    setupLexer(m_outputEditor, m_outputLexer);
    splitter->addWidget(m_outputEditor);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter, 1);
}

void InsightsWidget::setupLexer(QsciScintilla* editor, QsciLexerCPP* lexer) {
    QFont font(QStringLiteral("Monospace"), 10);
    lexer->setDefaultFont(font);
    editor->setLexer(lexer);
    editor->setTabWidth(4);
    editor->setIndentationsUseTabs(false);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setMarginType(0, QsciScintilla::NumberMargin);
    editor->setMarginWidth(0, QStringLiteral("00000"));
}

void InsightsWidget::setSourceCode(const QString& code, const QString& filePath) {
    m_currentSourceCode = code;
    m_currentFilePath   = filePath;
    m_sourceEditor->setText(code);
    m_outputEditor->clear();
    m_statusLabel->setText(QStringLiteral("Ready — press Run Insights"));
}

void InsightsWidget::setStandard(const QString& standard) {
    m_standard = standard;
}

void InsightsWidget::runInsights() {
    if (m_currentSourceCode.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("No source code loaded."));
        return;
    }

    if (!m_runner->isAvailable()) {
        m_statusLabel->setText(QStringLiteral("insights binary not found — configure in Tools > Settings"));
        return;
    }

    // Write current source to a temp file (insights needs a real file path).
    // Use a unique filename to avoid race conditions between concurrent instances.
    QString uuid = QUuid::createUuid().toString().remove('{').remove('}').remove('-');
    QString tmpPath = QDir::tempPath()
        + QStringLiteral("/cppatlas_insights_")
        + uuid
        + QStringLiteral(".cpp");
    QFile tmpFile(tmpPath);
    if (!tmpFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_statusLabel->setText(QStringLiteral("Failed to create temp file."));
        return;
    }
    QTextStream out(&tmpFile);
    out << m_currentSourceCode;
    tmpFile.close();

    QStringList flags;
    flags << QStringLiteral("-std=") + m_standard;

    m_runButton->setEnabled(false);
    m_outputEditor->clear();
    m_runner->run(tmpPath, flags);
}

void InsightsWidget::onInsightsStarted() {
    m_statusLabel->setText(QStringLiteral("Running C++ Insights..."));
    m_stopButton->setEnabled(true);
    m_runButton->setEnabled(false);
}

void InsightsWidget::onProgressMessage(const QString& msg) {
    m_statusLabel->setText(msg);
}

void InsightsWidget::onInsightsFinished(bool success, const QString& output,
                                         const QString& error) {
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);

    if (success) {
        m_outputEditor->setText(output);
        m_statusLabel->setText(QStringLiteral("Done."));
    } else {
        m_outputEditor->setText(
            QStringLiteral("// C++ Insights error:\n//\n")
            + error.split('\n').join(QStringLiteral("\n// ")));
        m_statusLabel->setText(QStringLiteral("Error: ") + error.section('\n', 0, 0));
    }
}

void InsightsWidget::applyThemeToEditor(QsciScintilla* editor, const QString& themeName) {
    // Reuse the same theme logic as CodeEditor::applyTheme()
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
    editor->recolor();
}

void InsightsWidget::onThemeChanged(const QString& themeName) {
    applyThemeToEditor(m_sourceEditor, themeName);
    applyThemeToEditor(m_outputEditor, themeName);
}

void InsightsWidget::stopProcess() {
    m_runner->cancel();
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("Stopped."));
}
