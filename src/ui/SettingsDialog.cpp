#include "ui/SettingsDialog.h"
#include "ui/AvatarUtils.h"
#include "core/AppSettings.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(const QString& username, QWidget* parent)
    : QDialog(parent)
    , m_username(username)
{
    setWindowTitle(QStringLiteral("Settings — CppAtlas"));
    setMinimumSize(500, 400);
    // Remove minimize button to prevent hang
    setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QTabWidget* tabs = new QTabWidget(this);
    setupAppearanceTab(tabs);
    setupAccountTab(tabs);
    mainLayout->addWidget(tabs);

    // Bottom close button
    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(btnBox);

    loadCurrentValues();
}

void SettingsDialog::setupAppearanceTab(QTabWidget* tabs)
{
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);

    // Theme
    QGroupBox* themeGroup = new QGroupBox(QStringLiteral("Theme"), page);
    QFormLayout* themeForm = new QFormLayout(themeGroup);
    m_themeCombo = new QComboBox(themeGroup);
    const QStringList themes = ThemeManager::instance()->availableThemes();
    for (const QString& t : themes) {
        m_themeCombo->addItem(t, t);
    }
    themeForm->addRow(QStringLiteral("Color Theme:"), m_themeCombo);
    layout->addWidget(themeGroup);

    // Font
    QGroupBox* fontGroup = new QGroupBox(QStringLiteral("Editor Font"), page);
    QFormLayout* fontForm = new QFormLayout(fontGroup);
    m_fontFamilyCombo = new QFontComboBox(fontGroup);
    m_fontFamilyCombo->setFontFilters(QFontComboBox::MonospacedFonts);
    fontForm->addRow(QStringLiteral("Font Family:"), m_fontFamilyCombo);
    m_fontSizeSpinBox = new QSpinBox(fontGroup);
    m_fontSizeSpinBox->setRange(6, 72);
    m_fontSizeSpinBox->setValue(12);
    fontForm->addRow(QStringLiteral("Font Size:"), m_fontSizeSpinBox);
    layout->addWidget(fontGroup);

    // Editor options
    QGroupBox* optGroup = new QGroupBox(QStringLiteral("Editor Options"), page);
    QVBoxLayout* optLayout = new QVBoxLayout(optGroup);
    m_showLineNumbersChk = new QCheckBox(QStringLiteral("Show Line Numbers"), optGroup);
    m_wordWrapChk        = new QCheckBox(QStringLiteral("Word Wrap"), optGroup);
    optLayout->addWidget(m_showLineNumbersChk);
    optLayout->addWidget(m_wordWrapChk);
    layout->addWidget(optGroup);

    layout->addStretch();

    // Apply / Reset buttons
    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* applyBtn = new QPushButton(QStringLiteral("Apply"), page);
    QPushButton* resetBtn = new QPushButton(QStringLiteral("Reset"), page);
    btnRow->addStretch();
    btnRow->addWidget(resetBtn);
    btnRow->addWidget(applyBtn);
    layout->addLayout(btnRow);

    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApply);
    connect(resetBtn, &QPushButton::clicked, this, &SettingsDialog::onReset);
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::onThemeChanged);

    tabs->addTab(page, QStringLiteral("Appearance"));
}

