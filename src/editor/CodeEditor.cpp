#include "editor/CodeEditor.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QPainter>
#include <QTextBlock>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QScrollBar>

/**
 * @brief C++ syntax highlighter
 */
class CppSyntaxHighlighter : public QSyntaxHighlighter {
public:
    CppSyntaxHighlighter(QTextDocument *parent = nullptr)
        : QSyntaxHighlighter(parent)
    {
        // Keywords
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(QColor("#569CD6")); // Blue
        keywordFormat.setFontWeight(QFont::Bold);
        
        QStringList keywordPatterns = {
            "\\bclass\\b", "\\bstruct\\b", "\\benum\\b", "\\bunion\\b",
            "\\bif\\b", "\\belse\\b", "\\bfor\\b", "\\bwhile\\b", "\\bdo\\b",
            "\\bswitch\\b", "\\bcase\\b", "\\bdefault\\b", "\\bbreak\\b",
            "\\bcontinue\\b", "\\breturn\\b", "\\bgoto\\b",
            "\\btry\\b", "\\bcatch\\b", "\\bthrow\\b",
            "\\bpublic\\b", "\\bprivate\\b", "\\bprotected\\b",
            "\\bvirtual\\b", "\\boverride\\b", "\\bfinal\\b",
            "\\bconst\\b", "\\bconstexpr\\b", "\\bstatic\\b",
            "\\bextern\\b", "\\binline\\b", "\\bexplicit\\b",
            "\\bnamespace\\b", "\\busing\\b", "\\btypedef\\b",
            "\\btemplate\\b", "\\btypename\\b",
            "\\bnew\\b", "\\bdelete\\b", "\\bsizeof\\b",
            "\\bauto\\b", "\\bdecltype\\b", "\\bnullptr\\b",
            "\\bvolatile\\b", "\\bmutable\\b", "\\boperator\\b"
        };
        
        for (const QString &pattern : keywordPatterns) {
            HighlightingRule rule;
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            m_highlightingRules.append(rule);
        }
        
        // Types
        QTextCharFormat typeFormat;
        typeFormat.setForeground(QColor("#4EC9B0")); // Cyan
        
        QStringList typePatterns = {
            "\\bvoid\\b", "\\bbool\\b", "\\bchar\\b", "\\bint\\b",
            "\\bshort\\b", "\\blong\\b", "\\bfloat\\b", "\\bdouble\\b",
            "\\bunsigned\\b", "\\bsigned\\b",
            "\\bint8_t\\b", "\\bint16_t\\b", "\\bint32_t\\b", "\\bint64_t\\b",
            "\\buint8_t\\b", "\\buint16_t\\b", "\\buint32_t\\b", "\\buint64_t\\b",
            "\\bsize_t\\b", "\\bptrdiff_t\\b", "\\bstring\\b"
        };
        
        for (const QString &pattern : typePatterns) {
            HighlightingRule rule;
            rule.pattern = QRegularExpression(pattern);
            rule.format = typeFormat;
            m_highlightingRules.append(rule);
        }
        
        // Preprocessor directives
        QTextCharFormat preprocessorFormat;
        preprocessorFormat.setForeground(QColor("#C586C0")); // Purple
        
        HighlightingRule preprocessorRule;
        preprocessorRule.pattern = QRegularExpression("^\\s*#.*");
        preprocessorRule.format = preprocessorFormat;
        m_highlightingRules.append(preprocessorRule);
        
        // String literals
        QTextCharFormat stringFormat;
        stringFormat.setForeground(QColor("#CE9178")); // Orange
        
        HighlightingRule stringRule;
        stringRule.pattern = QRegularExpression("\".*?\"");
        stringRule.format = stringFormat;
        m_highlightingRules.append(stringRule);
        
        // Character literals
        stringRule.pattern = QRegularExpression("'.'");
        m_highlightingRules.append(stringRule);
        
        // Single-line comments
        QTextCharFormat commentFormat;
        commentFormat.setForeground(QColor("#6A9955")); // Green
        
        HighlightingRule commentRule;
        commentRule.pattern = QRegularExpression("//.*");
        commentRule.format = commentFormat;
        m_highlightingRules.append(commentRule);
        
        // Multi-line comments
        m_commentStartExpression = QRegularExpression("/\\*");
        m_commentEndExpression = QRegularExpression("\\*/");
        m_multiLineCommentFormat = commentFormat;
    }
    
protected:
    void highlightBlock(const QString &text) override {
        // Apply single-line rules
        for (const HighlightingRule &rule : m_highlightingRules) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
        
        // Handle multi-line comments
        setCurrentBlockState(0);
        
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(m_commentStartExpression);
        
        while (startIndex >= 0) {
            QRegularExpressionMatch match = m_commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex, commentLength, m_multiLineCommentFormat);
            startIndex = text.indexOf(m_commentStartExpression, startIndex + commentLength);
        }
    }
    
