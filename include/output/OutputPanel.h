#ifndef OUTPUTPANEL_H
#define OUTPUTPANEL_H

#include <QTabWidget>

class BuildOutputWidget;
class RunOutputWidget;
class ProblemsWidget;

/**
 * @brief Container for output tabs (Build, Run, Problems)
 */
class OutputPanel : public QTabWidget {
    Q_OBJECT
    
public:
    explicit OutputPanel(QWidget *parent = nullptr);
    ~OutputPanel() override = default;
    
    /**
     * @brief Get build output widget
     * @return Pointer to build output widget
     */
    BuildOutputWidget* buildOutput() const { return m_buildOutput; }
    
    /**
     * @brief Get run output widget
     * @return Pointer to run output widget
     */
    RunOutputWidget* runOutput() const { return m_runOutput; }
    
    /**
     * @brief Get problems widget
     * @return Pointer to problems widget
     */
    ProblemsWidget* problems() const { return m_problems; }
    
    /**
     * @brief Show build tab
     */
    void showBuildTab();
    
    /**
     * @brief Show run tab
     */
    void showRunTab();
    
    /**
     * @brief Show problems tab
     */
    void showProblemsTab();
    
private:
    BuildOutputWidget* m_buildOutput;
    RunOutputWidget* m_runOutput;
    ProblemsWidget* m_problems;
};

#endif // OUTPUTPANEL_H
