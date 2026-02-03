#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QMap>

class QSyntaxHighlighter;
class LineNumberArea;

/**
 * @brief Code editor widget with C++ syntax highlighting and features
 */
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
    
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() override = default;
    
    /**
     * @brief Load file content into editor
     * @param filePath Path to file
     * @return true if successful
     */
    bool loadFile(const QString& filePath);
    
    /**
     * @brief Save editor content to file
     * @param filePath Path to file
     * @return true if successful
     */
    bool saveFile(const QString& filePath);
    
    /**
     * @brief Check if editor content has been modified
     * @return true if modified
     */
    bool isModified() const;
    
    /**
     * @brief Set modified state
     * @param modified Modified state
     */
    void setModified(bool modified);
    
    /**
     * @brief Get current file path
     * @return File path
     */
    QString filePath() const;
    
    /**
     * @brief Set current file path
     * @param path File path
     */
    void setFilePath(const QString& path);
    
    /**
     * @brief Go to specific line
     * @param line Line number (1-based)
     */
    void gotoLine(int line);
    
    /**
     * @brief Set error marker at line
     * @param line Line number (1-based)
     * @param message Error message
     */
    void setErrorMarker(int line, const QString& message);
    
    /**
     * @brief Clear all error markers
     */
    void clearErrorMarkers();
    
    /**
     * @brief Paint line number area
     * @param event Paint event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    
    /**
     * @brief Get width of line number area
     * @return Width in pixels
     */
    int lineNumberAreaWidth();
    
signals:
    void modificationChanged(bool modified);
    
protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    
private:
    LineNumberArea *m_lineNumberArea;
    QSyntaxHighlighter *m_highlighter;
    QString m_filePath;
    bool m_isModified = false;
    QMap<int, QString> m_errorMarkers;  // line -> error message
    
    void setupEditor();
};

/**
 * @brief Widget for displaying line numbers
 */
class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), m_codeEditor(editor) {}
    
    QSize sizeHint() const override {
        return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
    }
    
protected:
    void paintEvent(QPaintEvent *event) override {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }
    
private:
    CodeEditor *m_codeEditor;
};

#endif // CODEEDITOR_H
