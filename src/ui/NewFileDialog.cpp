#include "ui/NewFileDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QRandomGenerator>

NewFileDialog::NewFileDialog(const QString& defaultDirectory, QWidget *parent)
    : QDialog(parent)
    , m_defaultDirectory(defaultDirectory)
{
    setWindowTitle("New File");
    setMinimumSize(600, 400);
    setupUI();
    populateTemplates();

    // Select first template by default
    if (m_templateList->count() > 0) {
        m_templateList->setCurrentRow(0);
    }
}

void NewFileDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Top area: template list (left) + description (right)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    m_templateList = new QListWidget(this);
    m_templateList->setMinimumWidth(200);
    splitter->addWidget(m_templateList);

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_descriptionLabel->setMargin(10);
    m_descriptionLabel->setMinimumWidth(200);
    splitter->addWidget(m_descriptionLabel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    mainLayout->addWidget(splitter, 1);

    // Name field
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:", this));
    m_nameEdit = new QLineEdit("Source", this);
    nameLayout->addWidget(m_nameEdit);
    mainLayout->addLayout(nameLayout);

    // Location field
    QHBoxLayout* locationLayout = new QHBoxLayout();
    locationLayout->addWidget(new QLabel("Location:", this));
    m_locationEdit = new QLineEdit(m_defaultDirectory, this);
    m_locationEdit->setReadOnly(true);
    locationLayout->addWidget(m_locationEdit);
    m_browseBtn = new QPushButton("Browse...", this);
    locationLayout->addWidget(m_browseBtn);
    mainLayout->addLayout(locationLayout);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_createBtn = new QPushButton("Create", this);
    m_createBtn->setDefault(true);
    m_cancelBtn = new QPushButton("Cancel", this);
    buttonLayout->addWidget(m_createBtn);
    buttonLayout->addWidget(m_cancelBtn);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(m_templateList, &QListWidget::currentRowChanged,
            this, &NewFileDialog::onTemplateSelectionChanged);
    connect(m_browseBtn, &QPushButton::clicked,
            this, &NewFileDialog::onBrowseClicked);
    connect(m_createBtn, &QPushButton::clicked,
            this, &NewFileDialog::onCreateClicked);
    connect(m_cancelBtn, &QPushButton::clicked,
            this, &QDialog::reject);
}

void NewFileDialog::populateTemplates() {
    auto addItem = [this](const QString& text, FileTemplate type) {
        QListWidgetItem* item = new QListWidgetItem(text, m_templateList);
        item->setData(Qt::UserRole, type);
    };

    addItem("C++ Source File (.cpp)", CppSource);
    addItem("C++ Header File (.hpp)", CppHeader);
    addItem("C Header File (.h)", CHeader);
    addItem("C++ Class (.cpp + .hpp)", CppClass);
    addItem("C Source File (.c)", CSource);
    addItem("Empty File", EmptyFile);
}

void NewFileDialog::onTemplateSelectionChanged() {
    QListWidgetItem* item = m_templateList->currentItem();
    if (!item) return;

    FileTemplate type = static_cast<FileTemplate>(item->data(Qt::UserRole).toInt());

    switch (type) {
    case CppSource:
        m_descriptionLabel->setText("Creates a C++ source file with a basic main() function.");
        m_nameEdit->setText("Source");
        break;
    case CppHeader:
        m_descriptionLabel->setText("Creates a C++ header file with include guards.");
        m_nameEdit->setText("Header");
        break;
    case CHeader:
        m_descriptionLabel->setText("Creates a C header file with include guards.");
        m_nameEdit->setText("Header");
        break;
    case CppClass:
        m_descriptionLabel->setText(
            "Creates a C++ class with both header (.hpp) and source (.cpp) files.");
        m_nameEdit->setText("MyClass");
        break;
    case CSource:
        m_descriptionLabel->setText("Creates a C source file with a basic main() function.");
        m_nameEdit->setText("Source");
        break;
    case EmptyFile:
        m_descriptionLabel->setText("Creates an empty file with no content.");
        m_nameEdit->setText("Untitled");
        break;
    }
}

void NewFileDialog::onBrowseClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Location",
                                                     m_locationEdit->text());
    if (!dir.isEmpty()) {
        m_locationEdit->setText(dir);
    }
}

