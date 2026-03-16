#include "ui/AdminContentMaintenanceWidget.h"
#include "ui/AdminQuestionEditorDialog.h"
#include "ui/AdminQuizEditorDialog.h"
#include "quiz/AdminContentService.h"
#include "quiz/QuizDatabase.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────────────────────────

AdminContentMaintenanceWidget::AdminContentMaintenanceWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    refreshAll();
}

// ─────────────────────────────────────────────────────────────────────────────
// UI construction
// ─────────────────────────────────────────────────────────────────────────────

void AdminContentMaintenanceWidget::setupUi()
{
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(4, 4, 4, 4);
    root->setSpacing(8);

    QWidget* qzWidget = new QWidget(this);
    setupQuizzesGroup(qzWidget);
    root->addWidget(qzWidget);

    QWidget* qWidget = new QWidget(this);
    setupQuestionsGroup(qWidget);
    root->addWidget(qWidget, 1);
}

void AdminContentMaintenanceWidget::setupQuizzesGroup(QWidget* parent)
{
    QGroupBox* box = new QGroupBox(tr("Quizzes"), parent);
    QVBoxLayout* vl = new QVBoxLayout(box);

    // Search bar
    QHBoxLayout* searchRow = new QHBoxLayout;
    m_qzSearchEdit = new QLineEdit(box);
    m_qzSearchEdit->setPlaceholderText(tr("Filter by title…"));
    connect(m_qzSearchEdit, &QLineEdit::textChanged,
            this, &AdminContentMaintenanceWidget::onFilterQuizzes);
    searchRow->addWidget(m_qzSearchEdit, 1);
    vl->addLayout(searchRow);

    // Table
    m_quizzesTable = new QTableWidget(0, 5, box);
    m_quizzesTable->setHorizontalHeaderLabels(
        {tr("ID"), tr("Title"), tr("Type"), tr("Difficulty"), tr("Active")});
    m_quizzesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_quizzesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_quizzesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_quizzesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_quizzesTable->setMinimumHeight(120);
    vl->addWidget(m_quizzesTable);

    // Buttons
    QHBoxLayout* btnRow = new QHBoxLayout;
    m_createQzBtn  = new QPushButton(tr("Create Quiz"),  box);
    m_editQzBtn    = new QPushButton(tr("Edit Quiz"),    box);
    m_deleteQzBtn  = new QPushButton(tr("Delete Quiz"),  box);
    m_restoreQzBtn = new QPushButton(tr("Restore Quiz"), box);
    for (QPushButton* b : {m_createQzBtn, m_editQzBtn, m_deleteQzBtn, m_restoreQzBtn}) {
        b->setMinimumWidth(100);
        btnRow->addWidget(b);
    }
    btnRow->addStretch(1);
    vl->addLayout(btnRow);

    connect(m_createQzBtn,  &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onCreateQuiz);
    connect(m_editQzBtn,    &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onEditQuiz);
    connect(m_deleteQzBtn,  &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onDeleteQuiz);
    connect(m_restoreQzBtn, &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onRestoreQuiz);

    QVBoxLayout* parentLayout = new QVBoxLayout(parent);
    parentLayout->setContentsMargins(0, 0, 0, 0);
    parentLayout->addWidget(box);
}

