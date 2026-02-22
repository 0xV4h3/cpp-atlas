#ifndef ASSEMBLYWIDGET_H
#define ASSEMBLYWIDGET_H

#include <QMap>
#include <QWidget>
#include "tools/AssemblyRunner.h"

class QsciScintilla;
class QsciLexerCPP;
class QComboBox;
class QPushButton;
class QLabel;
class QSplitter;

/**
 * @brief Widget for the Assembly output tab.
 *
 * Layout:
 *   [Toolbar: compiler combo | standard combo | optimization combo |
 *             syntax combo | Run button | status label]
 *   [QSplitter horizontal]
 *     Left:  source code mirror (read-only QsciScintilla, synced from editor)
 *     Right: assembly output   (read-only QsciScintilla, plain highlighting)
 *
 * Bidirectional line highlighting:
 *   • When the cursor moves in the assembly pane, the corresponding source
 *     line is highlighted in the source mirror and sourceLineActivated() is
 *     emitted (so MainWindow can optionally navigate the real editor).
 *   • Call highlightSourceLine(n) from outside to scroll both panes to the
 *     assembly lines that correspond to source line n.
 *
 * Integration:
 *   • Call setSourceCode(code, filePath) when the active editor changes.
 *   • Connect sourceLineActivated(int) to navigate the active CodeEditor.
 *   • Connects to ThemeManager::themeChanged for live theme updates.
 *   • Uses AssemblyRunner for async process execution.
 */
class AssemblyWidget : public QWidget {
    Q_OBJECT

public:
    explicit AssemblyWidget(QWidget* parent = nullptr);
    ~AssemblyWidget() override = default;

    /**
     * @brief Supply source code from the currently active editor.
     * Saves to a temp file on Run.  Does NOT auto-run.
     */
    void setSourceCode(const QString& code, const QString& filePath);

    /**
     * @brief Highlight the assembly lines that originate from \a sourceLine.
     * Also scrolls the source mirror to that line.
     * @param sourceLine 1-based source line number.
     */
    void highlightSourceLine(int sourceLine);

public slots:
    void runAssembly();
    void onThemeChanged(const QString& themeName);

signals:
    /**
     * @brief Emitted when the user clicks on an assembly line that maps back
     * to a source line, so the main editor can navigate there.
     * @param sourceLine 1-based source line number.
     */
    void sourceLineActivated(int sourceLine);

private slots:
    void onRunnerStarted();
    void onRunnerFinished(bool success, const QString& output, const QString& error);
    void onProgressMessage(const QString& msg);
    void onLineMapReady(const QMap<int, int>& asmLineToSrcLine);
    void onAsmCursorPositionChanged(int line, int col);
    void onCompilerComboChanged(int index);

private:
    void setupUi();
    void setupLexer(QsciScintilla* editor, QsciLexerCPP* lexer);
    void applyThemeToEditor(QsciScintilla* editor, const QString& themeName);
    void populateCompilerCombo();
    void clearHighlights();

    // Toolbar widgets
    QComboBox*   m_compilerCombo;
    QComboBox*   m_standardCombo;
    QComboBox*   m_optimizationCombo;
    QComboBox*   m_syntaxCombo;
    QPushButton* m_runButton;
    QLabel*      m_statusLabel;

    // Editor panes
    QsciScintilla* m_sourceEditor;    // Left: source mirror (read-only)
    QsciScintilla* m_asmEditor;       // Right: assembly output (read-only)
    QsciLexerCPP*  m_sourceLexer;
    QsciLexerCPP*  m_asmLexer;

    // Highlighting marker handles
    int m_srcHighlightMarker  = -1;
    int m_asmHighlightMarker  = -1;

    // Backend
    AssemblyRunner* m_runner;

    // State
    QString         m_currentSourceCode;
    QString         m_currentFilePath;
    QMap<int, int>  m_asmLineToSrcLine;  // asm line (1-based) → src line (1-based)
    QMap<int, int>  m_srcLineToFirstAsm; // src line → first asm line for it
};

#endif // ASSEMBLYWIDGET_H
