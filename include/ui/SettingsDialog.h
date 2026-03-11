#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>

class QTabWidget;
class QComboBox;
class QFontComboBox;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QLabel;
class QLineEdit;

/**
 * @brief Non-modal settings dialog with Appearance and Account tabs.
 *
 * Usage (non-modal, auto-deletes on close):
 * @code
 *   auto* dlg = new SettingsDialog(username, parentWidget);
 *   dlg->setAttribute(Qt::WA_DeleteOnClose);
 *   connect(dlg, &SettingsDialog::settingsChanged, this, &MainWindow::applyEditorSettings);
 *   dlg->show();
 * @endcode
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QString& username, QWidget* parent = nullptr);

signals:
    /** Emitted when settings are applied and callers should reload them. */
    void settingsChanged();

private slots:
    void onApply();
    void onReset();
    void onThemeChanged(int index);
    void onAvatarUploadClicked();
    void onDisplayNameSave();
    void onPasswordChange();

private:
    void setupAppearanceTab(QTabWidget* tabs);
    void setupAccountTab(QTabWidget* tabs);
    void loadCurrentValues();
    void applyAppearanceSettings();
    void updateAvatarPreview(const QString& path);

    QString m_username;

    // Appearance tab widgets
    QComboBox*     m_themeCombo         = nullptr;
    QFontComboBox* m_fontFamilyCombo    = nullptr;
    QSpinBox*      m_fontSizeSpinBox    = nullptr;
    QCheckBox*     m_showLineNumbersChk = nullptr;
    QCheckBox*     m_wordWrapChk        = nullptr;

    // Account tab widgets
    QLabel*    m_avatarPreview     = nullptr;
    QLineEdit* m_displayNameEdit   = nullptr;
    QLineEdit* m_oldPasswordEdit   = nullptr;
    QLineEdit* m_newPasswordEdit   = nullptr;
    QLineEdit* m_confirmPassEdit   = nullptr;

    // Stored initial values for Reset
    QString m_initialTheme;
    QString m_initialFontFamily;
    int     m_initialFontSize = 12;
    bool    m_initialShowLineNumbers = true;
    bool    m_initialWordWrap        = false;

    QString m_currentAvatarPath;
};

#endif // SETTINGSDIALOG_H