void AdminContentMaintenanceWidget::setupQuestionsGroup(QWidget* parent)
{
    QGroupBox* box = new QGroupBox(tr("Questions"), parent);
    QVBoxLayout* vl = new QVBoxLayout(box);

    // Filter row
    QHBoxLayout* filterRow = new QHBoxLayout;
    m_qSearchEdit = new QLineEdit(box);
    m_qSearchEdit->setPlaceholderText(tr("Search content…"));
    connect(m_qSearchEdit, &QLineEdit::textChanged,
            this, &AdminContentMaintenanceWidget::onFilterQuestions);

    m_qQuizFilter = new QComboBox(box);
    m_qQuizFilter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(m_qQuizFilter, &QComboBox::currentIndexChanged,
            this, &AdminContentMaintenanceWidget::onFilterQuestions);

    m_qTypeFilter = new QComboBox(box);
    m_qTypeFilter->addItems({tr("All types"), "mcq", "multi_select",
                              "true_false", "code_output", "fill_blank"});
    connect(m_qTypeFilter, &QComboBox::currentIndexChanged,
            this, &AdminContentMaintenanceWidget::onFilterQuestions);

    m_qActiveFilter = new QComboBox(box);
    m_qActiveFilter->addItems({tr("All"), tr("Active"), tr("Deleted")});
    connect(m_qActiveFilter, &QComboBox::currentIndexChanged,
            this, &AdminContentMaintenanceWidget::onFilterQuestions);

    filterRow->addWidget(m_qSearchEdit, 1);
    filterRow->addWidget(new QLabel(tr("Quiz:")));
    filterRow->addWidget(m_qQuizFilter);
    filterRow->addWidget(new QLabel(tr("Type:")));
    filterRow->addWidget(m_qTypeFilter);
    filterRow->addWidget(new QLabel(tr("State:")));
    filterRow->addWidget(m_qActiveFilter);
    vl->addLayout(filterRow);

    // Table
    m_questionsTable = new QTableWidget(0, 6, box);
    m_questionsTable->setHorizontalHeaderLabels(
        {tr("ID"), tr("Quiz"), tr("Type"), tr("Difficulty"), tr("Active"), tr("Order")});
    m_questionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_questionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_questionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_questionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_questionsTable, &QTableWidget::itemSelectionChanged,
            this, &AdminContentMaintenanceWidget::onQuestionSelectionChanged);
    vl->addWidget(m_questionsTable, 1);

    // Buttons
    QHBoxLayout* btnRow = new QHBoxLayout;
    m_createQBtn  = new QPushButton(tr("Create Question"),  box);
    m_editQBtn    = new QPushButton(tr("Edit Question"),    box);
    m_deleteQBtn  = new QPushButton(tr("Delete Question"),  box);
    m_restoreQBtn = new QPushButton(tr("Restore Question"), box);
    for (QPushButton* b : {m_createQBtn, m_editQBtn, m_deleteQBtn, m_restoreQBtn}) {
        b->setMinimumWidth(120);
        btnRow->addWidget(b);
    }
    btnRow->addStretch(1);
    vl->addLayout(btnRow);

    connect(m_createQBtn,  &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onCreateQuestion);
    connect(m_editQBtn,    &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onEditQuestion);
    connect(m_deleteQBtn,  &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onDeleteQuestion);
    connect(m_restoreQBtn, &QPushButton::clicked,
            this, &AdminContentMaintenanceWidget::onRestoreQuestion);

    QVBoxLayout* parentLayout = new QVBoxLayout(parent);
    parentLayout->setContentsMargins(0, 0, 0, 0);
    parentLayout->addWidget(box);
}

// ─────────────────────────────────────────────────────────────────────────────
// Data loading
// ─────────────────────────────────────────────────────────────────────────────

void AdminContentMaintenanceWidget::refreshAll()
{
    loadQuizzesIntoFilter();
    loadQuizzes();
    loadQuestions();
}

void AdminContentMaintenanceWidget::loadQuizzesIntoFilter()
{
    const int currentData = m_qQuizFilter->currentData().toInt();
    m_qQuizFilter->clear();
    m_qQuizFilter->addItem(tr("All quizzes"), -1);

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q("SELECT id, title FROM quizzes ORDER BY title", db);
    while (q.next())
        m_qQuizFilter->addItem(q.value("title").toString(), q.value("id").toInt());

    // Restore previous selection if possible
    for (int i = 0; i < m_qQuizFilter->count(); ++i) {
        if (m_qQuizFilter->itemData(i).toInt() == currentData) {
            m_qQuizFilter->setCurrentIndex(i);
            break;
        }
    }
}

