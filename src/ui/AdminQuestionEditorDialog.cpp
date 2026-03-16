#include "ui/AdminQuestionEditorDialog.h"
#include "quiz/AdminContentService.h"
#include "quiz/QuizRepository.h"
#include "quiz/QuizDatabase.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFont>

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

AdminQuestionEditorDialog::AdminQuestionEditorDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
}

AdminQuestionEditorDialog::AdminQuestionEditorDialog(int questionId, QWidget* parent)
    : QDialog(parent)
    , m_questionId(questionId)
{
    setupUi();
    loadQuestion(questionId);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI setup
// ─────────────────────────────────────────────────────────────────────────────

void AdminQuestionEditorDialog::setupUi()
{
    setWindowTitle(m_questionId == -1 ? tr("New Question") : tr("Edit Question"));
    setMinimumSize(700, 600);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setSpacing(8);

    // ── Basic fields ─────────────────────────────────────────────────────────
    QGroupBox* basicGroup = new QGroupBox(tr("Question details"), this);
    QFormLayout* form = new QFormLayout(basicGroup);

    m_typeCombo = new QComboBox(basicGroup);
    m_typeCombo->addItems({"mcq", "multi_select", "true_false", "code_output", "fill_blank"});
    connect(m_typeCombo, &QComboBox::currentTextChanged,
            this, &AdminQuestionEditorDialog::onTypeChanged);
    form->addRow(tr("Type:"), m_typeCombo);

    m_contentEdit = new QPlainTextEdit(basicGroup);
    m_contentEdit->setPlaceholderText(tr("Question text (required)"));
    m_contentEdit->setMaximumHeight(80);
    form->addRow(tr("Content:"), m_contentEdit);

    m_codeEdit = new QPlainTextEdit(basicGroup);
    m_codeEdit->setPlaceholderText(tr("Optional code snippet shown with the question"));
    m_codeEdit->setMaximumHeight(70);
    m_codeEdit->setFont(QFont("Monospace", 9));
    form->addRow(tr("Code snippet:"), m_codeEdit);

    m_explanationEdit = new QPlainTextEdit(basicGroup);
    m_explanationEdit->setPlaceholderText(tr("Explanation shown after answering (must NOT be the answer token for fill_blank)"));
    m_explanationEdit->setMaximumHeight(70);
    form->addRow(tr("Explanation:"), m_explanationEdit);

    m_hintEdit = new QLineEdit(basicGroup);
    form->addRow(tr("Hint:"), m_hintEdit);

    m_refUrlEdit = new QLineEdit(basicGroup);
    form->addRow(tr("Reference URL:"), m_refUrlEdit);

    QHBoxLayout* numerics = new QHBoxLayout;
    m_difficultySpin = new QSpinBox(basicGroup);
    m_difficultySpin->setRange(1, 4);
    m_difficultySpin->setValue(1);
    numerics->addWidget(new QLabel(tr("Difficulty [1..4]:")));
    numerics->addWidget(m_difficultySpin);
    numerics->addSpacing(16);
    m_pointsSpin = new QSpinBox(basicGroup);
    m_pointsSpin->setRange(1, 100);
    m_pointsSpin->setValue(10);
    numerics->addWidget(new QLabel(tr("Points:")));
    numerics->addWidget(m_pointsSpin);
    numerics->addSpacing(16);
    m_timeLimitSpin = new QSpinBox(basicGroup);
    m_timeLimitSpin->setRange(0, 3600);
    m_timeLimitSpin->setSuffix(tr(" sec"));
    numerics->addWidget(new QLabel(tr("Time limit:")));
    numerics->addWidget(m_timeLimitSpin);
    numerics->addSpacing(16);
    m_orderIndexSpin = new QSpinBox(basicGroup);
    m_orderIndexSpin->setRange(0, 9999);
    numerics->addWidget(new QLabel(tr("Order:")));
    numerics->addWidget(m_orderIndexSpin);
    numerics->addStretch(1);
    form->addRow(numerics);

    root->addWidget(basicGroup);

    // ── Answer area (stacked: options table vs accepted-answers list) ─────────
    m_answerStack = new QStackedWidget(this);

    // Page 0: options table
    QWidget* optionsPage = new QWidget;
    QVBoxLayout* optLayout = new QVBoxLayout(optionsPage);
    optLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox* optGroup = new QGroupBox(tr("Options"), optionsPage);
    QVBoxLayout* optGroupLayout = new QVBoxLayout(optGroup);

    m_optionsTable = new QTableWidget(0, 3, optGroup);
    m_optionsTable->setHorizontalHeaderLabels({tr("Content"), tr("Code"), tr("Correct?")});
    m_optionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_optionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_optionsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    optGroupLayout->addWidget(m_optionsTable);

    QHBoxLayout* optBtns = new QHBoxLayout;
    m_addOptionBtn = new QPushButton(tr("+ Add option"), optGroup);
    m_removeOptionBtn = new QPushButton(tr("− Remove selected"), optGroup);
    connect(m_addOptionBtn,    &QPushButton::clicked, this, &AdminQuestionEditorDialog::onAddOption);
    connect(m_removeOptionBtn, &QPushButton::clicked, this, &AdminQuestionEditorDialog::onRemoveOption);
    optBtns->addWidget(m_addOptionBtn);
    optBtns->addWidget(m_removeOptionBtn);
    optBtns->addStretch(1);
    optGroupLayout->addLayout(optBtns);
    optLayout->addWidget(optGroup);
    m_answerStack->addWidget(optionsPage);

    // Page 1: accepted-answers list (fill_blank)
    QWidget* answersPage = new QWidget;
    QVBoxLayout* ansLayout = new QVBoxLayout(answersPage);
    ansLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox* ansGroup = new QGroupBox(tr("Accepted answers (fill_blank)"), answersPage);
    QVBoxLayout* ansGroupLayout = new QVBoxLayout(ansGroup);

    m_answerHintLabel = new QLabel(
        tr("Each entry is an accepted answer token (case-insensitive, whitespace-collapsed).\n"
           "Do NOT place answer tokens in the explanation — that field is shown to users."),
        ansGroup);
    m_answerHintLabel->setWordWrap(true);
    m_answerHintLabel->setStyleSheet("color: #e5c07b;");
    ansGroupLayout->addWidget(m_answerHintLabel);

    m_answersWidget = new QListWidget(ansGroup);
    ansGroupLayout->addWidget(m_answersWidget);

    QHBoxLayout* ansBtns = new QHBoxLayout;
    m_answerInputEdit = new QLineEdit(ansGroup);
    m_answerInputEdit->setPlaceholderText(tr("Type answer token…"));
    m_addAnswerBtn    = new QPushButton(tr("+ Add"), ansGroup);
    m_removeAnswerBtn = new QPushButton(tr("− Remove"), ansGroup);
    connect(m_addAnswerBtn,    &QPushButton::clicked, this, &AdminQuestionEditorDialog::onAddAcceptedAnswer);
    connect(m_removeAnswerBtn, &QPushButton::clicked, this, &AdminQuestionEditorDialog::onRemoveAcceptedAnswer);
    ansBtns->addWidget(m_answerInputEdit, 1);
    ansBtns->addWidget(m_addAnswerBtn);
    ansBtns->addWidget(m_removeAnswerBtn);
    ansGroupLayout->addLayout(ansBtns);

    ansLayout->addWidget(ansGroup);
    m_answerStack->addWidget(answersPage);

    root->addWidget(m_answerStack, 1);

    // ── Dialog buttons ───────────────────────────────────────────────────────
    QDialogButtonBox* btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(btns, &QDialogButtonBox::accepted, this, &AdminQuestionEditorDialog::onAccept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(btns);

    // Default to options page
    m_answerStack->setCurrentIndex(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Load existing question
// ─────────────────────────────────────────────────────────────────────────────

void AdminQuestionEditorDialog::loadQuestion(int questionId)
{
    QuizRepository repo;
    const QuestionDTO q = repo.questionById(questionId);
    if (q.id == -1) return;

    m_typeCombo->setCurrentText(q.type);
    m_contentEdit->setPlainText(q.content);
    m_codeEdit->setPlainText(q.codeSnippet);
    m_explanationEdit->setPlainText(q.explanation);
    m_hintEdit->setText(q.hint);
    m_refUrlEdit->setText(q.refUrl);
    m_difficultySpin->setValue(q.difficulty);
    m_pointsSpin->setValue(q.points);
    m_timeLimitSpin->setValue(q.timeLimitSec);
    m_orderIndexSpin->setValue(q.orderIndex);

    if (q.type == "fill_blank") {
        // Populate accepted answers list
        populateAcceptedAnswers(q.acceptedAnswers.isEmpty()
            ? [&]() -> QStringList {
                QStringList opts;
                for (const auto& o : q.options)
                    if (o.isCorrect) opts << o.content;
                return opts;
              }()
            : q.acceptedAnswers);
    } else {
        populateOptionsTable([&]() -> QVariantMap {
            Q_UNUSED(q);
            return {};
        }());
        // Fill options table directly from DTO
        m_optionsTable->setRowCount(0);
        for (const auto& opt : q.options) {
            const int row = m_optionsTable->rowCount();
            m_optionsTable->insertRow(row);
            m_optionsTable->setItem(row, 0, new QTableWidgetItem(opt.content));
            m_optionsTable->setItem(row, 1, new QTableWidgetItem(opt.codeSnippet));
            auto* chk = new QTableWidgetItem;
            chk->setCheckState(opt.isCorrect ? Qt::Checked : Qt::Unchecked);
            m_optionsTable->setItem(row, 2, chk);
        }
    }
}

void AdminQuestionEditorDialog::populateOptionsTable(const QVariantMap& /*unused*/)
{
    // Called to reset; actual population is done directly in loadQuestion
}

void AdminQuestionEditorDialog::populateAcceptedAnswers(const QStringList& answers)
{
    m_answersWidget->clear();
    for (const QString& a : answers)
        m_answersWidget->addItem(a);
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void AdminQuestionEditorDialog::onTypeChanged(const QString& type)
{
    m_answerStack->setCurrentIndex(type == "fill_blank" ? 1 : 0);
}

void AdminQuestionEditorDialog::onAddOption()
{
    const int row = m_optionsTable->rowCount();
    m_optionsTable->insertRow(row);
    m_optionsTable->setItem(row, 0, new QTableWidgetItem(QString()));
    m_optionsTable->setItem(row, 1, new QTableWidgetItem(QString()));
    auto* chk = new QTableWidgetItem;
    chk->setCheckState(Qt::Unchecked);
    m_optionsTable->setItem(row, 2, chk);
    m_optionsTable->editItem(m_optionsTable->item(row, 0));
}

void AdminQuestionEditorDialog::onRemoveOption()
{
    const int row = m_optionsTable->currentRow();
    if (row >= 0) m_optionsTable->removeRow(row);
}

void AdminQuestionEditorDialog::onAddAcceptedAnswer()
{
    const QString token = m_answerInputEdit->text().simplified();
    if (token.isEmpty()) return;
    m_answersWidget->addItem(token);
    m_answerInputEdit->clear();
}

void AdminQuestionEditorDialog::onRemoveAcceptedAnswer()
{
    delete m_answersWidget->takeItem(m_answersWidget->currentRow());
}

void AdminQuestionEditorDialog::onAccept()
{
    const QString type    = m_typeCombo->currentText();
    const QString content = m_contentEdit->toPlainText().simplified();

    if (content.isEmpty()) {
        QMessageBox::warning(this, tr("Validation"), tr("Question content must not be empty."));
        return;
    }

    QVariantMap payload;
    payload["type"]        = type;
    payload["content"]     = content;
    payload["code_snippet"]= m_codeEdit->toPlainText();
    payload["explanation"] = m_explanationEdit->toPlainText();
    payload["hint"]        = m_hintEdit->text();
    payload["ref_url"]     = m_refUrlEdit->text();
    payload["difficulty"]  = m_difficultySpin->value();
    payload["points"]      = m_pointsSpin->value();
    payload["time_limit"]  = m_timeLimitSpin->value();
    payload["order_index"] = m_orderIndexSpin->value();

    AdminContentService& svc = AdminContentService::instance();
    AdminOpResult result;

    if (m_questionId == -1) {
        result = svc.createQuestion(payload);
    } else {
        result = svc.updateQuestion(m_questionId, payload);
    }

    if (!result.ok) {
        QMessageBox::critical(this, tr("Save failed"), result.message);
        return;
    }

    // For fill_blank: also save accepted answers into fill_blank_answers table.
    // In create mode, we need to get the newly inserted question id first.
    int questionIdForAnswers = m_questionId;
    if (type == "fill_blank") {
        if (questionIdForAnswers == -1) {
            // Retrieve the newly created question id
            QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
            QSqlQuery idq(db);
            idq.exec("SELECT MAX(id) FROM questions");
            if (idq.next()) {
                questionIdForAnswers = idq.value(0).toInt();
                m_questionId = questionIdForAnswers;  // update for affectedQuestionId()
            }
        }

        if (questionIdForAnswers > 0) {
            QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
            db.transaction();

            // Remove old entries and re-insert
            QSqlQuery del(db);
            del.prepare("DELETE FROM fill_blank_answers WHERE question_id = :qid");
            del.bindValue(":qid", questionIdForAnswers);
            del.exec();

            for (int i = 0; i < m_answersWidget->count(); ++i) {
                QSqlQuery ins(db);
                ins.prepare(
                    "INSERT INTO fill_blank_answers (question_id, answer, order_index) "
                    "VALUES (:qid, :ans, :idx)"
                );
                ins.bindValue(":qid", questionIdForAnswers);
                ins.bindValue(":ans", m_answersWidget->item(i)->text());
                ins.bindValue(":idx", i);
                ins.exec();
            }
            db.commit();
        }
    }

    m_resultMessage = result.message;
    accept();
}
