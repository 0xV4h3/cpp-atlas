#pragma once
#include <QMainWindow>

class QLabel;
class QTabWidget;
class QTextEdit;
class QPushButton;

class QuizAdminPanel : public QMainWindow
{
    Q_OBJECT
public:
    explicit QuizAdminPanel(QWidget* parent = nullptr);

private slots:
    void onApplyContentUpdates();
    void onValidateContent();
    void onExportBackup();
    void refreshStats();

private:
    void setupUi();
    void setupContentTab(QWidget* tab);
    void setupValidationTab(QWidget* tab);
    void setupExportTab(QWidget* tab);
    void setupStatsTab(QWidget* tab);
    void log(const QString& message);

    QLabel*     m_modeLabel = nullptr;
    QTabWidget* m_tabs      = nullptr;
    QTextEdit*  m_logView   = nullptr;
};
