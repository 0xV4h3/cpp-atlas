#include "ui/FindReplaceDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>

FindReplaceDialog::FindReplaceDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , m_mode(mode)
{
    setWindowTitle(mode == Find ? "Find" : "Find and Replace");
    setModal(false);
    setupUi();
}

void FindReplaceDialog::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Find/Replace input
    QGridLayout* inputLayout = new QGridLayout();
    
    QLabel* findLabel = new QLabel("Find:", this);
    m_findLineEdit = new QLineEdit(this);
    m_findLineEdit->setMinimumWidth(300);
    
    inputLayout->addWidget(findLabel, 0, 0);
    inputLayout->addWidget(m_findLineEdit, 0, 1);
    
    if (m_mode == Replace) {
        QLabel* replaceLabel = new QLabel("Replace:", this);
        m_replaceLineEdit = new QLineEdit(this);
        inputLayout->addWidget(replaceLabel, 1, 0);
        inputLayout->addWidget(m_replaceLineEdit, 1, 1);
    }
    
    mainLayout->addLayout(inputLayout);
    
    // Options
    QGroupBox* optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout* optionsLayout = new QVBoxLayout(optionsGroup);
    
    m_caseSensitiveCheckBox = new QCheckBox("Case sensitive", this);
    m_wholeWordCheckBox = new QCheckBox("Whole word", this);
    m_regexCheckBox = new QCheckBox("Regular expression", this);
    
    optionsLayout->addWidget(m_caseSensitiveCheckBox);
    optionsLayout->addWidget(m_wholeWordCheckBox);
    optionsLayout->addWidget(m_regexCheckBox);
    
    mainLayout->addWidget(optionsGroup);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_findNextButton = new QPushButton("Find Next", this);
    m_findPrevButton = new QPushButton("Find Previous", this);
    buttonLayout->addWidget(m_findNextButton);
    buttonLayout->addWidget(m_findPrevButton);
    
    if (m_mode == Replace) {
        m_replaceButton = new QPushButton("Replace", this);
        m_replaceAllButton = new QPushButton("Replace All", this);
        buttonLayout->addWidget(m_replaceButton);
        buttonLayout->addWidget(m_replaceAllButton);
        
        connect(m_replaceButton, &QPushButton::clicked, this, &FindReplaceDialog::replaceNext);
        connect(m_replaceAllButton, &QPushButton::clicked, this, &FindReplaceDialog::replaceAll);
    }
    
    buttonLayout->addStretch();
    
    m_closeButton = new QPushButton("Close", this);
    buttonLayout->addWidget(m_closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connections
    connect(m_findNextButton, &QPushButton::clicked, this, &FindReplaceDialog::findNext);
    connect(m_findPrevButton, &QPushButton::clicked, this, &FindReplaceDialog::findPrevious);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::close);
    
    // Enter key triggers find next
    m_findNextButton->setDefault(true);
    
    // Set minimum size
    setMinimumWidth(450);
}

QString FindReplaceDialog::findText() const {
    return m_findLineEdit->text();
}

QString FindReplaceDialog::replaceText() const {
    return m_replaceLineEdit ? m_replaceLineEdit->text() : QString();
}

bool FindReplaceDialog::caseSensitive() const {
    return m_caseSensitiveCheckBox->isChecked();
}

bool FindReplaceDialog::wholeWord() const {
    return m_wholeWordCheckBox->isChecked();
}

bool FindReplaceDialog::useRegex() const {
    return m_regexCheckBox->isChecked();
}
