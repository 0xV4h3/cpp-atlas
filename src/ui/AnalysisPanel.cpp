#include "ui/AnalysisPanel.h"
#include "ui/InsightsWidget.h"
#include "ui/AssemblyWidget.h"
#include "ui/BenchmarkWidget.h"

AnalysisPanel::AnalysisPanel(QWidget* parent)
    : QTabWidget(parent)
{
    // Create sub-widgets
    m_insights  = new InsightsWidget(this);
    m_assembly  = new AssemblyWidget(this);
    m_benchmark = new BenchmarkWidget(this);

    addTab(m_insights,  QStringLiteral("Insights"));
    addTab(m_assembly,  QStringLiteral("Assembly"));
    addTab(m_benchmark, QStringLiteral("Benchmark"));

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
    m_assembly->setStandard(standard);
    m_benchmark->setStandard(standard);
}
