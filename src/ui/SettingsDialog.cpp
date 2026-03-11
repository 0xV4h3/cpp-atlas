#include "ui/SettingsDialog.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QColorDialog>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QPainterPath>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

SettingsDialog::SettingsDialog(const QString& username, QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
    , m_settings(username)
    , m_username(username)
{
    setWindowTitle("CppAtlas — Settings");
    setModal(false);  // Non-modal so it doesn't block main window
    setMinimumSize(480, 420);

    setupUi();
    applyTheme();

    // Snapshot for Reset
    m_initialTheme      = m_settings.theme();
    m_initialFontFamily = m_settings.editorFontFamily();
    m_initialFontSize   = m_settings.editorFontSize();
    m_initialLineNums   = m_settings.showLineNumbers();
    m_initialWordWrap   = m_settings.wordWrap();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &SettingsDialog::applyTheme);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────

void SettingsDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    m_tabWidget = new QTabWidget(this);

    QWidget* appearanceTab = new QWidget(m_tabWidget);
    QWidget* accountTab    = new QWidget(m_tabWidget);

    setupAppearanceTab(appearanceTab);
    setupAccountTab(accountTab);

    m_tabWidget->addTab(appearanceTab, "Appearance");
    m_tabWidget->addTab(accountTab,    "Account");

    mainLayout->addWidget(m_tabWidget, 1);

    // Bottom button row: Reset, Apply, Close
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* resetBtn = new QPushButton("Reset", this);
    resetBtn->setObjectName("settingsResetBtn");
    connect(resetBtn, &QPushButton::clicked, this, &SettingsDialog::onResetAppearance);
    btnLayout->addWidget(resetBtn);

    QPushButton* applyBtn = new QPushButton("Apply", this);
    applyBtn->setObjectName("settingsApplyBtn");
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApplyAppearance);
    btnLayout->addWidget(applyBtn);

    QPushButton* closeBtn = new QPushButton("Close", this);
    closeBtn->setObjectName("settingsCloseBtn");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(closeBtn);

    mainLayout->addLayout(btnLayout);
}

void SettingsDialog::setupAppearanceTab(QWidget* tab)
{
    QFormLayout* form = new QFormLayout(tab);
    form->setContentsMargins(16, 16, 16, 16);
    form->setSpacing(12);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Theme
    m_themeCombo = new QComboBox(tab);
    const QStringList themes = ThemeManager::instance()->availableThemes();
    const QStringList sortedThemes = [&]() {
        QStringList t = themes;
        std::sort(t.begin(), t.end());
        return t;
    }();
    m_themeCombo->addItems(sortedThemes);
    const QString currentTheme = m_settings.theme();
    const int themeIdx = m_themeCombo->findText(currentTheme);
    if (themeIdx >= 0) m_themeCombo->setCurrentIndex(themeIdx);
    connect(m_themeCombo, &QComboBox::currentTextChanged,
            this, &SettingsDialog::onThemeChanged);
    form->addRow("Theme:", m_themeCombo);

    // Font family (monospace)
    m_fontFamilyCombo = new QFontComboBox(tab);
    m_fontFamilyCombo->setFontFilters(QFontComboBox::MonospacedFonts);
    m_fontFamilyCombo->setCurrentFont(QFont(m_settings.editorFontFamily()));
    form->addRow("Editor Font:", m_fontFamilyCombo);

    // Font size
    m_fontSizeSpinBox = new QSpinBox(tab);
    m_fontSizeSpinBox->setRange(6, 32);
    m_fontSizeSpinBox->setValue(m_settings.editorFontSize());
    form->addRow("Font Size:", m_fontSizeSpinBox);

    // Line numbers
    m_lineNumbersCheck = new QCheckBox(tab);
    m_lineNumbersCheck->setChecked(m_settings.showLineNumbers());
    form->addRow("Show Line Numbers:", m_lineNumbersCheck);

    // Word wrap
    m_wordWrapCheck = new QCheckBox(tab);
    m_wordWrapCheck->setChecked(m_settings.wordWrap());
    form->addRow("Word Wrap:", m_wordWrapCheck);
}

