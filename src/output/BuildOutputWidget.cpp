#include "output/BuildOutputWidget.h"
#include <QHBoxLayout>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QClipboard>
#include <QApplication>
#include <QRegularExpression>

BuildOutputWidget::BuildOutputWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void BuildOutputWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Toolbar
    QWidget* toolbar = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    
    m_clearButton = new QPushButton("Clear", toolbar);
    m_copyButton = new QPushButton("Copy", toolbar);
    
    toolbarLayout->addWidget(m_clearButton);
    toolbarLayout->addWidget(m_copyButton);
    toolbarLayout->addStretch();
    
    // Text edit
    m_textEdit = new QPlainTextEdit(this);
    m_textEdit->setReadOnly(true);
    m_textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    
    // Set monospace font
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(9);
    m_textEdit->setFont(font);
    
    // Set dark theme colors
    QPalette p = m_textEdit->palette();
    p.setColor(QPalette::Base, QColor("#1E1E1E"));
    p.setColor(QPalette::Text, QColor("#D4D4D4"));
    m_textEdit->setPalette(p);
    
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(m_textEdit);
    
    // Connect signals
    connect(m_clearButton, &QPushButton::clicked, this, &BuildOutputWidget::onClearClicked);
    connect(m_copyButton, &QPushButton::clicked, this, &BuildOutputWidget::onCopyClicked);
}

void BuildOutputWidget::appendText(const QString& text) {
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor("#D4D4D4"));
    
    cursor.setCharFormat(format);
    cursor.insertText(text + "\n");
    
    m_textEdit->setTextCursor(cursor);
    m_textEdit->ensureCursorVisible();
}

void BuildOutputWidget::appendError(const QString& text) {
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor("#F44747"));  // Red
    format.setFontWeight(QFont::Bold);
    
    cursor.setCharFormat(format);
    cursor.insertText(text + "\n");
    
    m_textEdit->setTextCursor(cursor);
    m_textEdit->ensureCursorVisible();
}

void BuildOutputWidget::appendWarning(const QString& text) {
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor("#CCA700"));  // Yellow
    
    cursor.setCharFormat(format);
    cursor.insertText(text + "\n");
    
    m_textEdit->setTextCursor(cursor);
    m_textEdit->ensureCursorVisible();
}

void BuildOutputWidget::clear() {
    m_textEdit->clear();
}

QString BuildOutputWidget::text() const {
    return m_textEdit->toPlainText();
}

void BuildOutputWidget::onClearClicked() {
    clear();
}

void BuildOutputWidget::onCopyClicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(m_textEdit->toPlainText());
}

void BuildOutputWidget::parseAndHighlightLine(const QString& line) {
    // Parse GCC/Clang diagnostic format: file:line:col: severity: message
    QRegularExpression re(R"(([^:]+):(\d+):(\d+):\s+(error|warning|note))");
    QRegularExpressionMatch match = re.match(line);
    
    if (match.hasMatch()) {
        QString severity = match.captured(4);
        if (severity == "error") {
            appendError(line);
        } else if (severity == "warning") {
            appendWarning(line);
        } else {
            appendText(line);
        }
    } else {
        appendText(line);
    }
}
