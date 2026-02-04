#include "ui/WelcomeWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

WelcomeWidget::WelcomeWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void WelcomeWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // Title
    m_titleLabel = new QLabel("CppAtlas", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    QLabel* subtitleLabel = new QLabel("Educational C++ IDE", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);
    
    mainLayout->addSpacing(30);
    
    // Horizontal layout for two columns
    QHBoxLayout* columnsLayout = new QHBoxLayout();
    
    // Start section
    QGroupBox* startGroup = new QGroupBox("Start", this);
    QVBoxLayout* startLayout = new QVBoxLayout(startGroup);
    
    m_newFileBtn = new QPushButton("📄 New File", this);
    connect(m_newFileBtn, &QPushButton::clicked, this, &WelcomeWidget::newFileRequested);
    startLayout->addWidget(m_newFileBtn);
    
    m_openFolderBtn = new QPushButton("📂 Open Folder", this);
    connect(m_openFolderBtn, &QPushButton::clicked, this, &WelcomeWidget::openFolderRequested);
    startLayout->addWidget(m_openFolderBtn);
    
    m_openFileBtn = new QPushButton("📄 Open File", this);
    connect(m_openFileBtn, &QPushButton::clicked, this, &WelcomeWidget::openFileRequested);
    startLayout->addWidget(m_openFileBtn);
    
    startLayout->addStretch();
    columnsLayout->addWidget(startGroup);
    
    // Recent projects section
    QGroupBox* recentGroup = new QGroupBox("Recent Projects", this);
    QVBoxLayout* recentLayout = new QVBoxLayout(recentGroup);
    
    m_recentList = new QListWidget(this);
    connect(m_recentList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        if (item) {
            emit recentProjectClicked(item->text());
        }
    });
    recentLayout->addWidget(m_recentList);
    
    columnsLayout->addWidget(recentGroup);
    
    mainLayout->addLayout(columnsLayout);
    
    setLayout(mainLayout);
}

void WelcomeWidget::updateRecentProjects(const QStringList& projects) {
    m_recentList->clear();
    for (const QString& project : projects) {
        m_recentList->addItem("📁 " + project);
    }
}
