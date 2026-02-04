#include "vcs/GitPanelWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

GitPanelWidget::GitPanelWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void GitPanelWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Branch label
    m_branchLabel = new QLabel("Branch: -", this);
    m_branchLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    mainLayout->addWidget(m_branchLabel);
    
    // Staged changes
    QGroupBox* stagedGroup = new QGroupBox("Staged Changes", this);
    QVBoxLayout* stagedLayout = new QVBoxLayout(stagedGroup);
    m_stagedTree = new QTreeWidget(this);
    m_stagedTree->setHeaderLabels({"File", "Status"});
    m_stagedTree->setRootIsDecorated(false);
    stagedLayout->addWidget(m_stagedTree);
    
    m_unstageAllButton = new QPushButton("Unstage All", this);
    connect(m_unstageAllButton, &QPushButton::clicked, this, &GitPanelWidget::onUnstageClicked);
    stagedLayout->addWidget(m_unstageAllButton);
    
    mainLayout->addWidget(stagedGroup);
    
    // Unstaged changes
    QGroupBox* changesGroup = new QGroupBox("Changes", this);
    QVBoxLayout* changesLayout = new QVBoxLayout(changesGroup);
    m_changesTree = new QTreeWidget(this);
    m_changesTree->setHeaderLabels({"File", "Status"});
    m_changesTree->setRootIsDecorated(false);
    changesLayout->addWidget(m_changesTree);
    
    m_stageAllButton = new QPushButton("Stage All", this);
    connect(m_stageAllButton, &QPushButton::clicked, this, &GitPanelWidget::onStageClicked);
    changesLayout->addWidget(m_stageAllButton);
    
    mainLayout->addWidget(changesGroup);
    
    // Commit message
    QGroupBox* commitGroup = new QGroupBox("Commit", this);
    QVBoxLayout* commitLayout = new QVBoxLayout(commitGroup);
    m_commitMessage = new QPlainTextEdit(this);
    m_commitMessage->setPlaceholderText("Enter commit message...");
    m_commitMessage->setMaximumHeight(60);
    commitLayout->addWidget(m_commitMessage);
    
    QHBoxLayout* commitButtonLayout = new QHBoxLayout();
    m_commitButton = new QPushButton("Commit", this);
    connect(m_commitButton, &QPushButton::clicked, this, &GitPanelWidget::onCommitClicked);
    commitButtonLayout->addWidget(m_commitButton);
    
    m_refreshButton = new QPushButton("Refresh", this);
    connect(m_refreshButton, &QPushButton::clicked, this, &GitPanelWidget::onRefreshClicked);
    commitButtonLayout->addWidget(m_refreshButton);
    
    commitLayout->addLayout(commitButtonLayout);
    
    mainLayout->addWidget(commitGroup);
    
    setLayout(mainLayout);
}

void GitPanelWidget::setGitManager(GitManager* manager) {
    m_git = manager;
    if (m_git) {
        connect(m_git, &GitManager::statusChanged, this, &GitPanelWidget::onStatusChanged);
        connect(m_git, &GitManager::branchChanged, this, [this](const QString& branch) {
            m_branchLabel->setText(QString("Branch: %1").arg(branch));
        });
        refresh();
    }
}

void GitPanelWidget::refresh() {
    if (!m_git || !m_git->isGitRepository()) {
        m_branchLabel->setText("Branch: Not a git repository");
        m_stagedTree->clear();
        m_changesTree->clear();
        return;
    }
    
    updateFileList();
}

void GitPanelWidget::updateFileList() {
    if (!m_git) return;
    
    m_stagedTree->clear();
    m_changesTree->clear();
    
    QString branch = m_git->currentBranch();
    m_branchLabel->setText(QString("Branch: %1").arg(branch.isEmpty() ? "-" : branch));
    
    QList<GitFileStatus> statusList = m_git->status();
    
    for (const auto& fs : statusList) {
        QString statusText;
        
        if (fs.indexStatus != GitFileStatus::Clean) {
            // Staged file
            if (fs.indexStatus == GitFileStatus::Added) statusText = "Added";
            else if (fs.indexStatus == GitFileStatus::Modified) statusText = "Modified";
            else if (fs.indexStatus == GitFileStatus::Deleted) statusText = "Deleted";
            else if (fs.indexStatus == GitFileStatus::Renamed) statusText = "Renamed";
            
            QTreeWidgetItem* item = new QTreeWidgetItem(m_stagedTree);
            item->setText(0, fs.filePath);
            item->setText(1, statusText);
        }
        
        if (fs.workTreeStatus != GitFileStatus::Clean) {
            // Unstaged file
            if (fs.workTreeStatus == GitFileStatus::Modified) statusText = "Modified";
            else if (fs.workTreeStatus == GitFileStatus::Deleted) statusText = "Deleted";
            else if (fs.workTreeStatus == GitFileStatus::Untracked) statusText = "Untracked";
            
            QTreeWidgetItem* item = new QTreeWidgetItem(m_changesTree);
            item->setText(0, fs.filePath);
            item->setText(1, statusText);
        }
    }
}

void GitPanelWidget::onStageClicked() {
    if (!m_git) return;
    
    if (m_git->stageAll()) {
        refresh();
    } else {
        QMessageBox::warning(this, "Git Error", "Failed to stage files");
    }
}

void GitPanelWidget::onUnstageClicked() {
    if (!m_git) return;
    
    if (m_git->unstageAll()) {
        refresh();
    } else {
        QMessageBox::warning(this, "Git Error", "Failed to unstage files");
    }
}

void GitPanelWidget::onCommitClicked() {
    if (!m_git) return;
    
    QString message = m_commitMessage->toPlainText().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Git Error", "Please enter a commit message");
        return;
    }
    
    if (m_git->commit(message)) {
        m_commitMessage->clear();
        refresh();
        QMessageBox::information(this, "Git", "Commit successful");
    } else {
        QMessageBox::warning(this, "Git Error", "Failed to commit. Make sure you have staged changes.");
    }
}

void GitPanelWidget::onRefreshClicked() {
    refresh();
}

void GitPanelWidget::onStatusChanged() {
    refresh();
}
