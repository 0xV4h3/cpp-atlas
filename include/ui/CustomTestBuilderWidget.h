#ifndef CUSTOMTESTBUILDERWIDGET_H
#define CUSTOMTESTBUILDERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QSplitter>
#include <QStackedWidget>

#include "quiz/QuizRepository.h"
#include "quiz/QuizExporter.h"

/**
 * @brief Custom Test Builder — Page 4 of QuizModeWindow.
 *
 * Two sub-views (inner QStackedWidget):
 *   Sub-page 0 — "My Tests" list: saved custom tests + Import/Export buttons
 *   Sub-page 1 — "Builder": topic/question browser (left), selected questions (right),
 *                            random generator + save/save-as controls (bottom)
 */
class CustomTestBuilderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTestBuilderWidget(QWidget* parent = nullptr);
    ~CustomTestBuilderWidget() = default;

    void refresh();

signals:
    void launchCustomTest(const QList<QuestionDTO>& questions);
    void backRequested();

private slots:
    void onTopicSelected();
    void onAddQuestionClicked();
    void onAddAllClicked();
    void onRemoveQuestionClicked();
    void onRemoveAllClicked();
    void onMoveUpClicked();
    void onMoveDownClicked();
    void onGenerateRandomClicked();
    void onSaveTestClicked();
    void onSaveAsClicked();
    void onLaunchClicked();
    void onExportClicked();              // used in both My Tests page and builder
    void onExportSelectedTestClicked(); // My Tests page export
    void onImportClicked();
    void onDeleteTestClicked();
    void onLoadTestClicked();
    void onNewTestClicked();
    void onSearchChanged(const QString& text);
    void applyTheme();

private:
    void setupUi();
    void setupMyTestsPage();
    void setupBuilderPage();
    void populateTopicTree();
    void populateQuestionBrowser(int topicId, const QString& filterText = {});
    void populateMyTests();
    void addQuestionToSelected(const QuestionDTO& q);
    void refreshSelectedList();
    void clearBuilder();
    void updateSaveButtonStates();
    QString difficultyLabel(int d) const;
    QString questionTypeLabel(const QString& type) const;

    QuizRepository   m_repo;

    // Inner stack: 0=MyTests, 1=Builder
    QStackedWidget*  m_innerStack       = nullptr;

    // ── Sub-page 0: My Tests ─────────────────────────────────────────────────
    QListWidget*     m_myTestsList      = nullptr;
    QPushButton*     m_newTestBtn       = nullptr;
    QPushButton*     m_loadTestBtn      = nullptr;
    QPushButton*     m_editTestBtn      = nullptr;
    QPushButton*     m_exportMyTestBtn  = nullptr;  // Export on My Tests page
    QPushButton*     m_deleteTestBtn    = nullptr;
    QPushButton*     m_importBtn        = nullptr;

    // ── Sub-page 1: Builder ──────────────────────────────────────────────────
    QLineEdit*       m_searchEdit       = nullptr;  // search by tag/title
    QTreeWidget*     m_topicTree        = nullptr;
    QListWidget*     m_questionList     = nullptr;
    QPushButton*     m_addBtn           = nullptr;
    QPushButton*     m_addAllBtn        = nullptr;

    QListWidget*     m_selectedList     = nullptr;
    QPushButton*     m_moveUpBtn        = nullptr;
    QPushButton*     m_moveDownBtn      = nullptr;
    QPushButton*     m_removeBtn        = nullptr;
    QPushButton*     m_removeAllBtn     = nullptr;

    // Bottom bar
    QComboBox*       m_diffCombo        = nullptr;
    QSpinBox*        m_countSpin        = nullptr;
    QPushButton*     m_generateBtn      = nullptr;
    QLineEdit*       m_testTitleEdit    = nullptr;
    QLineEdit*       m_testDescEdit     = nullptr;
    QPushButton*     m_saveBtn          = nullptr;  // "Save" (New Test) or "Save Changes" (Edit)
    QPushButton*     m_saveAsBtn        = nullptr;  // "Save As New Test" (Edit only)
    QPushButton*     m_launchBtn        = nullptr;

    // State
    QList<QuestionDTO> m_selectedQuestions;
    QList<QuizDTO>     m_myTests;
    int                m_editingTestId   = -1;  // -1 = new, >0 = editing
    bool               m_hasChanges      = false;

    // Snapshot for change detection (Edit mode)
    QList<int>         m_originalQuestionIds;
    QString            m_originalTitle;
    QString            m_originalDesc;
};

#endif // CUSTOMTESTBUILDERWIDGET_H
