#pragma once
#include <QMainWindow>

class QLabel;
class QTabWidget;
class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QString;

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
    void onOpenQuestionEditor();
    void onRollbackLastPatch();
    void onShowJournalHistory();

private:
    void setupUi();
    void setupContentTab(QWidget* tab);
    void setupValidationTab(QWidget* tab);
    void setupExportTab(QWidget* tab);
    void setupStatsTab(QWidget* tab);
    void setupMaintenanceTab(QWidget* tab);
    /** Creates a compact tab layout: description label on top, action button directly below.
     *  The shared activity log (dominant area) is added to the main window layout separately. */
    QVBoxLayout* createAdminTabLayout(QWidget* tab, const QString& description,
                                      QPushButton* actionBtn);
    void log(const QString& message);

    QLabel*     m_modeLabel = nullptr;
    QTabWidget* m_tabs      = nullptr;
    QTextEdit*  m_logView   = nullptr;
};
