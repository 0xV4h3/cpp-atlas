#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

class WelcomeScreen : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomeScreen(QWidget* parent = nullptr);

    void setReturnToProjectVisible(bool visible);
    void setCurrentUser(const QString& displayName, const QString& username, bool isAdmin);

signals:
    void newFileRequested();
    void openFileRequested();
    void openFolderRequested();
    void createProjectRequested();
    void openProjectRequested();
    void recentProjectSelected(const QString& path);
    void quizModeRequested();
    void continueWithoutProjectRequested();
    void returnToProjectRequested();
    void ideModeRequested();   // NEW: emitted when IDE Mode button clicked

private slots:
    void onRecentProjectClicked(QListWidgetItem* item);
    void onRecentProjectDoubleClicked(QListWidgetItem* item);
    void clearRecentProjects();
    void applyTheme();

private:
    void setupUI();
    void createModeSelectionArea();
    void createQuickActionsArea(QWidget* actionsWidget);
    void createRecentProjectsArea(QWidget* recentWidget);
    void loadRecentProjects();

    QLabel*      m_logoLabel      = nullptr;
    QLabel*      m_titleLabel     = nullptr;
    QLabel*      m_subtitleLabel  = nullptr;
    QLabel*      m_userInfoLabel  = nullptr;

    QWidget*     m_modeSelector   = nullptr;
    QPushButton* m_ideModeBtn     = nullptr;
    QPushButton* m_quizModeBtn    = nullptr;

    QPushButton* m_newFileBtn         = nullptr;
    QPushButton* m_openFileBtn        = nullptr;
    QPushButton* m_openFolderBtn      = nullptr;
    QPushButton* m_createProjectBtn   = nullptr;
    QPushButton* m_openProjectBtn     = nullptr;

    QListWidget* m_recentProjectsList     = nullptr;
    QPushButton* m_clearRecentBtn         = nullptr;

    // Footer — only one is visible at a time (controlled by mainwindow)
    QPushButton* m_returnToProjectBtn     = nullptr;
    QPushButton* m_continueWithoutProjectBtn = nullptr;
};
