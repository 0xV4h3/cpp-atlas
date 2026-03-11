#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QComboBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "core/AppSettings.h"

/**
 * @brief Tabbed settings dialog for CppAtlas.
 *
 * Tabs:
 *  - Appearance: theme, editor font family/size, line numbers, word wrap
 *  - Account:    display name, avatar color, change password
 *  - About:      app info and version
 *
 * Emits settingsChanged() when font/theme settings are modified so that
 * MainWindow can react (e.g., update window title, re-apply fonts).
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QString& username, QWidget* parent = nullptr);
    ~SettingsDialog() = default;

signals:
    /** Emitted when appearance settings (font, theme) change. */
    void settingsChanged();

private slots:
    void applyTheme();
    void onThemeChanged(const QString& themeName);
    void onApplyDisplayName();
    void onChangeAvatarColor();
    void onChangePassword();

private:
    void setupUi();
    void setupAppearanceTab(QWidget* tab);
    void setupAccountTab(QWidget* tab);
    void setupAboutTab(QWidget* tab);

    // ── Appearance tab widgets ────────────────────────────────────────────
    QComboBox*     m_themeCombo         = nullptr;
    QFontComboBox* m_fontFamilyCombo    = nullptr;
    QSpinBox*      m_fontSizeSpinBox    = nullptr;
    QCheckBox*     m_lineNumbersCheck   = nullptr;
    QCheckBox*     m_wordWrapCheck      = nullptr;

    // ── Account tab widgets ───────────────────────────────────────────────
    QLabel*        m_displayNameLabel   = nullptr;
    QLineEdit*     m_displayNameEdit    = nullptr;
    QPushButton*   m_avatarColorBtn     = nullptr;
    QLineEdit*     m_currentPassEdit    = nullptr;
    QLineEdit*     m_newPassEdit        = nullptr;
    QLineEdit*     m_confirmPassEdit    = nullptr;
    QLabel*        m_passwordStatusLabel= nullptr;

    // ── Core ─────────────────────────────────────────────────────────────
    QTabWidget*    m_tabWidget          = nullptr;
    AppSettings    m_settings;
    QString        m_username;
};

#endif // SETTINGSDIALOG_H