void SettingsDialog::setupAccountTab(QTabWidget* tabs)
{
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);

    // Avatar section
    QGroupBox* avatarGroup = new QGroupBox(QStringLiteral("Avatar"), page);
    QVBoxLayout* avatarLayout = new QVBoxLayout(avatarGroup);

    // Current avatar preview
    QHBoxLayout* previewRow = new QHBoxLayout();
    m_avatarPreview = new QLabel(avatarGroup);
    m_avatarPreview->setFixedSize(64, 64);
    m_avatarPreview->setAlignment(Qt::AlignCenter);
    m_avatarPreview->setScaledContents(true);
    previewRow->addWidget(m_avatarPreview);
    previewRow->addStretch();
    avatarLayout->addLayout(previewRow);

    // Built-in avatar thumbnails
    QHBoxLayout* builtinRow = new QHBoxLayout();
    QLabel* builtinLabel = new QLabel(QStringLiteral("Built-in:"), avatarGroup);
    builtinRow->addWidget(builtinLabel);

    const QStringList builtinPaths = {
        QStringLiteral(":/avatars/avatar_blue.svg"),
        QStringLiteral(":/avatars/avatar_green.svg"),
        QStringLiteral(":/avatars/avatar_red.svg"),
        QStringLiteral(":/avatars/avatar_purple.svg"),
        QStringLiteral(":/avatars/avatar_orange.svg"),
        QStringLiteral(":/avatars/avatar_teal.svg"),
    };
    for (int i = 0; i < builtinPaths.size(); ++i) {
        QPushButton* btn = new QPushButton(avatarGroup);
        btn->setFixedSize(32, 32);
        btn->setFlat(true);
        // Render avatar as thumbnail for button icon
        const QPixmap px = loadAvatarPixmap(builtinPaths[i], 28);
        if (!px.isNull()) {
            btn->setIcon(QIcon(px));
            btn->setIconSize(QSize(28, 28));
        }
        const QString path = builtinPaths[i];
        connect(btn, &QPushButton::clicked, this, [this, path]() {
            m_currentAvatarPath = path;
            updateAvatarPreview(path);
        });
        builtinRow->addWidget(btn);
    }
    builtinRow->addStretch();
    avatarLayout->addLayout(builtinRow);

    // Upload custom image
    QPushButton* uploadBtn = new QPushButton(QStringLiteral("Upload Custom Image..."), avatarGroup);
    connect(uploadBtn, &QPushButton::clicked, this, &SettingsDialog::onAvatarUploadClicked);
    avatarLayout->addWidget(uploadBtn);

    // Save avatar button
    QPushButton* saveAvatarBtn = new QPushButton(QStringLiteral("Save Avatar"), avatarGroup);
    connect(saveAvatarBtn, &QPushButton::clicked, this, [this]() {
        if (!m_currentAvatarPath.isEmpty()) {
            UserManager::instance().updateAvatarPath(m_username, m_currentAvatarPath);
            emit settingsChanged();
        }
    });
    avatarLayout->addWidget(saveAvatarBtn);
    layout->addWidget(avatarGroup);

    // Display name
    QGroupBox* nameGroup = new QGroupBox(QStringLiteral("Display Name"), page);
    QHBoxLayout* nameRow = new QHBoxLayout(nameGroup);
    m_displayNameEdit = new QLineEdit(nameGroup);
    QPushButton* saveNameBtn = new QPushButton(QStringLiteral("Save"), nameGroup);
    nameRow->addWidget(m_displayNameEdit);
    nameRow->addWidget(saveNameBtn);
    connect(saveNameBtn, &QPushButton::clicked, this, &SettingsDialog::onDisplayNameSave);
    layout->addWidget(nameGroup);

    // Change password
    QGroupBox* pwGroup = new QGroupBox(QStringLiteral("Change Password"), page);
    QFormLayout* pwForm = new QFormLayout(pwGroup);
    m_oldPasswordEdit  = new QLineEdit(pwGroup);
    m_newPasswordEdit  = new QLineEdit(pwGroup);
    m_confirmPassEdit  = new QLineEdit(pwGroup);
    m_oldPasswordEdit->setEchoMode(QLineEdit::Password);
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPassEdit->setEchoMode(QLineEdit::Password);
    pwForm->addRow(QStringLiteral("Current Password:"), m_oldPasswordEdit);
    pwForm->addRow(QStringLiteral("New Password:"),     m_newPasswordEdit);
    pwForm->addRow(QStringLiteral("Confirm Password:"), m_confirmPassEdit);
    QPushButton* changePwBtn = new QPushButton(QStringLiteral("Change Password"), pwGroup);
    connect(changePwBtn, &QPushButton::clicked, this, &SettingsDialog::onPasswordChange);
    pwForm->addRow(QString(), changePwBtn);
    layout->addWidget(pwGroup);

    layout->addStretch();

    tabs->addTab(page, QStringLiteral("Account"));
}

void SettingsDialog::loadCurrentValues()
{
    AppSettings settings(m_username);

    // Theme
    const QString savedTheme = settings.theme();
    m_initialTheme = savedTheme;
    const int themeIdx = m_themeCombo->findData(savedTheme);
    if (themeIdx >= 0) m_themeCombo->setCurrentIndex(themeIdx);

    // Font
    m_initialFontFamily = settings.editorFontFamily();
    m_initialFontSize   = settings.editorFontSize();
    m_fontFamilyCombo->setCurrentFont(QFont(m_initialFontFamily));
    m_fontSizeSpinBox->setValue(m_initialFontSize);

    // Options
    m_initialShowLineNumbers = settings.showLineNumbers();
    m_initialWordWrap        = settings.wordWrap();
    m_showLineNumbersChk->setChecked(m_initialShowLineNumbers);
    m_wordWrapChk->setChecked(m_initialWordWrap);

    // Account — display name
    const UserRecord user = UserManager::instance().currentUser();
    if (m_displayNameEdit) {
        m_displayNameEdit->setText(user.displayName);
    }

    // Avatar
    m_currentAvatarPath = user.avatarPath;
    updateAvatarPreview(m_currentAvatarPath);
}

