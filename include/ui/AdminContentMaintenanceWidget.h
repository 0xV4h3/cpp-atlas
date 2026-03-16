#pragma once
#include <QWidget>

class QTableWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QGroupBox;
class QLabel;

/**
 * @brief Full CRUD maintenance widget for quiz content.
 *
 * Provides:
 *  - Questions table with search/filter (by quiz, type, active state, text)
 *  - Quizzes table with search/filter
 *  - Actions: create/edit/soft-delete/restore for both questions and quizzes
 *  - Option editor panel for the selected question
 *  - Accepted-answer editor for fill_blank questions
 *
 * All writes go through AdminContentService — no raw DB access from this widget.
 */
class AdminContentMaintenanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminContentMaintenanceWidget(QWidget* parent = nullptr);

    /** Refresh both tables from the database. */
    void refreshAll();

signals:
    /** Emitted with a human-readable message after every write operation. */
    void operationCompleted(const QString& message);

private slots:
    // Questions
    void onCreateQuestion();
    void onEditQuestion();
    void onDeleteQuestion();
    void onRestoreQuestion();
    void onFilterQuestions();
    void onQuestionSelectionChanged();

    // Quizzes
    void onCreateQuiz();
    void onEditQuiz();
    void onDeleteQuiz();
    void onRestoreQuiz();
    void onFilterQuizzes();

private:
    void setupUi();
    void setupQuestionsGroup(QWidget* parent);
    void setupQuizzesGroup(QWidget* parent);

    void loadQuestions();
    void loadQuizzes();
    void loadQuizzesIntoFilter();

    int selectedQuestionId() const;
    int selectedQuizId() const;

    // ── Questions area ────────────────────────────────────────────────────────
    QTableWidget* m_questionsTable   = nullptr;
    QLineEdit*    m_qSearchEdit      = nullptr;
    QComboBox*    m_qQuizFilter      = nullptr;
    QComboBox*    m_qTypeFilter      = nullptr;
    QComboBox*    m_qActiveFilter    = nullptr;
    QPushButton*  m_createQBtn       = nullptr;
    QPushButton*  m_editQBtn         = nullptr;
    QPushButton*  m_deleteQBtn       = nullptr;
    QPushButton*  m_restoreQBtn      = nullptr;

    // ── Quizzes area ──────────────────────────────────────────────────────────
    QTableWidget* m_quizzesTable     = nullptr;
    QLineEdit*    m_qzSearchEdit     = nullptr;
    QPushButton*  m_createQzBtn      = nullptr;
    QPushButton*  m_editQzBtn        = nullptr;
    QPushButton*  m_deleteQzBtn      = nullptr;
    QPushButton*  m_restoreQzBtn     = nullptr;
};
