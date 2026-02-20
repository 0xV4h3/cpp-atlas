#include "editor/CodeEditor.h"
#include "ui/ThemeManager.h"
#include <QFile>
#include <QTextStream>
#include <QFont>
#include <QFontDatabase>

CodeEditor::CodeEditor(QWidget *parent)
    : QsciScintilla(parent)
{
    setupEditor();
    setupLexer();
    setupMargins();
    setupFolding();
    setupAutoCompletion();
    setupBraceMatching();
    
    connect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &CodeEditor::onThemeChanged);
    applyTheme(ThemeManager::instance()->currentThemeName());
}

void CodeEditor::onThemeChanged(const QString& themeName) {
    applyTheme(themeName);
}

void CodeEditor::setupEditor() {
    // Set font
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(10);
    setFont(font);
    
    // Tab settings
    setTabWidth(4);
    setIndentationsUseTabs(false);
    setTabIndents(true);
    setAutoIndent(true);
    setBackspaceUnindents(true);
    
    // Line wrapping
    setWrapMode(QsciScintilla::WrapNone);
    
    // Current line highlighting
    setCaretLineVisible(true);
    
    // Cursor settings (will be updated by theme)
    setCaretWidth(2);

    // Whitespace visibility
    setWhitespaceVisibility(QsciScintilla::WsInvisible);
    
    // EOL mode
    setEolMode(QsciScintilla::EolUnix);
    setEolVisibility(false);
}

void CodeEditor::setupLexer() {
    m_lexer = new QsciLexerCPP(this);
    
    // Set lexer font
    QFont font("Monospace", 10);
    m_lexer->setDefaultFont(font);
}

void CodeEditor::setupMargins() {
    // Line numbers margin
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginWidth(0, "00000");
    
    // Symbol margin for markers (errors, warnings, breakpoints)
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginWidth(1, 16);
    setMarginSensitivity(1, true);
    
    // Define marker symbols
    m_errorMarkerHandle = markerDefine(QsciScintilla::Circle);
    m_warningMarkerHandle = markerDefine(QsciScintilla::Circle);
}

void CodeEditor::setupFolding() {
    // Enable code folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
}

void CodeEditor::setupAutoCompletion() {
    // Setup API for auto-completion
    m_apis = new QsciAPIs(m_lexer);
    
    // C++ keywords
    QStringList keywords = {
        "class", "struct", "template", "namespace", "public", "private", 
        "protected", "virtual", "override", "const", "constexpr", "static", 
        "auto", "nullptr", "void", "bool", "char", "int", "float", "double",
        "if", "else", "for", "while", "do", "switch", "case", "break", 
        "continue", "return", "try", "catch", "throw", "new", "delete",
        "enum", "union", "typedef", "using", "typename", "inline", "extern",
        "explicit", "operator", "friend", "mutable", "volatile", "register",
        "signed", "unsigned", "short", "long", "sizeof", "decltype", "final"
    };
    
    // C++ types
    QStringList types = {
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "size_t", "ptrdiff_t", "string", "wstring"
    };
    
    // STL containers and utilities
    QStringList stl = {
        "std::vector", "std::string", "std::map", "std::set", "std::list",
        "std::unique_ptr", "std::shared_ptr", "std::weak_ptr", "std::make_unique",
        "std::make_shared", "std::cout", "std::cin", "std::cerr", "std::endl",
        "std::array", "std::deque", "std::queue", "std::stack", "std::unordered_map",
        "std::unordered_set", "std::pair", "std::tuple", "std::optional",
        "std::function", "std::move", "std::forward", "std::begin", "std::end"
    };
    
    // Add all to API
    for (const QString& keyword : keywords) {
        m_apis->add(keyword);
    }
    for (const QString& type : types) {
        m_apis->add(type);
    }
    for (const QString& item : stl) {
        m_apis->add(item);
    }
    
    m_apis->prepare();
    
    // Auto-completion settings
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionThreshold(2);
    setAutoCompletionCaseSensitivity(true);
    setAutoCompletionReplaceWord(false);
}

void CodeEditor::setupBraceMatching() {
    // Enable strict brace matching
    setBraceMatching(QsciScintilla::StrictBraceMatch);
    setMatchedBraceBackgroundColor(QColor("#007ACC"));
    setMatchedBraceForegroundColor(QColor("#FFFFFF"));
    setUnmatchedBraceBackgroundColor(QColor("#F14C4C"));
    setUnmatchedBraceForegroundColor(QColor("#FFFFFF"));
}

bool CodeEditor::loadFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream in(&file);
    setText(in.readAll());
    file.close();
    
    m_filePath = filePath;
    m_isModified = false;
    setModified(false);
    emit modificationChanged(false);
    
    return true;
}

