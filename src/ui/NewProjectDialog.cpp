#include "ui/NewProjectDialog.h"
#include "compiler/CompilerRegistry.h"
#include "compiler/ICompiler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStandardPaths>

NewProjectDialog::NewProjectDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Create New Project");
    setMinimumWidth(500);
    setupUI();
    loadCompilers();
    validateInput();
}

void NewProjectDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    
    // Project name
    QFormLayout* formLayout = new QFormLayout();
    
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("MyProject");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &NewProjectDialog::validateInput);
    formLayout->addRow("Project Name:", m_nameEdit);
    
    // Location
    QHBoxLayout* locationLayout = new QHBoxLayout();
    m_locationEdit = new QLineEdit(this);
    m_locationEdit->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    connect(m_locationEdit, &QLineEdit::textChanged, this, &NewProjectDialog::validateInput);
    locationLayout->addWidget(m_locationEdit);
    
    m_browseBtn = new QPushButton("Browse...", this);
    connect(m_browseBtn, &QPushButton::clicked, this, &NewProjectDialog::onBrowseLocation);
    locationLayout->addWidget(m_browseBtn);
    
    formLayout->addRow("Location:", locationLayout);
    mainLayout->addLayout(formLayout);
    
    // Create folder checkbox
    m_createFolderCheck = new QCheckBox("Create project folder", this);
    m_createFolderCheck->setChecked(true);
    connect(m_createFolderCheck, &QCheckBox::toggled, this, &NewProjectDialog::validateInput);
    mainLayout->addWidget(m_createFolderCheck);
    
    // Folder preview
    m_folderPreview = new QLabel(this);
    m_folderPreview->setStyleSheet("color: gray; font-style: italic; padding-left: 20px;");
    mainLayout->addWidget(m_folderPreview);
    
    // Build Configuration group
    QGroupBox* buildGroup = new QGroupBox("Build Configuration", this);
    QFormLayout* buildLayout = new QFormLayout(buildGroup);
    
    m_compilerCombo = new QComboBox(this);
    buildLayout->addRow("Compiler:", m_compilerCombo);
    
    m_standardCombo = new QComboBox(this);
    m_standardCombo->addItems({"C++11", "C++14", "C++17", "C++20", "C++23"});
    m_standardCombo->setCurrentText("C++17");
    buildLayout->addRow("C++ Standard:", m_standardCombo);
    
    mainLayout->addWidget(buildGroup);
    
    // Initial Files group
    QGroupBox* filesGroup = new QGroupBox("Initial Files", this);
    QVBoxLayout* filesLayout = new QVBoxLayout(filesGroup);
    
    m_createMainCheck = new QCheckBox("Create main.cpp with Hello, CppAtlas!", this);
    m_createMainCheck->setChecked(true);
    filesLayout->addWidget(m_createMainCheck);
    
    m_createCMakeCheck = new QCheckBox("Create CMakeLists.txt", this);
    m_createCMakeCheck->setChecked(false);
    filesLayout->addWidget(m_createCMakeCheck);
    
    mainLayout->addWidget(filesGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_createBtn = new QPushButton("Create", this);
    m_createBtn->setDefault(true);
    connect(m_createBtn, &QPushButton::clicked, this, &NewProjectDialog::onCreateProject);
    buttonLayout->addWidget(m_createBtn);
    
    m_cancelBtn = new QPushButton("Cancel", this);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(m_cancelBtn);
    
    mainLayout->addLayout(buttonLayout);
}

void NewProjectDialog::loadCompilers() {
    m_compilerCombo->clear();
    
    auto compilers = CompilerRegistry::instance().getAvailableCompilers();
    if (compilers.isEmpty()) {
        compilers = CompilerRegistry::instance().getCompilers();
    }
    
    for (const auto& compiler : compilers) {
        m_compilerCombo->addItem(compiler->name(), compiler->id());
    }
    
    if (m_compilerCombo->count() == 0) {
        m_compilerCombo->addItem("(No compilers found)", "");
    }
}

void NewProjectDialog::onBrowseLocation() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Project Location",
                                                     m_locationEdit->text());
    if (!dir.isEmpty()) {
        m_locationEdit->setText(dir);
    }
}

void NewProjectDialog::onCreateProject() {
    QString name = m_nameEdit->text().trimmed();
    
    // Validate project name
    QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z0-9_]*$");
    if (!nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Invalid Name",
            "Project name must start with a letter and contain only\n"
            "alphanumeric characters and underscores.");
        return;
    }
    
    // Validate location
    QString location = m_locationEdit->text().trimmed();
    QDir locationDir(location);
    if (!locationDir.exists()) {
        QMessageBox::warning(this, "Invalid Location",
            "The selected location does not exist.");
        return;
    }
    
    // Check if project folder already exists
    if (m_createFolderCheck->isChecked()) {
        QDir projectDir(location + QDir::separator() + name);
        if (projectDir.exists()) {
            QMessageBox::warning(this, "Folder Exists",
                "A folder with this project name already exists at the selected location.");
            return;
        }
    }
    
    // Store the expected project file path
    QString projectDir = location;
    if (m_createFolderCheck->isChecked()) {
        projectDir += QDir::separator() + name;
    }
    m_projectFilePath = projectDir + QDir::separator() + name + ".cppatlas";
    
    accept();
}

void NewProjectDialog::validateInput() {
    QString name = m_nameEdit->text().trimmed();
    QString location = m_locationEdit->text().trimmed();
    
    QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z0-9_]*$");
    bool nameValid = nameRegex.match(name).hasMatch();
    bool locationValid = QDir(location).exists();
    
    m_createBtn->setEnabled(nameValid && locationValid && !name.isEmpty());
    
    // Update folder preview
    if (!name.isEmpty() && !location.isEmpty()) {
        QString preview = location;
        if (m_createFolderCheck->isChecked()) {
            preview += QDir::separator() + name;
        }
        preview += QDir::separator() + name + ".cppatlas";
        m_folderPreview->setText(preview);
    } else {
        m_folderPreview->clear();
    }
}

QString NewProjectDialog::projectName() const {
    return m_nameEdit->text().trimmed();
}

QString NewProjectDialog::projectLocation() const {
    return m_locationEdit->text().trimmed();
}

QString NewProjectDialog::projectFilePath() const {
    return m_projectFilePath;
}

bool NewProjectDialog::createProjectFolder() const {
    return m_createFolderCheck->isChecked();
}

bool NewProjectDialog::createMainCpp() const {
    return m_createMainCheck->isChecked();
}

bool NewProjectDialog::createCMakeLists() const {
    return m_createCMakeCheck->isChecked();
}

QString NewProjectDialog::selectedCompilerId() const {
    return m_compilerCombo->currentData().toString();
}

QString NewProjectDialog::selectedStandard() const {
    QString text = m_standardCombo->currentText();
    // Convert "C++17" to "c++17"
    return text.toLower();
}
