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
 *   Sub-page 0 — "My Tests" list: saved custom tests + Import button
 *   Sub-page 1 — "Builder":  topic/question browser on the left,
 *                              selected questions list on the right,
 *                              Random Generator panel at the bottom
 *
 * Workflow:
 *   1. User picks questions manually from the topic/question browser (drag or Add button).
 *   2. OR user clicks "Generate Random" → selects topics + count → fills the right panel.
 *   3. User names the test, saves to DB (createCustomTest + addQuestionToCustomTest).
 *   4. User can Export to .cppatlas file or Import from one.
 *   5. "Launch" button emits launchCustomTest(questions) → QuizModeWindow starts session.
 *
 * Signals:
 *   launchCustomTest(QList<QuestionDTO>) — start a custom session
 *   backRequested()                      — navigate back to selection screen
 */
class CustomTestBuilderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTestBuilderWidget(QWidget* parent = nullptr);
    ~CustomTestBuilderWidget() = default;

    /** Refresh lists from DB (called when page becomes visible). */
    void refresh();

signals:
    /** Emitted when user clicks Launch — QuizModeWindow starts the session. */
    void launchCustomTest(const QList<QuestionDTO>& questions);

    /** Emitted when user clicks Back. */
    void backRequested();

private slots:
    void onTopicSelected();
    void onAddQuestionClicked();
    void onRemoveQuestionClicked();
    void onMoveUpClicked();
    void onMoveDownClicked();
    void onGenerateRandomClicked();
    void onSaveTestClicked();
    void onLaunchClicked();
    void onExportClicked();
    void onImportClicked();
    void onDeleteTestClicked();
    void onLoadTestClicked();
    void onNewTestClicked();
    void applyTheme();

private:
    void setupUi();
    void setupMyTestsPage();
    void setupBuilderPage();
    void populateTopicTree();
    void populateQuestionBrowser(int topicId);
    void populateMyTests();
    void addQuestionToSelected(const QuestionDTO& q);
    void refreshSelectedList();
    void clearBuilder();
    QString difficultyLabel(int d) const;

    QuizRepository   m_repo;

    // Inner stack: 0=MyTests, 1=Builder
    QStackedWidget*  m_innerStack    = nullptr;

    // ── Sub-page 0: My Tests ─────────────────────────────────────────────
    QListWidget*     m_myTestsList   = nullptr;
    QPushButton*     m_newTestBtn    = nullptr;
    QPushButton*     m_loadTestBtn   = nullptr;
    QPushButton*     m_deleteTestBtn = nullptr;
    QPushButton*     m_importBtn     = nullptr;

    // ── Sub-page 1: Builder ──────────────────────────────────────────────
    // Left: Topic tree + Question browser
    QTreeWidget*     m_topicTree     = nullptr;
    QListWidget*     m_questionList  = nullptr;  // browsable questions

    // Right: Selected questions
    QListWidget*     m_selectedList  = nullptr;
    QPushButton*     m_addBtn        = nullptr;
    QPushButton*     m_removeBtn     = nullptr;
    QPushButton*     m_moveUpBtn     = nullptr;
    QPushButton*     m_moveDownBtn   = nullptr;

    // Bottom: Random Generator + Actions
    QComboBox*       m_diffCombo     = nullptr;
    QSpinBox*        m_countSpin     = nullptr;
    QPushButton*     m_generateBtn   = nullptr;
    QLineEdit*       m_testTitleEdit = nullptr;
    QLineEdit*       m_testDescEdit  = nullptr;
    QPushButton*     m_saveBtn       = nullptr;
    QPushButton*     m_launchBtn     = nullptr;
    QPushButton*     m_exportBtn     = nullptr;
    QPushButton*     m_backBtn       = nullptr;

    // State
    QList<QuestionDTO> m_selectedQuestions;   // current builder selection
    QList<QuizDTO>     m_myTests;             // cached list of user's saved tests
    int                m_editingTestId = -1;  // -1 = new test, >0 = editing saved
};

#endif // CUSTOMTESTBUILDERWIDGET_H