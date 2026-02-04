#include "editor/CodeEditor.h"
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
    setCaretLineBackgroundColor(QColor("#2A2A2A"));
    
    // Selection colors
    setSelectionBackgroundColor(QColor("#264F78"));
    
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
    
    // Apply dark theme colors to lexer
    applyTheme("dark");
    
    setLexer(m_lexer);
}

void CodeEditor::setupMargins() {
    // Line numbers margin
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginWidth(0, "00000");
    setMarginsForegroundColor(QColor("#858585"));
    setMarginsBackgroundColor(QColor("#252526"));
    
    // Symbol margin for markers (errors, warnings, breakpoints)
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginWidth(1, 16);
    setMarginSensitivity(1, true);
    setMarginsBackgroundColor(QColor("#252526"));
    
    // Define marker symbols
    m_errorMarkerHandle = markerDefine(QsciScintilla::Circle);
    setMarkerBackgroundColor(QColor("#F14C4C"), m_errorMarkerHandle);
    setMarkerForegroundColor(QColor("#FFFFFF"), m_errorMarkerHandle);
    
    m_warningMarkerHandle = markerDefine(QsciScintilla::Circle);
    setMarkerBackgroundColor(QColor("#CCA700"), m_warningMarkerHandle);
    setMarkerForegroundColor(QColor("#FFFFFF"), m_warningMarkerHandle);
}

void CodeEditor::setupFolding() {
    // Enable code folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFoldMarginColors(QColor("#252526"), QColor("#252526"));
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
    if (themeName == "dark") {
        // Dark theme (VS Code Dark+)
        setPaper(QColor("#1E1E1E"));
        setColor(QColor("#D4D4D4"));
        setCaretLineBackgroundColor(QColor("#2A2A2A"));
        setSelectionBackgroundColor(QColor("#264F78"));
        setMarginsBackgroundColor(QColor("#252526"));
        setMarginsForegroundColor(QColor("#858585"));
        setFoldMarginColors(QColor("#252526"), QColor("#252526"));
        
        if (m_lexer) {
            m_lexer->setDefaultPaper(QColor("#1E1E1E"));
            m_lexer->setDefaultColor(QColor("#D4D4D4"));
            m_lexer->setColor(QColor("#569CD6"), QsciLexerCPP::Keyword);      // Blue
            m_lexer->setColor(QColor("#4EC9B0"), QsciLexerCPP::KeywordSet2);  // Cyan (types)
            m_lexer->setColor(QColor("#CE9178"), QsciLexerCPP::DoubleQuotedString); // Orange
            m_lexer->setColor(QColor("#CE9178"), QsciLexerCPP::SingleQuotedString);
            m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::Comment);      // Green
            m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::CommentLine);
            m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::CommentDoc);
            m_lexer->setColor(QColor("#C586C0"), QsciLexerCPP::PreProcessor); // Purple
            m_lexer->setColor(QColor("#B5CEA8"), QsciLexerCPP::Number);       // Light green
            m_lexer->setColor(QColor("#DCDCAA"), QsciLexerCPP::Operator);     // Yellow
        }
    } else if (themeName == "light") {
        // Light theme (VS Code Light+)
        setPaper(QColor("#FFFFFF"));
        setColor(QColor("#000000"));
        setCaretLineBackgroundColor(QColor("#F3F3F3"));
        setSelectionBackgroundColor(QColor("#ADD6FF"));
        setMarginsBackgroundColor(QColor("#F5F5F5"));
        setMarginsForegroundColor(QColor("#237893"));
        setFoldMarginColors(QColor("#F5F5F5"), QColor("#F5F5F5"));
        
        if (m_lexer) {
            m_lexer->setDefaultPaper(QColor("#FFFFFF"));
            m_lexer->setDefaultColor(QColor("#000000"));
            m_lexer->setColor(QColor("#0000FF"), QsciLexerCPP::Keyword);
            m_lexer->setColor(QColor("#267F99"), QsciLexerCPP::KeywordSet2);
            m_lexer->setColor(QColor("#A31515"), QsciLexerCPP::DoubleQuotedString);
            m_lexer->setColor(QColor("#A31515"), QsciLexerCPP::SingleQuotedString);
            m_lexer->setColor(QColor("#008000"), QsciLexerCPP::Comment);
            m_lexer->setColor(QColor("#008000"), QsciLexerCPP::CommentLine);
            m_lexer->setColor(QColor("#008000"), QsciLexerCPP::CommentDoc);
            m_lexer->setColor(QColor("#0000FF"), QsciLexerCPP::PreProcessor);
            m_lexer->setColor(QColor("#098658"), QsciLexerCPP::Number);
            m_lexer->setColor(QColor("#000000"), QsciLexerCPP::Operator);
        }
    }
}

void CodeEditor::onTextChanged() {
    if (!m_isModified) {
        m_isModified = true;
        emit modificationChanged(true);
    }
}
