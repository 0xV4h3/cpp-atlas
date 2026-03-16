#pragma once
#include <QDialog>
#include <QVariantMap>

class QComboBox;
class QLineEdit;
class QPlainTextEdit;
class QSpinBox;
class QTableWidget;
class QListWidget;
class QStackedWidget;
class QLabel;
class QPushButton;

/**
 * @brief Modal dialog for creating or editing a quiz question and its options.
 *
 * The dialog operates in two modes:
 *  - **Create** mode: all fields start empty; OK calls AdminContentService::createQuestion.
 *  - **Edit** mode:   fields are pre-populated from @p questionId; OK calls updateQuestion.
 *
 * For fill_blank questions an explicit "Accepted answers" list widget replaces the
 * regular options table so that correct answer tokens are managed separately
 * from the explanation text.
 *
 * Usage:
 * @code
 *   // Create new question
 *   AdminQuestionEditorDialog dlg(this);
 *   if (dlg.exec() == QDialog::Accepted) { ... }
 *
 *   // Edit existing question
 *   AdminQuestionEditorDialog dlg(questionId, this);
 *   if (dlg.exec() == QDialog::Accepted) { ... }
 * @endcode
 */
class AdminQuestionEditorDialog : public QDialog
{
    Q_OBJECT
public:
    /** Create-mode constructor. */
    explicit AdminQuestionEditorDialog(QWidget* parent = nullptr);

    /** Edit-mode constructor. Loads the question identified by @p questionId. */
    explicit AdminQuestionEditorDialog(int questionId, QWidget* parent = nullptr);

    /**
     * @brief The question id affected by the last accepted operation.
     * Returns -1 in create mode before acceptance, or the id of the created row
     * after acceptance.
     */
    int affectedQuestionId() const { return m_questionId; }

    /**
     * @brief The result message from the last AdminContentService call.
     */
    QString resultMessage() const { return m_resultMessage; }

private slots:
    void onTypeChanged(const QString& type);
    void onAddOption();
    void onRemoveOption();
    void onAddAcceptedAnswer();
    void onRemoveAcceptedAnswer();
    void onAccept();

private:
    void setupUi();
    void loadQuestion(int questionId);
    void populateOptionsTable(const QVariantMap& questionData);
    void populateAcceptedAnswers(const QStringList& answers);

    // ── Widgets ──────────────────────────────────────────────────────────────
    QComboBox*      m_quizCombo       = nullptr;
    QComboBox*      m_typeCombo       = nullptr;
    QPlainTextEdit* m_contentEdit     = nullptr;
    QPlainTextEdit* m_codeEdit        = nullptr;
    QPlainTextEdit* m_explanationEdit = nullptr;
    QLineEdit*      m_hintEdit        = nullptr;
    QLineEdit*      m_refUrlEdit      = nullptr;
    QSpinBox*       m_difficultySpin  = nullptr;
    QSpinBox*       m_pointsSpin      = nullptr;
    QSpinBox*       m_timeLimitSpin   = nullptr;
    QSpinBox*       m_orderIndexSpin  = nullptr;

    // Options area (mcq / multi_select / true_false / code_output)
    QTableWidget*   m_optionsTable    = nullptr;
    QPushButton*    m_addOptionBtn    = nullptr;
    QPushButton*    m_removeOptionBtn = nullptr;

    // Accepted-answers area (fill_blank only)
    QListWidget*    m_answersWidget   = nullptr;
    QLineEdit*      m_answerInputEdit = nullptr;
    QPushButton*    m_addAnswerBtn    = nullptr;
    QPushButton*    m_removeAnswerBtn = nullptr;

    // Stacked widget switches between options and accepted-answers panels
    QStackedWidget* m_answerStack     = nullptr;
    QLabel*         m_answerHintLabel = nullptr;

    // ── State ────────────────────────────────────────────────────────────────
    int     m_questionId    = -1;  // -1 = create mode
    QString m_resultMessage;
};
