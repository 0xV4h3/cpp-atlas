#include "ui/AdminQuizEditorDialog.h"
#include "quiz/AdminContentService.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVBoxLayout>

#include "quiz/QuizDatabase.h"

// ─────────────────────────────────────────────────────────────────────────────

AdminQuizEditorDialog::AdminQuizEditorDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
    setWindowTitle(tr("New Quiz"));
}

AdminQuizEditorDialog::AdminQuizEditorDialog(int quizId, QWidget* parent)
    : QDialog(parent)
    , m_quizId(quizId)
{
    setupUi();
    setWindowTitle(tr("Edit Quiz"));
    loadQuiz(quizId);
}

// ─────────────────────────────────────────────────────────────────────────────

void AdminQuizEditorDialog::setupUi()
{
    setMinimumWidth(400);
    QVBoxLayout* root = new QVBoxLayout(this);

    QFormLayout* form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignRight);

    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setPlaceholderText(tr("Quiz title"));
    form->addRow(tr("Title:"), m_titleEdit);

    m_descEdit = new QPlainTextEdit(this);
    m_descEdit->setPlaceholderText(tr("Optional description"));
    m_descEdit->setMaximumHeight(80);
    form->addRow(tr("Description:"), m_descEdit);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItems({tr("standard"), tr("adaptive"), tr("custom")});
    form->addRow(tr("Type:"), m_typeCombo);

    m_difficultySpin = new QSpinBox(this);
    m_difficultySpin->setRange(1, 5);
    m_difficultySpin->setValue(1);
    form->addRow(tr("Difficulty (1–5):"), m_difficultySpin);

    m_timeLimitSpin = new QSpinBox(this);
    m_timeLimitSpin->setRange(0, 3600);
    m_timeLimitSpin->setSuffix(tr(" s"));
    form->addRow(tr("Time limit:"), m_timeLimitSpin);

    m_isTimedCheck = new QCheckBox(tr("Timed"), this);
    form->addRow(QString(), m_isTimedCheck);

    root->addLayout(form);

    QDialogButtonBox* bb = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(bb, &QDialogButtonBox::accepted, this, &AdminQuizEditorDialog::onAccept);
    connect(bb, &QDialogButtonBox::rejected, this, &AdminQuizEditorDialog::reject);
    root->addWidget(bb);
}

void AdminQuizEditorDialog::loadQuiz(int quizId)
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT title, description, type, difficulty, time_limit, is_timed "
              "FROM quizzes WHERE id = :id");
    q.bindValue(":id", quizId);
    if (!q.exec() || !q.next())
        return;

    m_titleEdit->setText(q.value("title").toString());
    m_descEdit->setPlainText(q.value("description").toString());
    const int typeIdx = m_typeCombo->findText(q.value("type").toString());
    if (typeIdx >= 0) m_typeCombo->setCurrentIndex(typeIdx);
    m_difficultySpin->setValue(q.value("difficulty").toInt());
    m_timeLimitSpin->setValue(q.value("time_limit").toInt());
    m_isTimedCheck->setChecked(q.value("is_timed").toInt() == 1);
}

void AdminQuizEditorDialog::onAccept()
{
    const QString title = m_titleEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, tr("Validation"), tr("Title is required."));
        return;
    }

    QVariantMap payload;
    payload["title"]       = title;
    payload["description"] = m_descEdit->toPlainText().trimmed();
    payload["type"]        = m_typeCombo->currentText();
    payload["difficulty"]  = m_difficultySpin->value();
    payload["time_limit"]  = m_timeLimitSpin->value();
    payload["is_timed"]    = m_isTimedCheck->isChecked();

    AdminContentService& svc = AdminContentService::instance();
    AdminOpResult result;
    if (m_quizId == -1) {
        result = svc.createQuiz(payload);
        if (result.ok) m_quizId = result.entityId;
    } else {
        result = svc.updateQuiz(m_quizId, payload);
    }

    if (!result.ok) {
        QMessageBox::critical(this, tr("Save failed"), result.message);
        return;
    }

    m_resultMessage = result.message;
    accept();
}
