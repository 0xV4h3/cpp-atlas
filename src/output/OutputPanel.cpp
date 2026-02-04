#include "output/OutputPanel.h"
#include "output/BuildOutputWidget.h"
#include "output/RunOutputWidget.h"
#include "output/ProblemsWidget.h"

OutputPanel::OutputPanel(QWidget *parent)
    : QTabWidget(parent)
{
    m_buildOutput = new BuildOutputWidget(this);
    m_runOutput = new RunOutputWidget(this);
    m_problems = new ProblemsWidget(this);
    
    addTab(m_buildOutput, "Build");
    addTab(m_runOutput, "Run");
    addTab(m_problems, "Problems");
    
    setTabPosition(QTabWidget::South);
}

void OutputPanel::showBuildTab() {
    setCurrentWidget(m_buildOutput);
}

void OutputPanel::showRunTab() {
    setCurrentWidget(m_runOutput);
}

void OutputPanel::showProblemsTab() {
    setCurrentWidget(m_problems);
}