void AdminContentMaintenanceWidget::loadQuestions()
{
    const QString search     = m_qSearchEdit->text().trimmed();
    const int     quizFilter = m_qQuizFilter->currentData().toInt();
    const QString typeFilter = (m_qTypeFilter->currentIndex() == 0)
                               ? QString() : m_qTypeFilter->currentText();
    const int     activeFilter = m_qActiveFilter->currentIndex(); // 0=All 1=Active 2=Deleted

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);

    QString sql =
        "SELECT q.id, qz.title, q.type, q.difficulty, q.is_active, q.order_index, q.content "
        "FROM questions q "
        "LEFT JOIN quizzes qz ON qz.id = q.quiz_id "
        "WHERE 1=1 ";

    if (!search.isEmpty())
        sql += "AND lower(q.content) LIKE :search ";
    if (quizFilter > 0)
        sql += "AND q.quiz_id = :quiz_id ";
    if (!typeFilter.isEmpty())
        sql += "AND q.type = :type ";
    if (activeFilter == 1)
        sql += "AND q.is_active = 1 ";
    else if (activeFilter == 2)
        sql += "AND q.is_active = 0 ";
    sql += "ORDER BY q.order_index";

    QSqlQuery q(db);
    q.prepare(sql);
    if (!search.isEmpty())
        q.bindValue(":search", "%" + search.toLower() + "%");
    if (quizFilter > 0)
        q.bindValue(":quiz_id", quizFilter);
    if (!typeFilter.isEmpty())
        q.bindValue(":type", typeFilter);

    m_questionsTable->setRowCount(0);
    if (!q.exec()) return;

    while (q.next()) {
        const int row = m_questionsTable->rowCount();
        m_questionsTable->insertRow(row);
        m_questionsTable->setItem(row, 0, new QTableWidgetItem(q.value("id").toString()));
        m_questionsTable->setItem(row, 1, new QTableWidgetItem(q.value("title").toString()));
        m_questionsTable->setItem(row, 2, new QTableWidgetItem(q.value("type").toString()));
        m_questionsTable->setItem(row, 3, new QTableWidgetItem(q.value("difficulty").toString()));
        m_questionsTable->setItem(row, 4, new QTableWidgetItem(
            q.value("is_active").toInt() == 1 ? tr("Yes") : tr("No")));
        m_questionsTable->setItem(row, 5, new QTableWidgetItem(q.value("order_index").toString()));
        // Store id as UserRole on column 0
        m_questionsTable->item(row, 0)->setData(Qt::UserRole, q.value("id").toInt());
    }
    m_questionsTable->resizeColumnsToContents();
}

void AdminContentMaintenanceWidget::loadQuizzes()
{
    const QString search = m_qzSearchEdit->text().trimmed();

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QString sql =
        "SELECT id, title, type, difficulty, is_active FROM quizzes WHERE 1=1 ";
    if (!search.isEmpty())
        sql += "AND lower(title) LIKE :search ";
    sql += "ORDER BY title";

    QSqlQuery q(db);
    q.prepare(sql);
    if (!search.isEmpty())
        q.bindValue(":search", "%" + search.toLower() + "%");

    m_quizzesTable->setRowCount(0);
    if (!q.exec()) return;

    while (q.next()) {
        const int row = m_quizzesTable->rowCount();
        m_quizzesTable->insertRow(row);
        m_quizzesTable->setItem(row, 0, new QTableWidgetItem(q.value("id").toString()));
        m_quizzesTable->setItem(row, 1, new QTableWidgetItem(q.value("title").toString()));
        m_quizzesTable->setItem(row, 2, new QTableWidgetItem(q.value("type").toString()));
        m_quizzesTable->setItem(row, 3, new QTableWidgetItem(q.value("difficulty").toString()));
        m_quizzesTable->setItem(row, 4, new QTableWidgetItem(
            q.value("is_active").toInt() == 1 ? tr("Yes") : tr("No")));
        m_quizzesTable->item(row, 0)->setData(Qt::UserRole, q.value("id").toInt());
    }
    m_quizzesTable->resizeColumnsToContents();
}

// ─────────────────────────────────────────────────────────────────────────────
// ID helpers
// ─────────────────────────────────────────────────────────────────────────────

int AdminContentMaintenanceWidget::selectedQuestionId() const
{
    const QList<QTableWidgetItem*> sel = m_questionsTable->selectedItems();
    if (sel.isEmpty()) return -1;
    return m_questionsTable->item(sel.first()->row(), 0)->data(Qt::UserRole).toInt();
}

int AdminContentMaintenanceWidget::selectedQuizId() const
{
    const QList<QTableWidgetItem*> sel = m_quizzesTable->selectedItems();
    if (sel.isEmpty()) return -1;
    return m_quizzesTable->item(sel.first()->row(), 0)->data(Qt::UserRole).toInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// Question slots
// ─────────────────────────────────────────────────────────────────────────────

void AdminContentMaintenanceWidget::onCreateQuestion()
{
    AdminQuestionEditorDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        refreshAll();
        emit operationCompleted(dlg.resultMessage());
    }
}

