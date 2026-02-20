#include "output/OutputPanel.h"
#include "output/TerminalWidget.h"
#include "output/ProblemsWidget.h"

OutputPanel::OutputPanel(QWidget *parent)
    : QTabWidget(parent)
{
    m_terminal = new TerminalWidget(this);
    m_problems = new ProblemsWidget(this);
    
    addTab(m_terminal, "Terminal");
    addTab(m_problems, "Problems");
    
    setTabPosition(QTabWidget::South);
}

void OutputPanel::showTerminalTab() {
    setCurrentWidget(m_terminal);
}

void OutputPanel::showProblemsTab() {
    setCurrentWidget(m_problems);
}