void SettingsDialog::applyAppearanceSettings()
{
    AppSettings settings(m_username);

    // Theme
    const QString theme = m_themeCombo->currentData().toString();
    settings.setTheme(theme);
    ThemeManager::instance()->setTheme(theme);

    // Font
    settings.setEditorFontFamily(m_fontFamilyCombo->currentFont().family());
    settings.setEditorFontSize(m_fontSizeSpinBox->value());

    // Options
    settings.setShowLineNumbers(m_showLineNumbersChk->isChecked());
    settings.setWordWrap(m_wordWrapChk->isChecked());

    emit settingsChanged();
}

void SettingsDialog::onApply()
{
    applyAppearanceSettings();
    // Update initial values so Reset reflects what was just applied
    m_initialTheme          = m_themeCombo->currentData().toString();
    m_initialFontFamily     = m_fontFamilyCombo->currentFont().family();
    m_initialFontSize       = m_fontSizeSpinBox->value();
    m_initialShowLineNumbers = m_showLineNumbersChk->isChecked();
    m_initialWordWrap        = m_wordWrapChk->isChecked();
}

void SettingsDialog::onReset()
{
    // Revert to values that existed when dialog was opened (or last Apply)
    const int themeIdx = m_themeCombo->findData(m_initialTheme);
    if (themeIdx >= 0) m_themeCombo->setCurrentIndex(themeIdx);
    m_fontFamilyCombo->setCurrentFont(QFont(m_initialFontFamily));
    m_fontSizeSpinBox->setValue(m_initialFontSize);
    m_showLineNumbersChk->setChecked(m_initialShowLineNumbers);
    m_wordWrapChk->setChecked(m_initialWordWrap);
}

void SettingsDialog::onThemeChanged(int index)
{
    Q_UNUSED(index)
    // Live preview
    const QString theme = m_themeCombo->currentData().toString();
    ThemeManager::instance()->setTheme(theme);
}

void SettingsDialog::onAvatarUploadClicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Select Avatar Image"),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        QStringLiteral("Images (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty()) return;

    // Copy to app data directory
    const QString destDir = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation) + QStringLiteral("/avatars");
    QDir().mkpath(destDir);
    const QString destPath = destDir + QStringLiteral("/") + m_username + QStringLiteral(".png");

    if (QFile::exists(destPath)) QFile::remove(destPath);

    if (QFile::copy(filePath, destPath)) {
        m_currentAvatarPath = destPath;
        updateAvatarPreview(destPath);
    } else {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("Failed to copy avatar image."));
    }
}

void SettingsDialog::onDisplayNameSave()
{
    if (!m_displayNameEdit) return;
    const QString newName = m_displayNameEdit->text().trimmed();
    if (newName.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("Display name cannot be empty."));
        return;
    }
    if (UserManager::instance().updateDisplayName(newName)) {
        QMessageBox::information(this, QStringLiteral("Success"),
            QStringLiteral("Display name updated."));
        emit settingsChanged();
    } else {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("Failed to update display name."));
    }
}

void SettingsDialog::onPasswordChange()
{
    const QString oldPw = m_oldPasswordEdit ? m_oldPasswordEdit->text() : QString();
    const QString newPw = m_newPasswordEdit ? m_newPasswordEdit->text() : QString();
    const QString confPw = m_confirmPassEdit ? m_confirmPassEdit->text() : QString();

    if (newPw != confPw) {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("New passwords do not match."));
        return;
    }
    if (newPw.length() < 4) {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("New password must be at least 4 characters."));
        return;
    }
    if (UserManager::instance().changePassword(m_username, oldPw, newPw)) {
        QMessageBox::information(this, QStringLiteral("Success"),
            QStringLiteral("Password changed successfully."));
        if (m_oldPasswordEdit)  m_oldPasswordEdit->clear();
        if (m_newPasswordEdit)  m_newPasswordEdit->clear();
        if (m_confirmPassEdit)  m_confirmPassEdit->clear();
    } else {
        QMessageBox::warning(this, QStringLiteral("Error"),
            QStringLiteral("Failed to change password. Check your current password."));
    }
}

void SettingsDialog::updateAvatarPreview(const QString& path)
{
    if (!m_avatarPreview) return;

    if (path.isEmpty()) {
        m_avatarPreview->setPixmap(QPixmap());
        m_avatarPreview->setText(QStringLiteral("C++"));
        return;
    }

    const QPixmap px = loadAvatarPixmap(path, 64);
    if (!px.isNull()) {
        m_avatarPreview->setPixmap(px);
        m_avatarPreview->setText(QString());
    } else {
        m_avatarPreview->setPixmap(QPixmap());
        m_avatarPreview->setText(QStringLiteral("C++"));
    }
}