void SettingsDialog::setupAccountTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    const UserRecord& user = UserManager::instance().currentUser();

    // User info (read-only)
    QFormLayout* infoForm = new QFormLayout();
    infoForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    infoForm->setSpacing(8);

    QLabel* usernameLabel = new QLabel(user.username, tab);
    usernameLabel->setObjectName("settingsReadOnly");
    infoForm->addRow("Username:", usernameLabel);

    m_displayNameLabel = new QLabel(user.displayName, tab);
    m_displayNameLabel->setObjectName("settingsReadOnly");
    infoForm->addRow("Display Name:", m_displayNameLabel);

    layout->addLayout(infoForm);

    // Change display name
    QFrame* nameSep = new QFrame(tab);
    nameSep->setFrameShape(QFrame::HLine);
    nameSep->setObjectName("settingsSeparator");
    layout->addWidget(nameSep);

    QLabel* nameLabel = new QLabel("Change Display Name", tab);
    nameLabel->setObjectName("settingsSectionLabel");
    layout->addWidget(nameLabel);

    QHBoxLayout* nameRow = new QHBoxLayout();
    m_displayNameEdit = new QLineEdit(tab);
    m_displayNameEdit->setObjectName("settingsInput");
    m_displayNameEdit->setPlaceholderText("New display name...");
    nameRow->addWidget(m_displayNameEdit);
    QPushButton* applyNameBtn = new QPushButton("Apply", tab);
    applyNameBtn->setObjectName("settingsApplyBtn");
    connect(applyNameBtn, &QPushButton::clicked,
            this, &SettingsDialog::onApplyDisplayName);
    nameRow->addWidget(applyNameBtn);
    layout->addLayout(nameRow);

    // Avatar section
    QFrame* avatarSep = new QFrame(tab);
    avatarSep->setFrameShape(QFrame::HLine);
    avatarSep->setObjectName("settingsSeparator");
    layout->addWidget(avatarSep);

    QLabel* avatarLabel = new QLabel("Avatar", tab);
    avatarLabel->setObjectName("settingsSectionLabel");
    layout->addWidget(avatarLabel);

    // Preview
    m_avatarPreviewLabel = new QLabel(tab);
    m_avatarPreviewLabel->setObjectName("avatarPreview");
    m_avatarPreviewLabel->setFixedSize(64, 64);
    m_avatarPreviewLabel->setAlignment(Qt::AlignCenter);
    updateAvatarPreview();
    layout->addWidget(m_avatarPreviewLabel, 0, Qt::AlignHCenter);

    // Buttons row
    QHBoxLayout* avatarBtnRow = new QHBoxLayout();

    m_uploadAvatarBtn = new QPushButton("Upload image...", tab);
    m_uploadAvatarBtn->setObjectName("settingsApplyBtn");
    connect(m_uploadAvatarBtn, &QPushButton::clicked, this, &SettingsDialog::onUploadAvatar);
    avatarBtnRow->addWidget(m_uploadAvatarBtn);

    m_removeAvatarBtn = new QPushButton("Remove avatar", tab);
    m_removeAvatarBtn->setObjectName("settingsResetBtn");
    // Only enabled when a custom image avatar is set; avatar color is always available as fallback
    m_removeAvatarBtn->setEnabled(!user.avatarPath.isEmpty());
    connect(m_removeAvatarBtn, &QPushButton::clicked, this, &SettingsDialog::onRemoveAvatar);
    avatarBtnRow->addWidget(m_removeAvatarBtn);

    m_avatarColorBtn = new QPushButton(user.avatarColor, tab);
    m_avatarColorBtn->setObjectName("settingsAvatarColorBtn");
    m_avatarColorBtn->setFixedSize(120, 32);
    m_avatarColorBtn->setStyleSheet(
        QString("background-color: %1; color: white; border-radius: 4px;")
            .arg(user.avatarColor));
    m_avatarColorBtn->setVisible(user.avatarPath.isEmpty());
    connect(m_avatarColorBtn, &QPushButton::clicked, this, &SettingsDialog::onChangeAvatarColor);
    avatarBtnRow->addWidget(m_avatarColorBtn);

    layout->addLayout(avatarBtnRow);

    // Change password
    QFrame* passSep = new QFrame(tab);
    passSep->setFrameShape(QFrame::HLine);
    passSep->setObjectName("settingsSeparator");
    layout->addWidget(passSep);

    QLabel* passLabel = new QLabel("Change Password", tab);
    passLabel->setObjectName("settingsSectionLabel");
    layout->addWidget(passLabel);

    QFormLayout* passForm = new QFormLayout();
    passForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    passForm->setSpacing(8);

    m_currentPassEdit = new QLineEdit(tab);
    m_currentPassEdit->setObjectName("settingsInput");
    m_currentPassEdit->setEchoMode(QLineEdit::Password);
    passForm->addRow("Current Password:", m_currentPassEdit);

    m_newPassEdit = new QLineEdit(tab);
    m_newPassEdit->setObjectName("settingsInput");
    m_newPassEdit->setEchoMode(QLineEdit::Password);
    passForm->addRow("New Password:", m_newPassEdit);

    m_confirmPassEdit = new QLineEdit(tab);
    m_confirmPassEdit->setObjectName("settingsInput");
    m_confirmPassEdit->setEchoMode(QLineEdit::Password);
    passForm->addRow("Confirm Password:", m_confirmPassEdit);

    layout->addLayout(passForm);

    m_passwordStatusLabel = new QLabel(tab);
    m_passwordStatusLabel->setObjectName("settingsStatusLabel");
    m_passwordStatusLabel->setVisible(false);
    layout->addWidget(m_passwordStatusLabel);

    QPushButton* changePassBtn = new QPushButton("Change Password", tab);
    changePassBtn->setObjectName("settingsApplyBtn");
    connect(changePassBtn, &QPushButton::clicked,
            this, &SettingsDialog::onChangePassword);
    layout->addWidget(changePassBtn, 0, Qt::AlignLeft);

    layout->addStretch();
}

