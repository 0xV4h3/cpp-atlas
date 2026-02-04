#include "ui/SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGroupBox>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
    loadSettings();
}

void SettingsDialog::setupUi() {
    setWindowTitle("Settings");
    resize(700, 500);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    
    // Page list
    m_pageList = new QListWidget(this);
    m_pageList->addItem("General");
    m_pageList->addItem("Editor");
    m_pageList->addItem("Compiler");
    m_pageList->setMaximumWidth(150);
    connect(m_pageList, &QListWidget::currentRowChanged, this, &SettingsDialog::onPageChanged);
    mainLayout->addWidget(m_pageList);
    
    // Pages
    m_pages = new QStackedWidget(this);
    
    // General page
    QWidget* generalPage = new QWidget();
    QVBoxLayout* generalLayout = new QVBoxLayout(generalPage);
    QFormLayout* generalForm = new QFormLayout();
    
    m_themeCombo = new QComboBox(this);
    m_themeCombo->addItems({"Dark", "Light", "Dracula", "Monokai"});
    generalForm->addRow("Theme:", m_themeCombo);
    
    m_restoreSessionCheck = new QCheckBox("Restore session on startup", this);
    generalForm->addRow("", m_restoreSessionCheck);
    
    generalLayout->addLayout(generalForm);
    generalLayout->addStretch();
    m_pages->addWidget(generalPage);
    
    // Editor page
    QWidget* editorPage = new QWidget();
    QVBoxLayout* editorLayout = new QVBoxLayout(editorPage);
    QFormLayout* editorForm = new QFormLayout();
    
    m_fontCombo = new QFontComboBox(this);
    editorForm->addRow("Font Family:", m_fontCombo);
    
    m_fontSizeSpinBox = new QSpinBox(this);
    m_fontSizeSpinBox->setRange(6, 24);
    m_fontSizeSpinBox->setValue(10);
    editorForm->addRow("Font Size:", m_fontSizeSpinBox);
    
    m_tabWidthSpinBox = new QSpinBox(this);
    m_tabWidthSpinBox->setRange(1, 8);
    m_tabWidthSpinBox->setValue(4);
    editorForm->addRow("Tab Width:", m_tabWidthSpinBox);
    
    m_showLineNumbersCheck = new QCheckBox("Show line numbers", this);
    m_showLineNumbersCheck->setChecked(true);
    editorForm->addRow("", m_showLineNumbersCheck);
    
    m_wordWrapCheck = new QCheckBox("Word wrap", this);
    editorForm->addRow("", m_wordWrapCheck);
    
    editorLayout->addLayout(editorForm);
    editorLayout->addStretch();
    m_pages->addWidget(editorPage);
    
    // Compiler page
    QWidget* compilerPage = new QWidget();
    QVBoxLayout* compilerLayout = new QVBoxLayout(compilerPage);
    QFormLayout* compilerForm = new QFormLayout();
    
    m_defaultCompilerCombo = new QComboBox(this);
    m_defaultCompilerCombo->addItems({"g++", "clang++"});
    compilerForm->addRow("Default Compiler:", m_defaultCompilerCombo);
    
    m_defaultStandardCombo = new QComboBox(this);
    m_defaultStandardCombo->addItems({"c++11", "c++14", "c++17", "c++20", "c++23"});
    m_defaultStandardCombo->setCurrentText("c++17");
    compilerForm->addRow("Default Standard:", m_defaultStandardCombo);
    
    compilerLayout->addLayout(compilerForm);
    compilerLayout->addStretch();
    m_pages->addWidget(compilerPage);
    
    mainLayout->addWidget(m_pages);
    
    // Button box
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_pages);
    
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply,
        this
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &SettingsDialog::onApply);
    rightLayout->addWidget(buttonBox);
    
    mainLayout->addLayout(rightLayout);
    
    setLayout(mainLayout);
    
    m_pageList->setCurrentRow(0);
}

void SettingsDialog::loadSettings() {
    QSettings settings("CppAtlas", "CppAtlas");
    
    // General
    QString theme = settings.value("theme", "dark").toString();
    int themeIndex = m_themeCombo->findText(theme, Qt::MatchFixedString);
    if (themeIndex >= 0) {
        m_themeCombo->setCurrentIndex(themeIndex);
    }
    m_restoreSessionCheck->setChecked(settings.value("restoreSession", false).toBool());
    
    // Editor
    m_fontCombo->setCurrentFont(QFont(settings.value("editorFont", "Monospace").toString()));
    m_fontSizeSpinBox->setValue(settings.value("editorFontSize", 10).toInt());
    m_tabWidthSpinBox->setValue(settings.value("tabWidth", 4).toInt());
    m_showLineNumbersCheck->setChecked(settings.value("showLineNumbers", true).toBool());
    m_wordWrapCheck->setChecked(settings.value("wordWrap", false).toBool());
    
    // Compiler
    QString compiler = settings.value("defaultCompiler", "g++").toString();
    int compilerIndex = m_defaultCompilerCombo->findText(compiler);
    if (compilerIndex >= 0) {
        m_defaultCompilerCombo->setCurrentIndex(compilerIndex);
    }
    QString standard = settings.value("defaultStandard", "c++17").toString();
    int standardIndex = m_defaultStandardCombo->findText(standard);
    if (standardIndex >= 0) {
        m_defaultStandardCombo->setCurrentIndex(standardIndex);
    }
}

void SettingsDialog::saveSettings() {
    QSettings settings("CppAtlas", "CppAtlas");
    
    // General
    settings.setValue("theme", m_themeCombo->currentText().toLower());
    settings.setValue("restoreSession", m_restoreSessionCheck->isChecked());
    
    // Editor
    settings.setValue("editorFont", m_fontCombo->currentFont().family());
    settings.setValue("editorFontSize", m_fontSizeSpinBox->value());
    settings.setValue("tabWidth", m_tabWidthSpinBox->value());
    settings.setValue("showLineNumbers", m_showLineNumbersCheck->isChecked());
    settings.setValue("wordWrap", m_wordWrapCheck->isChecked());
    
    // Compiler
    settings.setValue("defaultCompiler", m_defaultCompilerCombo->currentText());
    settings.setValue("defaultStandard", m_defaultStandardCombo->currentText());
}

void SettingsDialog::onApply() {
    saveSettings();
    // Note: Apply button saves settings but does not close dialog
    // Only OK button should close the dialog
}

void SettingsDialog::onPageChanged(int row) {
    m_pages->setCurrentIndex(row);
}
