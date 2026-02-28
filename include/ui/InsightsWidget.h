#ifndef INSIGHTSWIDGET_H
#define INSIGHTSWIDGET_H

#include <QWidget>
#include "tools/CppInsightsRunner.h"

class QsciScintilla;
class QsciLexerCPP;
class QPushButton;
class QLabel;
class QSplitter;

/**
 * @brief Widget for the C++ Insights tab in AnalysisPanel.
 *
 * Layout:
 *   [Toolbar: Run Insights button | status]
 *   [QSplitter horizontal]
 *     Left:  source code mirror (read-only QsciScintilla, synced from editor)
 *     Right: transformed output (read-only QsciScintilla, C++ syntax highlighting)
 *
 * Integration:
 *   - Call setSourceCode(code, filePath) when the active editor changes.
 *   - Call setStandard(standard) when the toolbar standard combo changes.
 *   - Connects to ThemeManager::themeChanged for live theme updates.
 *   - Uses CppInsightsRunner for async process execution.
 *
 * CppInsights invocation:
 *   insights <tmp_file> -- -std=<standard> [extra flags]
 *   See: https://github.com/andreasfertig/cppinsights#usage
 */
class InsightsWidget : public QWidget {
    Q_OBJECT

public:
    explicit InsightsWidget(QWidget* parent = nullptr);
    ~InsightsWidget() override = default;

    /**
     * @brief Supply source code from the currently active editor.
     * Saves to a temp file on Run. Does NOT auto-run.
     */
    void setSourceCode(const QString& code, const QString& filePath);

    /**
     * @brief Set the C++ standard used when invoking insights.
     * Forwarded from AnalysisPanel::setStandard().
     */
    void setStandard(const QString& standard);

public slots:
    void runInsights();
    void onThemeChanged(const QString& themeName);

private slots:
    void onInsightsFinished(bool success, const QString& output, const QString& error);
    void onInsightsStarted();
    void onProgressMessage(const QString& msg);
    void stopProcess();

private:
    void setupUi();
    void setupLexer(QsciScintilla* editor, QsciLexerCPP* lexer);
    void applyThemeToEditor(QsciScintilla* editor, const QString& themeName);

    // Toolbar widgets
    QPushButton* m_runButton;
    QPushButton* m_stopButton = nullptr;
    QLabel*      m_statusLabel;

    // Editor panes
    QsciScintilla* m_sourceEditor;   // Left: source mirror (read-only)
    QsciScintilla* m_outputEditor;   // Right: transformed output (read-only)
    QsciLexerCPP*  m_sourceLexer;
    QsciLexerCPP*  m_outputLexer;

    // Backend
    CppInsightsRunner* m_runner;

    // State
    QString m_currentSourceCode;
    QString m_currentFilePath;
    QString m_standard = QStringLiteral("c++17");
};

#endif // INSIGHTSWIDGET_H