// ─────────────────────────────────────────────────────────────────────────────
// Avatar helpers
// ─────────────────────────────────────────────────────────────────────────────

void SettingsDialog::updateAvatarPreview()
{
    if (!m_avatarPreviewLabel) return;
    const UserRecord& user = UserManager::instance().currentUser();

    if (!user.avatarPath.isEmpty()) {
        QPixmap pix(user.avatarPath);
        if (!pix.isNull()) {
            // Circular crop
            QPixmap circle(64, 64);
            circle.fill(Qt::transparent);
            QPainter painter(&circle);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, 64, 64);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, 64, 64, pix.scaled(64, 64,
                Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            m_avatarPreviewLabel->setPixmap(circle);
            return;
        }
    }

    // Fallback: colored circle with initial
    QPixmap circle(64, 64);
    circle.fill(Qt::transparent);
    QPainter painter(&circle);
    painter.setRenderHint(QPainter::Antialiasing);
    const QColor color(user.avatarColor.isEmpty() ? "#007ACC" : user.avatarColor);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 64, 64);
    painter.setPen(Qt::white);
    QFont f = painter.font();
    f.setPointSize(24);
    f.setBold(true);
    painter.setFont(f);
    const QString initial = user.displayName.isEmpty()
        ? user.username.left(1).toUpper()
        : user.displayName.left(1).toUpper();
    painter.drawText(QRect(0, 0, 64, 64), Qt::AlignCenter, initial);
    m_avatarPreviewLabel->setPixmap(circle);
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void SettingsDialog::onThemeChanged(const QString& themeName)
{
    ThemeManager::instance()->setTheme(themeName);
    m_settings.setTheme(themeName);
}

