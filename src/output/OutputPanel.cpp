#include "output/OutputPanel.h"
#include "output/TerminalWidget.h"
#include "output/ProblemsWidget.h"
#include "ui/ThemeManager.h"
#include <QTabBar>

OutputPanel::OutputPanel(QWidget *parent)
    : QTabWidget(parent)
{
    m_terminal = new TerminalWidget(this);
    m_problems = new ProblemsWidget(this);

    addTab(m_terminal, "Terminal");
    addTab(m_problems, "Problems");

    setTabPosition(QTabWidget::South);

    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &OutputPanel::applyTheme);
}

void OutputPanel::showTerminalTab() { setCurrentWidget(m_terminal); }
void OutputPanel::showProblemsTab() { setCurrentWidget(m_problems); }

void OutputPanel::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();

    setStyleSheet(QString(R"(
        OutputPanel {
            background-color: %1;
        }
        OutputPanel QTabWidget::pane {
            background-color: %1;
            border: none;
            border-top: 1px solid %2;
        }
        OutputPanel QTabBar::tab {
            background-color: %3;
            color: %4;
            padding: 6px 16px;
            border: 1px solid %2;
            border-bottom: none;
        }
        OutputPanel QTabBar::tab:selected {
            background-color: %1;
            color: %5;
        }
        OutputPanel QTabBar::tab:hover:!selected {
            background-color: %6;
        }

        /* Problems: table background fix */
        QTableWidget {
            background-color: %1;
            alternate-background-color: %7;
            gridline-color: %2;
            color: %5;
            border: none;
        }
        QTableWidget QTableCornerButton::section {
            background-color: %3;
            border: 1px solid %2;
        }
        /* Qt internal name for the corner button */
        QTableView > QAbstractButton {
            background-color: %3;
            border: 1px solid %2;
        }
        QHeaderView::section {
            background-color: %3;
            color: %5;
            border: none;
            border-right: 1px solid %2;
            padding: 4px;
        }
        QHeaderView {
            background-color: %3;
        }
        QTableWidget::item {
            color: %5;
            background-color: %1;
        }
        QTableWidget::item:selected {
            background-color: %8;
            color: %5;
        }
        QTableWidget::item:alternate {
            background-color: %7;
        }

        /* Filter toolbar */
        QComboBox {
            background-color: %3;
            color: %5;
            border: 1px solid %2;
            border-radius: 3px;
            padding: 3px 6px;
        }
        QComboBox QAbstractItemView {
            background-color: %3;
            color: %5;
            selection-background-color: %8;
        }
        QComboBox::drop-down { border: none; }

        QPushButton {
            background-color: %3;
            color: %5;
            border: 1px solid %2;
            border-radius: 3px;
            padding: 3px 10px;
        }
        QPushButton:hover { background-color: %8; color: white; border-color: %8; }

        QLabel { color: %5; }
        QWidget { background-color: %1; }
    )")
                      .arg(t.panelBackground.name())          // %1 - main bg
                      .arg(t.border.name())                   // %2 - border
                      .arg(t.sidebarBackground.name())        // %3 - tab inactive / header bg
                      .arg(t.textSecondary.name())            // %4 - tab inactive text
                      .arg(t.textPrimary.name())              // %5 - primary text
                      .arg(t.tabInactive.lighter(110).name()) // %6 - tab hover
                      .arg(t.editorBackground.name())         // %7 - alternate row
                      .arg(t.accent.name())                   // %8 - selection / hover accent
                  );
}