void NewFileDialog::onCreateClicked() {
    QListWidgetItem* item = m_templateList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "New File", "Please select a file template.");
        return;
    }

    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "New File", "Please enter a file name.");
        return;
    }

    QString location = m_locationEdit->text();
    if (location.isEmpty()) {
        QMessageBox::warning(this, "New File", "Please select a location.");
        return;
    }

    FileTemplate type = static_cast<FileTemplate>(item->data(Qt::UserRole).toInt());
    m_createdFiles.clear();

    switch (type) {
    case CppSource: {
        QString filePath = QDir(location).filePath(name + ".cpp");
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + filePath);
            return;
        }
        QString tmpl = loadTemplate(":/templates/source.cpp.template");
        if (!createFileFromTemplate(filePath, tmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create file.");
            return;
        }
        m_createdFiles << filePath;
        break;
    }
    case CppHeader: {
        QString filePath = QDir(location).filePath(name + ".hpp");
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + filePath);
            return;
        }
        QString tmpl = loadTemplate(":/templates/header.hpp.template");
        QString guard = generateHeaderGuard(name + ".hpp");
        tmpl.replace("${HEADER_GUARD}", guard);
        if (!createFileFromTemplate(filePath, tmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create file.");
            return;
        }
        m_createdFiles << filePath;
        break;
    }
    case CHeader: {
        QString filePath = QDir(location).filePath(name + ".h");
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + filePath);
            return;
        }
        QString tmpl = loadTemplate(":/templates/header.hpp.template");
        QString guard = generateHeaderGuard(name + ".h");
        tmpl.replace("${HEADER_GUARD}", guard);
        if (!createFileFromTemplate(filePath, tmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create file.");
            return;
        }
        m_createdFiles << filePath;
        break;
    }
    case CppClass: {
        QString hppPath = QDir(location).filePath(name + ".hpp");
        QString cppPath = QDir(location).filePath(name + ".cpp");
        if (QFile::exists(hppPath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + hppPath);
            return;
        }
        if (QFile::exists(cppPath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + cppPath);
            return;
        }
        // Header
        QString hppTmpl = loadTemplate(":/templates/class.hpp.template");
        QString guard = generateHeaderGuard(name + ".hpp");
        hppTmpl.replace("${HEADER_GUARD}", guard);
        hppTmpl.replace("${CLASS_NAME}", name);
        if (!createFileFromTemplate(hppPath, hppTmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create header file.");
            return;
        }
        m_createdFiles << hppPath;
        // Source
        QString cppTmpl = loadTemplate(":/templates/class.cpp.template");
        cppTmpl.replace("${CLASS_NAME}", name);
        if (!createFileFromTemplate(cppPath, cppTmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create source file.");
            return;
        }
        m_createdFiles << cppPath;
        break;
    }
    case CSource: {
        QString filePath = QDir(location).filePath(name + ".c");
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + filePath);
            return;
        }
        QString tmpl = loadTemplate(":/templates/source.cpp.template");
        if (!createFileFromTemplate(filePath, tmpl)) {
            QMessageBox::warning(this, "New File", "Failed to create file.");
            return;
        }
        m_createdFiles << filePath;
        break;
    }
    case EmptyFile: {
        QString filePath = QDir(location).filePath(name);
        if (QFile::exists(filePath)) {
            QMessageBox::warning(this, "New File", "File already exists: " + filePath);
            return;
        }
        if (!createFileFromTemplate(filePath, QString())) {
            QMessageBox::warning(this, "New File", "Failed to create file.");
            return;
        }
        m_createdFiles << filePath;
        break;
    }
    }

    accept();
}

QStringList NewFileDialog::createdFiles() const {
    return m_createdFiles;
}

QString NewFileDialog::loadTemplate(const QString& resourcePath) const {
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream stream(&file);
    return stream.readAll();
}

QString NewFileDialog::generateHeaderGuard(const QString& fileName) const {
    QString guard = fileName.toUpper();
    guard.replace('.', '_');
    guard.replace('-', '_');
    guard.replace(' ', '_');
    // Add random suffix
    quint32 suffix = QRandomGenerator::global()->generate() % 100000;
    guard += QString("_%1").arg(suffix);
    return guard;
}

bool NewFileDialog::createFileFromTemplate(const QString& filePath,
                                            const QString& templateContent,
                                            const QString& className) const {
    Q_UNUSED(className);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream stream(&file);
    stream << templateContent;
    file.close();
    return true;
}