bool CodeEditor::saveFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out << text();
    file.close();
    
    m_filePath = filePath;
    m_isModified = false;
    setModified(false);
    emit modificationChanged(false);
    
    return true;
}

bool CodeEditor::isModified() const {
    return m_isModified;
}

void CodeEditor::setModified(bool modified) {
    if (m_isModified != modified) {
        m_isModified = modified;
        emit modificationChanged(modified);
    }
    QsciScintilla::setModified(modified);
}

QString CodeEditor::filePath() const {
    return m_filePath;
}

void CodeEditor::setFilePath(const QString& path) {
    m_filePath = path;
}

void CodeEditor::gotoLine(int line) {
    // QScintilla uses 0-based line numbers internally
    setCursorPosition(line - 1, 0);
    ensureLineVisible(line - 1);
}

void CodeEditor::setErrorMarker(int line, const QString& message) {
    // QScintilla uses 0-based line numbers
    markerAdd(line - 1, m_errorMarkerHandle);
    m_errorMarkers[line] = message;
}

void CodeEditor::setWarningMarker(int line, const QString& message) {
    // QScintilla uses 0-based line numbers
    markerAdd(line - 1, m_warningMarkerHandle);
}

void CodeEditor::clearErrorMarkers() {
    markerDeleteAll(m_errorMarkerHandle);
    m_errorMarkers.clear();
}

void CodeEditor::clearAllMarkers() {
    markerDeleteAll(-1);  // -1 means all markers
    m_errorMarkers.clear();
}

void CodeEditor::applyTheme(const QString& themeName) {
    Theme theme;
    if (themeName == "dark") {
        theme = ThemeManager::darkTheme();
    } else if (themeName == "light") {
        theme = ThemeManager::lightTheme();
    } else if (themeName == "dracula") {
        theme = ThemeManager::draculaTheme();
    } else if (themeName == "monokai") {
        theme = ThemeManager::monokaiTheme();
    } else {
        theme = ThemeManager::darkTheme();
    }

    // Lexer colors
    if (m_lexer) {
        m_lexer->setDefaultPaper(theme.editorBackground);
        m_lexer->setDefaultColor(theme.editorForeground);

        for (int style = 0; style <= 128; ++style) {
            m_lexer->setPaper(theme.editorBackground, style);
            m_lexer->setColor(theme.editorForeground, style);
        }

        m_lexer->setColor(theme.syntaxKeyword,      QsciLexerCPP::Keyword);
        m_lexer->setColor(theme.syntaxType,         QsciLexerCPP::KeywordSet2);
        m_lexer->setColor(theme.syntaxString,       QsciLexerCPP::DoubleQuotedString);
        m_lexer->setColor(theme.syntaxString,       QsciLexerCPP::SingleQuotedString);
        m_lexer->setColor(theme.syntaxComment,      QsciLexerCPP::Comment);
        m_lexer->setColor(theme.syntaxComment,      QsciLexerCPP::CommentLine);
        m_lexer->setColor(theme.syntaxComment,      QsciLexerCPP::CommentDoc);
        m_lexer->setColor(theme.syntaxPreprocessor, QsciLexerCPP::PreProcessor);
        m_lexer->setColor(theme.syntaxNumber,       QsciLexerCPP::Number);
        m_lexer->setColor(theme.syntaxFunction,     QsciLexerCPP::Operator);

        QFont font("Monospace", 10);
        m_lexer->setDefaultFont(font);
        for (int style = 0; style <= 128; ++style) {
            m_lexer->setFont(font, style);
        }
    }

    setLexer(m_lexer);

    // Main editor area
    setPaper(theme.editorBackground);
    setColor(theme.editorForeground);

    setCaretLineVisible(true);
    setCaretLineBackgroundColor(theme.editorCurrentLine);
    setCaretWidth(2);
    setCaretForegroundColor(theme.cursorColor);

    setSelectionBackgroundColor(theme.accent);
    setSelectionForegroundColor(theme.editorForeground);

    // Margins (line numbers)
    setMarginsBackgroundColor(theme.sidebarBackground);
    setMarginsForegroundColor(theme.textSecondary);

    QFont marginFont("Monospace", 10);
    setMarginsFont(marginFont);

    // Fold margin
    setFoldMarginColors(theme.sidebarBackground, theme.sidebarBackground);

    // Error/warning marker colors
    setMarkerBackgroundColor(theme.error,   m_errorMarkerHandle);
    setMarkerForegroundColor(Qt::white,     m_errorMarkerHandle);
    setMarkerBackgroundColor(theme.warning, m_warningMarkerHandle);
    setMarkerForegroundColor(Qt::white,     m_warningMarkerHandle);

    recolor();
}

void CodeEditor::onTextChanged() {
    if (!m_isModified) {
        m_isModified = true;
        emit modificationChanged(true);
    }
}
