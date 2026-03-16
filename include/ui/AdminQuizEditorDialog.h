#pragma once
#include <QDialog>
#include <QVariantMap>

class QComboBox;
class QLineEdit;
class QPlainTextEdit;
class QSpinBox;
class QCheckBox;

/**
 * @brief Modal dialog for creating or editing a quiz.
 *
 * Operates in two modes:
 *  - **Create** mode: all fields start empty; OK calls AdminContentService::createQuiz.
 *  - **Edit** mode:   fields are pre-populated from @p quizId; OK calls updateQuiz.
 */
class AdminQuizEditorDialog : public QDialog
{
    Q_OBJECT
public:
    /** Create-mode constructor. */
    explicit AdminQuizEditorDialog(QWidget* parent = nullptr);

    /** Edit-mode constructor. Loads the quiz identified by @p quizId. */
    explicit AdminQuizEditorDialog(int quizId, QWidget* parent = nullptr);

    /**
     * @brief The quiz id affected by the last accepted operation.
     * Returns -1 in create mode before acceptance.
     */
    int affectedQuizId() const { return m_quizId; }

    /** @brief The result message from the last service call. */
    QString resultMessage() const { return m_resultMessage; }

private slots:
    void onAccept();

private:
    void setupUi();
    void loadQuiz(int quizId);

    QLineEdit*      m_titleEdit        = nullptr;
    QPlainTextEdit* m_descEdit         = nullptr;
    QSpinBox*       m_difficultySpin   = nullptr;
    QSpinBox*       m_timeLimitSpin    = nullptr;
    QCheckBox*      m_isTimedCheck     = nullptr;
    QComboBox*      m_typeCombo        = nullptr;

    int     m_quizId       = -1;
    QString m_resultMessage;
};
