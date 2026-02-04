#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>
#include <QMap>

/**
 * @brief Code editor widget with C++ syntax highlighting and QScintilla features
 */
class CodeEditor : public QsciScintilla {
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
     * @brief Set warning marker at line
     * @param line Line number (1-based)
     * @param message Warning message
     */
    void setWarningMarker(int line, const QString& message);
    
    /**
     * @brief Clear all error markers
     */
    void clearErrorMarkers();
    
    /**
     * @brief Clear all markers
     */
    void clearAllMarkers();
    
    /**
     * @brief Apply theme to editor
     * @param themeName Theme name (dark, light, etc.)
     */
    void applyTheme(const QString& themeName);
    
signals:
    void modificationChanged(bool modified);
    
private slots:
    void onTextChanged();
    
private:
    void setupEditor();
    void setupLexer();
    void setupMargins();
    void setupFolding();
    void setupAutoCompletion();
    void setupBraceMatching();
    
    QString m_filePath;
    QsciLexerCPP* m_lexer = nullptr;
    QsciAPIs* m_apis = nullptr;
    int m_errorMarkerHandle = -1;
    int m_warningMarkerHandle = -1;
    QMap<int, QString> m_errorMarkers;  // line -> error message
    bool m_isModified = false;
};

#endif // CODEEDITOR_H
