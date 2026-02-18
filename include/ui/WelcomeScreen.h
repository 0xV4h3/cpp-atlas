#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

class WelcomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);
    ~WelcomeScreen() = default;

signals:
    // IDE Mode signals
    void newFileRequested();
    void openFileRequested();
    void openFolderRequested();
    void recentProjectSelected(const QString& projectPath);
    
    // Project signals
    void createProjectRequested();
    void openProjectRequested();
    
    // Quiz Mode signal (for future implementation)
    void quizModeRequested();
    
    // Continue without project
    void continueWithoutProjectRequested();

private slots:
    void onRecentProjectClicked(QListWidgetItem* item);
    void onRecentProjectDoubleClicked(QListWidgetItem* item);
    void clearRecentProjects();

private:
    void setupUI();
    void loadRecentProjects();
    void createModeSelectionArea();
    void createQuickActionsArea(QWidget* parent);
    void createRecentProjectsArea(QWidget* parent);
    void applyTheme();

    // Mode selection
    QWidget* m_modeSelector;
    QPushButton* m_ideModeBtn;
    QPushButton* m_quizModeBtn;
    
    // Quick actions (IDE mode)
    QPushButton* m_newFileBtn;
    QPushButton* m_openFileBtn;
    QPushButton* m_openFolderBtn;
    QPushButton* m_createProjectBtn;
    QPushButton* m_openProjectBtn;
    
    // Recent projects
    QListWidget* m_recentProjectsList;
    QPushButton* m_clearRecentBtn;
    
    // Labels
    QLabel* m_logoLabel;
    QLabel* m_titleLabel;
    QLabel* m_subtitleLabel;
};

#endif // WELCOMESCREEN_H
