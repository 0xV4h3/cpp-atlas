#ifndef GITPANELWIDGET_H
#define GITPANELWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include "vcs/GitManager.h"

class GitPanelWidget : public QWidget {
    Q_OBJECT
public:
    explicit GitPanelWidget(QWidget* parent = nullptr);
    
    void setGitManager(GitManager* manager);
    void refresh();

signals:
    void fileDoubleClicked(const QString& filePath);

private slots:
    void onStageClicked();
    void onUnstageClicked();
    void onCommitClicked();
    void onRefreshClicked();
    void onStatusChanged();

private:
    void setupUi();
    void updateFileList();
    
    GitManager* m_git = nullptr;
    
    QLabel* m_branchLabel;
    QTreeWidget* m_stagedTree;
    QTreeWidget* m_changesTree;
    QPlainTextEdit* m_commitMessage;
    QPushButton* m_commitButton;
    QPushButton* m_stageAllButton;
    QPushButton* m_unstageAllButton;
    QPushButton* m_refreshButton;
};

#endif // GITPANELWIDGET_H
