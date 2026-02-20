#ifndef OUTPUTPANEL_H
#define OUTPUTPANEL_H

#include <QTabWidget>

class TerminalWidget;
class ProblemsWidget;

/**
 * @brief Container for output tabs (Terminal, Problems)
 */
class OutputPanel : public QTabWidget {
    Q_OBJECT
    
public:
    explicit OutputPanel(QWidget *parent = nullptr);
    ~OutputPanel() override = default;
    
    /**
     * @brief Get terminal widget
     * @return Pointer to terminal widget
     */
    TerminalWidget* terminal() const { return m_terminal; }
    
    /**
     * @brief Get problems widget
     * @return Pointer to problems widget
     */
    ProblemsWidget* problems() const { return m_problems; }
    
    /**
     * @brief Show terminal tab
     */
    void showTerminalTab();
    
    /**
     * @brief Show problems tab
     */
    void showProblemsTab();
    
private:
    TerminalWidget* m_terminal;
    ProblemsWidget* m_problems;
};

#endif // OUTPUTPANEL_H