void SettingsDialog::onApplyAppearance()
{
    // Theme (already applied live via onThemeChanged, just save)
    m_settings.setTheme(m_themeCombo->currentText());

    // Font
    m_settings.setEditorFontFamily(m_fontFamilyCombo->currentFont().family());
    m_settings.setEditorFontSize(m_fontSizeSpinBox->value());

    // Editor options
    m_settings.setShowLineNumbers(m_lineNumbersCheck->isChecked());
    m_settings.setWordWrap(m_wordWrapCheck->isChecked());

    // Update snapshot (so next Reset goes to currently applied values)
    m_initialTheme      = m_themeCombo->currentText();
    m_initialFontFamily = m_fontFamilyCombo->currentFont().family();
    m_initialFontSize   = m_fontSizeSpinBox->value();
    m_initialLineNums   = m_lineNumbersCheck->isChecked();
    m_initialWordWrap   = m_wordWrapCheck->isChecked();

    emit settingsChanged();
}

void SettingsDialog::onResetAppearance()
{
    // Restore to snapshot (values when dialog was opened or last applied)
    const int themeIdx = m_themeCombo->findText(m_initialTheme);
    if (themeIdx >= 0) m_themeCombo->setCurrentIndex(themeIdx);
    // onThemeChanged fires automatically and reverts the theme live

    m_fontFamilyCombo->setCurrentFont(QFont(m_initialFontFamily));
    m_fontSizeSpinBox->setValue(m_initialFontSize);
    m_lineNumbersCheck->setChecked(m_initialLineNums);
    m_wordWrapCheck->setChecked(m_initialWordWrap);

    // Notify listeners so editors update to the reverted values
    emit settingsChanged();
}

void SettingsDialog::onApplyDisplayName()
{
    const QString newName = m_displayNameEdit->text().trimmed();
    if (newName.isEmpty()) return;

    if (UserManager::instance().updateDisplayName(newName)) {
        m_settings.setValue("display_name", newName);
        m_displayNameLabel->setText(newName);
        m_displayNameEdit->clear();
    }
}

void SettingsDialog::onChangeAvatarColor()
{
    const UserRecord& user = UserManager::instance().currentUser();
    const QColor initial(user.avatarColor);
    const QColor chosen = QColorDialog::getColor(initial, this, "Choose Avatar Color");
    if (!chosen.isValid()) return;

    const QString hex = chosen.name();
    if (UserManager::instance().updateAvatarColor(hex)) {
        m_avatarColorBtn->setText(hex);
        m_avatarColorBtn->setStyleSheet(
            QString("background-color: %1; color: white; border-radius: 4px;").arg(hex));
        updateAvatarPreview();
    }
}

void SettingsDialog::onUploadAvatar()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this, "Select Avatar Image", QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (filePath.isEmpty()) return;

    // Copy to app data location
    const QString destDir = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation) + "/avatars/";
    QDir().mkpath(destDir);
    const QString ext = QFileInfo(filePath).suffix();
    const QString destPath = destDir + m_username + "." + ext;
    QFile::remove(destPath);  // overwrite if exists
    if (!QFile::copy(filePath, destPath)) {
        QMessageBox::warning(this, "Upload Failed",
            "Could not copy the image file.");
        return;
    }

    UserManager::instance().updateAvatarPath(m_username, destPath);
    updateAvatarPreview();
    if (m_removeAvatarBtn) m_removeAvatarBtn->setEnabled(true);
    if (m_avatarColorBtn)  m_avatarColorBtn->setVisible(false);
}

void SettingsDialog::onRemoveAvatar()
{
    UserManager::instance().updateAvatarPath(m_username, QString());
    updateAvatarPreview();
    if (m_removeAvatarBtn) m_removeAvatarBtn->setEnabled(false);
    if (m_avatarColorBtn)  m_avatarColorBtn->setVisible(true);
}

