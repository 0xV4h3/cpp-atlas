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
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QShowEvent>
#include "core/AppSettings.h"

/**
 * @brief Tabbed settings dialog for CppAtlas.
 *
 * Tabs:
 *  - Appearance: theme, editor font family/size, line numbers, word wrap
 *  - Account:    display name, avatar color/image, change password
 *
 * Emits settingsChanged() when font/theme settings are applied so that
 * MainWindow can react (e.g., update window title, re-apply fonts).
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QString& username, QWidget* parent = nullptr);
    ~SettingsDialog() = default;

    /** Reload all controls from AppSettings (called on showEvent). */
    void syncFromSettings();

signals:
    /** Emitted when appearance settings (font, theme) are applied. */
    void settingsChanged();

private slots:
    void applyTheme();
    void onThemeChanged(const QString& themeName);
    void onApplyDisplayName();
    void onChangeAvatarColor();
    void onUploadAvatar();
    void onRemoveAvatar();
    void onChangePassword();
    void onApplyAppearance();
    void onLiveApply();
    void onResetAppearance();

protected:
    void showEvent(QShowEvent* event) override;

private:
    void setupUi();
    void setupAppearanceTab(QWidget* tab);
    void setupAccountTab(QWidget* tab);
    void updateAvatarPreview();

    // Appearance tab: snapshot for Reset
    QString        m_initialTheme;
    QString        m_initialFontFamily;
    int            m_initialFontSize   = 10;
    bool           m_initialLineNums   = true;
    bool           m_initialWordWrap   = false;

    // ── Appearance tab widgets ────────────────────────────────────────────
    QComboBox*     m_themeCombo         = nullptr;
    QFontComboBox* m_fontFamilyCombo    = nullptr;
    QSpinBox*      m_fontSizeSpinBox    = nullptr;
    QCheckBox*     m_lineNumbersCheck   = nullptr;
    QCheckBox*     m_wordWrapCheck      = nullptr;

    // ── Analysis panel per-tool editor widgets ────────────────────────────
    // Each tool has: font family, font size, line numbers, word wrap
    struct ToolEditorControls {
        QFontComboBox* fontFamilyCombo  = nullptr;
        QSpinBox*      fontSizeSpinBox  = nullptr;
        QCheckBox*     lineNumbersCheck = nullptr;
        QCheckBox*     wordWrapCheck    = nullptr;
    };
    ToolEditorControls m_insightsControls;
    ToolEditorControls m_assemblyControls;
    ToolEditorControls m_benchmarkControls;

    void setupToolEditorGroup(QWidget* parent, QVBoxLayout* layout,
                              const QString& title, const QString& toolKey,
                              ToolEditorControls& controls);

    // ── Account tab widgets ───────────────────────────────────────────────
    QLabel*        m_displayNameLabel   = nullptr;
    QLineEdit*     m_displayNameEdit    = nullptr;
    QLabel*        m_avatarPreviewLabel = nullptr;
    QPushButton*   m_avatarColorBtn     = nullptr;
    QPushButton*   m_uploadAvatarBtn    = nullptr;
    QPushButton*   m_removeAvatarBtn    = nullptr;
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