private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    
    QVector<HighlightingRule> m_highlightingRules;
    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
    QTextCharFormat m_multiLineCommentFormat;
};

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);
    m_highlighter = new CppSyntaxHighlighter(document());
    
    setupEditor();
    
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);
    connect(this, &CodeEditor::textChanged, this, [this]() {
        if (!m_isModified) {
            m_isModified = true;
            emit modificationChanged(true);
        }
    });
    
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::setupEditor() {
    // Set font
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(10);
    setFont(font);
    
    // Set tab width to 4 spaces
    QFontMetrics metrics(font);
    setTabStopDistance(metrics.horizontalAdvance(' ') * 4);
    
    // Enable line wrap
    setLineWrapMode(QPlainTextEdit::NoWrap);
    
    // Set background color
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor("#1E1E1E"));
    p.setColor(QPalette::Text, QColor("#D4D4D4"));
    setPalette(p);
}

bool CodeEditor::loadFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream in(&file);
    setPlainText(in.readAll());
    file.close();
    
    m_filePath = filePath;
    m_isModified = false;
    emit modificationChanged(false);
    
    return true;
}

bool CodeEditor::saveFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out << toPlainText();
    file.close();
    
    m_filePath = filePath;
    m_isModified = false;
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
}

QString CodeEditor::filePath() const {
    return m_filePath;
}

void CodeEditor::setFilePath(const QString& path) {
    m_filePath = path;
}

void CodeEditor::gotoLine(int line) {
    QTextCursor cursor(document()->findBlockByLineNumber(line - 1));
    setTextCursor(cursor);
    ensureCursorVisible();
    centerCursor();
}

void CodeEditor::setErrorMarker(int line, const QString& message) {
    m_errorMarkers[line] = message;
    viewport()->update();
}

void CodeEditor::clearErrorMarkers() {
    m_errorMarkers.clear();
    viewport()->update();
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    
    int space = 13 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        
        QColor lineColor = QColor("#2A2A2A");
        
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor("#252526"));
    
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#858585"));
            
            // Check for error marker
            if (m_errorMarkers.contains(blockNumber + 1)) {
                painter.fillRect(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                               QColor("#5A1D1D"));
            }
            
            painter.drawText(0, top, m_lineNumberArea->width() - 5, fontMetrics().height(),
                           Qt::AlignRight, number);
        }
        
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *event) {
    // Auto-indentation
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QTextCursor cursor = textCursor();
        QString currentLine = cursor.block().text();
        
        // Calculate indentation
        int indent = 0;
        for (QChar ch : currentLine) {
            if (ch == ' ') indent++;
            else if (ch == '\t') indent += 4;
            else break;
        }
        
        // Check if line ends with opening brace
        if (currentLine.trimmed().endsWith('{')) {
            QPlainTextEdit::keyPressEvent(event);
            insertPlainText(QString(indent + 4, ' '));
            return;
        }
        
        QPlainTextEdit::keyPressEvent(event);
        if (indent > 0) {
            insertPlainText(QString(indent, ' '));
        }
        return;
    }
    
    // Handle closing brace with automatic dedent
    if (event->key() == Qt::Key_BraceRight) {
        QTextCursor cursor = textCursor();
        QString currentLine = cursor.block().text().left(cursor.positionInBlock());
        
        // If line only contains whitespace, dedent by one level
        if (currentLine.trimmed().isEmpty() && currentLine.length() >= 4) {
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);
            cursor.removeSelectedText();
        }
    }
    
    QPlainTextEdit::keyPressEvent(event);
}
