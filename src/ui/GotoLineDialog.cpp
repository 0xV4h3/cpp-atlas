#include "ui/GotoLineDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

GotoLineDialog::GotoLineDialog(int maxLine, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Go to Line");
    setModal(true);
    setupUi();
    
    m_lineSpinBox->setMaximum(maxLine);
    m_lineSpinBox->setMinimum(1);
    m_lineSpinBox->setValue(1);
}

void GotoLineDialog::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Line number input
    QHBoxLayout* inputLayout = new QHBoxLayout();
    QLabel* label = new QLabel("Line number:", this);
    m_lineSpinBox = new QSpinBox(this);
    m_lineSpinBox->setMinimumWidth(150);
    inputLayout->addWidget(label);
    inputLayout->addWidget(m_lineSpinBox);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);
    
    m_okButton->setDefault(true);
    
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    
    // Connections
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // Set fixed size
    setFixedSize(300, 100);
}

int GotoLineDialog::lineNumber() const {
    return m_lineSpinBox->value();
}