void SettingsDialog::onChangePassword()
{
    const QString current = m_currentPassEdit->text();
    const QString newPwd  = m_newPassEdit->text();
    const QString confirm = m_confirmPassEdit->text();

    m_passwordStatusLabel->setVisible(false);

    if (current.isEmpty() || newPwd.isEmpty() || confirm.isEmpty()) {
        m_passwordStatusLabel->setText("All password fields are required.");
        m_passwordStatusLabel->setProperty("status", "error");
        m_passwordStatusLabel->setVisible(true);
        return;
    }
    if (newPwd != confirm) {
        m_passwordStatusLabel->setText("New passwords do not match.");
        m_passwordStatusLabel->setProperty("status", "error");
        m_passwordStatusLabel->setVisible(true);
        return;
    }
    if (UserManager::instance().changePassword(m_username, current, newPwd)) {
        m_passwordStatusLabel->setText("Password changed successfully.");
        m_passwordStatusLabel->setProperty("status", "success");
        m_passwordStatusLabel->setVisible(true);
        m_currentPassEdit->clear();
        m_newPassEdit->clear();
        m_confirmPassEdit->clear();
    } else {
        m_passwordStatusLabel->setText("Incorrect current password.");
        m_passwordStatusLabel->setProperty("status", "error");
        m_passwordStatusLabel->setVisible(true);
    }
    // Re-apply style to pick up property change
    m_passwordStatusLabel->style()->unpolish(m_passwordStatusLabel);
    m_passwordStatusLabel->style()->polish(m_passwordStatusLabel);
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────

void SettingsDialog::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();

    setStyleSheet(QString(R"(
        SettingsDialog {
            background-color: %1;
        }
        QTabWidget::pane {
            background-color: %2;
            border: 1px solid %3;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: %4;
            color: %5;
            padding: 6px 18px;
            border: 1px solid %3;
            border-bottom: none;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: %2;
            color: %6;
        }
        QTabBar::tab:hover:!selected {
            background-color: %3;
        }
        QLabel {
            color: %5;
        }
        #settingsSectionLabel {
            color: %6;
            font-size: 13px;
            font-weight: bold;
        }
        #settingsReadOnly {
            color: %5;
            font-size: 13px;
        }
        QComboBox, QFontComboBox, QSpinBox, QLineEdit {
            background-color: %7;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 4px 8px;
            font-size: 13px;
        }
        QComboBox:focus, QFontComboBox:focus, QSpinBox:focus, QLineEdit:focus {
            border: 1px solid %6;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background-color: %7;
            color: %5;
            selection-background-color: %6;
        }
        QCheckBox {
            color: %5;
            font-size: 13px;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid %3;
            border-radius: 3px;
            background-color: %7;
        }
        QCheckBox::indicator:checked {
            background-color: %6;
            border-color: %6;
        }
        #settingsApplyBtn, #settingsCloseBtn {
            background-color: %6;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
        }
        #settingsApplyBtn:hover, #settingsCloseBtn:hover {
            background-color: %8;
        }
        #settingsResetBtn {
            background-color: %7;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
        }
        #settingsResetBtn:hover {
            background-color: %3;
        }
        #settingsSeparator {
            color: %3;
        }
        #settingsStatusLabel[status="error"] {
            color: %9;
            font-size: 12px;
        }
        #settingsStatusLabel[status="success"] {
            color: %10;
            font-size: 12px;
        }
    )")
    .arg(t.windowBackground.name())   // %1
    .arg(t.panelBackground.name())    // %2
    .arg(t.border.name())             // %3
    .arg(t.tabInactive.name())        // %4
    .arg(t.textPrimary.name())        // %5
    .arg(t.accent.name())             // %6
    .arg(t.sidebarBackground.name())  // %7
    .arg(t.accent.darker(115).name()) // %8
    .arg(t.error.name())              // %9
    .arg(t.success.name())            // %10
    );
}