void AdminContentMaintenanceWidget::onEditQuestion()
{
    const int qid = selectedQuestionId();
    if (qid == -1) {
        QMessageBox::information(this, tr("Select Question"),
                                 tr("Please select a question to edit."));
        return;
    }
    AdminQuestionEditorDialog dlg(qid, this);
    if (dlg.exec() == QDialog::Accepted) {
        refreshAll();
        emit operationCompleted(dlg.resultMessage());
    }
}

void AdminContentMaintenanceWidget::onDeleteQuestion()
{
    const int qid = selectedQuestionId();
    if (qid == -1) {
        QMessageBox::information(this, tr("Select Question"),
                                 tr("Please select a question to delete."));
        return;
    }
    if (QMessageBox::question(this, tr("Confirm Delete"),
            tr("Soft-delete question %1?").arg(qid)) != QMessageBox::Yes)
        return;

    const AdminOpResult r = AdminContentService::instance().softDeleteQuestion(qid);
    if (!r.ok) {
        QMessageBox::critical(this, tr("Delete failed"), r.message);
        return;
    }
    refreshAll();
    emit operationCompleted(r.message);
}

void AdminContentMaintenanceWidget::onRestoreQuestion()
{
    const int qid = selectedQuestionId();
    if (qid == -1) {
        QMessageBox::information(this, tr("Select Question"),
                                 tr("Please select a question to restore."));
        return;
    }
    const AdminOpResult r = AdminContentService::instance().restoreQuestion(qid);
    if (!r.ok) {
        QMessageBox::critical(this, tr("Restore failed"), r.message);
        return;
    }
    refreshAll();
    emit operationCompleted(r.message);
}

void AdminContentMaintenanceWidget::onFilterQuestions()
{
    loadQuestions();
}

void AdminContentMaintenanceWidget::onQuestionSelectionChanged()
{
    // Could enable/disable action buttons based on selection
    const bool hasSelection = selectedQuestionId() != -1;
    m_editQBtn->setEnabled(hasSelection);
    m_deleteQBtn->setEnabled(hasSelection);
    m_restoreQBtn->setEnabled(hasSelection);
}

// ─────────────────────────────────────────────────────────────────────────────
// Quiz slots
// ─────────────────────────────────────────────────────────────────────────────

void AdminContentMaintenanceWidget::onCreateQuiz()
{
    AdminQuizEditorDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        refreshAll();
        emit operationCompleted(dlg.resultMessage());
    }
}

void AdminContentMaintenanceWidget::onEditQuiz()
{
    const int qzid = selectedQuizId();
    if (qzid == -1) {
        QMessageBox::information(this, tr("Select Quiz"),
                                 tr("Please select a quiz to edit."));
        return;
    }
    AdminQuizEditorDialog dlg(qzid, this);
    if (dlg.exec() == QDialog::Accepted) {
        refreshAll();
        emit operationCompleted(dlg.resultMessage());
    }
}

void AdminContentMaintenanceWidget::onDeleteQuiz()
{
    const int qzid = selectedQuizId();
    if (qzid == -1) {
        QMessageBox::information(this, tr("Select Quiz"),
                                 tr("Please select a quiz to delete."));
        return;
    }
    if (QMessageBox::question(this, tr("Confirm Delete"),
            tr("Soft-delete quiz %1?").arg(qzid)) != QMessageBox::Yes)
        return;

    const AdminOpResult r = AdminContentService::instance().softDeleteQuiz(qzid);
    if (!r.ok) {
        QMessageBox::critical(this, tr("Delete failed"), r.message);
        return;
    }
    refreshAll();
    emit operationCompleted(r.message);
}

void AdminContentMaintenanceWidget::onRestoreQuiz()
{
    const int qzid = selectedQuizId();
    if (qzid == -1) {
        QMessageBox::information(this, tr("Select Quiz"),
                                 tr("Please select a quiz to restore."));
        return;
    }
    const AdminOpResult r = AdminContentService::instance().restoreQuiz(qzid);
    if (!r.ok) {
        QMessageBox::critical(this, tr("Restore failed"), r.message);
        return;
    }
    refreshAll();
    emit operationCompleted(r.message);
}

void AdminContentMaintenanceWidget::onFilterQuizzes()
{
    loadQuizzes();
}
