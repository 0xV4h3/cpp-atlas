#include "ui/AnalysisPanel.h"
#include "ui/InsightsWidget.h"
#include "ui/AssemblyWidget.h"
#include "ui/BenchmarkWidget.h"

#include <QFont>

AnalysisPanel::AnalysisPanel(QWidget* parent)
    : QTabWidget(parent)
{
    // Use document mode so the tab bar matches the EditorTabWidget style
    setDocumentMode(true);

    // Create sub-widgets
    m_insights  = new InsightsWidget(this);
    m_assembly  = new AssemblyWidget(this);
    m_benchmark = new BenchmarkWidget(this);

    addTab(m_insights,  QStringLiteral("Insights"));
    addTab(m_assembly,  QStringLiteral("Assembly"));
    addTab(m_benchmark, QStringLiteral("Benchmark"));

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setMinimumWidth(100);
    setMaximumWidth(2000);

    // Forward AssemblyWidget line-activation signal to MainWindow
    connect(m_assembly, &AssemblyWidget::sourceLineActivated,
            this,       &AnalysisPanel::sourceLineActivated);

    // ThemeManager connections are handled inside each sub-widget —
    // no additional wiring needed here.
}

void AnalysisPanel::setSourceCode(const QString& code,
                                  const QString& filePath) {
    m_insights->setSourceCode(code, filePath);
    m_assembly->setSourceCode(code, filePath);
    // BenchmarkWidget has its own independent editor — not forwarded.
}

void AnalysisPanel::setCompilerId(const QString& id) {
    m_assembly->setCompilerId(id);
    m_benchmark->setCompilerId(id);
}

void AnalysisPanel::setStandard(const QString& standard) {
    m_insights->setStandard(standard);
    m_assembly->setStandard(standard);
    m_benchmark->setStandard(standard);
}

void AnalysisPanel::applyToolEditorSettings(const AppSettings& s) {
    // Insights
    {
        const QString family = s.analysisEditorFontFamily(QStringLiteral("insights"));
        const int     size   = s.analysisEditorFontSize(QStringLiteral("insights"));
        const bool    lnums  = s.analysisEditorShowLineNumbers(QStringLiteral("insights"));
        const bool    wrap   = s.analysisEditorWordWrap(QStringLiteral("insights"));
        QFont f(family.isEmpty() ? QStringLiteral("Monospace") : family,
                size > 0 ? size : 10);
        m_insights->applyEditorSettings(f, lnums, wrap);
    }
    // Assembly
    {
        const QString family = s.analysisEditorFontFamily(QStringLiteral("assembly"));
        const int     size   = s.analysisEditorFontSize(QStringLiteral("assembly"));
        const bool    lnums  = s.analysisEditorShowLineNumbers(QStringLiteral("assembly"));
        const bool    wrap   = s.analysisEditorWordWrap(QStringLiteral("assembly"));
        QFont f(family.isEmpty() ? QStringLiteral("Monospace") : family,
                size > 0 ? size : 10);
        m_assembly->applyEditorSettings(f, lnums, wrap);
    }
    // Benchmark
    {
        const QString family = s.analysisEditorFontFamily(QStringLiteral("benchmark"));
        const int     size   = s.analysisEditorFontSize(QStringLiteral("benchmark"));
        const bool    lnums  = s.analysisEditorShowLineNumbers(QStringLiteral("benchmark"));
        const bool    wrap   = s.analysisEditorWordWrap(QStringLiteral("benchmark"));
        QFont f(family.isEmpty() ? QStringLiteral("Monospace") : family,
                size > 0 ? size : 10);
        m_benchmark->applyEditorSettings(f, lnums, wrap);
    }
}
