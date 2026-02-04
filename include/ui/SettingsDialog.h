#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QFontComboBox>

/**
 * @brief Settings dialog for IDE configuration
 */
class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void onApply();
    void onPageChanged(int row);

private:
    void setupUi();
    void loadSettings();
    void saveSettings();
    
    QListWidget* m_pageList;
    QStackedWidget* m_pages;
    
    // General
    QComboBox* m_themeCombo;
    QCheckBox* m_restoreSessionCheck;
    
    // Editor
    QFontComboBox* m_fontCombo;
    QSpinBox* m_fontSizeSpinBox;
    QSpinBox* m_tabWidthSpinBox;
    QCheckBox* m_showLineNumbersCheck;
    QCheckBox* m_wordWrapCheck;
    
    // Compiler
    QComboBox* m_defaultCompilerCombo;
    QComboBox* m_defaultStandardCombo;
};

#endif // SETTINGSDIALOG_H
