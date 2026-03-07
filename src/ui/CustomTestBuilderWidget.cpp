#include "ui/CustomTestBuilderWidget.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QGroupBox>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────
CustomTestBuilderWidget::CustomTestBuilderWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &CustomTestBuilderWidget::applyTheme);
}

void CustomTestBuilderWidget::refresh()
{
    populateMyTests();
    m_innerStack->setCurrentIndex(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::setupUi()
{
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    m_innerStack = new QStackedWidget(this);
    setupMyTestsPage();
    setupBuilderPage();
    main->addWidget(m_innerStack);
}

void CustomTestBuilderWidget::setupMyTestsPage()
{
    QWidget* page = new QWidget(m_innerStack);
    page->setObjectName("myTestsPage");
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(16);

    // Header
    QHBoxLayout* hdr = new QHBoxLayout();
    QLabel* title = new QLabel("📁  My Custom Tests", page);
    title->setObjectName("sectionHeader");
    hdr->addWidget(title);
    hdr->addStretch();

    m_newTestBtn = new QPushButton("➕  New Test", page);
    m_newTestBtn->setObjectName("primaryButton");
    connect(m_newTestBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onNewTestClicked);
    hdr->addWidget(m_newTestBtn);

    m_importBtn = new QPushButton("📥  Import", page);
    m_importBtn->setObjectName("secondaryButton");
    connect(m_importBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onImportClicked);
    hdr->addWidget(m_importBtn);

    layout->addLayout(hdr);

    // Test list
    m_myTestsList = new QListWidget(page);
    m_myTestsList->setObjectName("myTestsList");
    m_myTestsList->setSpacing(4);
    m_myTestsList->setUniformItemSizes(false);
    layout->addWidget(m_myTestsList, 1);

    // Actions bar
    QHBoxLayout* actions = new QHBoxLayout();

    m_loadTestBtn = new QPushButton("▶  Launch Selected", page);
    m_loadTestBtn->setObjectName("primaryButton");
    m_loadTestBtn->setEnabled(false);
    connect(m_loadTestBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onLoadTestClicked);
    actions->addWidget(m_loadTestBtn);

    QPushButton* editBtn = new QPushButton("✏  Edit", page);
    editBtn->setObjectName("secondaryButton");
    connect(editBtn, &QPushButton::clicked, this, [this]() {
        QListWidgetItem* item = m_myTestsList->currentItem();
        if (!item) return;
        const int testId = item->data(Qt::UserRole).toInt();
        m_editingTestId  = testId;
        // Load questions for this test into builder
        const auto questions = m_repo.questionsForCustomTest(testId);
        m_selectedQuestions = questions;
        // Set title from test list
        for (const auto& t : m_myTests) {
            if (t.id == testId) {
                m_testTitleEdit->setText(t.title);
                m_testDescEdit->setText(t.description);
                break;
            }
        }
        refreshSelectedList();
        populateTopicTree();
        m_innerStack->setCurrentIndex(1);
    });
    actions->addWidget(editBtn);

    m_deleteTestBtn = new QPushButton("🗑  Delete", page);
    m_deleteTestBtn->setObjectName("dangerButton");
    m_deleteTestBtn->setEnabled(false);
    connect(m_deleteTestBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onDeleteTestClicked);
    actions->addWidget(m_deleteTestBtn);

    actions->addStretch();
    layout->addLayout(actions);

    // Enable/disable actions on selection change
    connect(m_myTestsList, &QListWidget::itemSelectionChanged, this, [this]() {
        const bool sel = m_myTestsList->currentItem() != nullptr;
        m_loadTestBtn->setEnabled(sel);
        m_deleteTestBtn->setEnabled(sel);
    });
    connect(m_myTestsList, &QListWidget::itemDoubleClicked, this, [this]() {
        onLoadTestClicked();
    });

    m_innerStack->addWidget(page);
}

void CustomTestBuilderWidget::setupBuilderPage()
{
    QWidget* page = new QWidget(m_innerStack);
    page->setObjectName("builderPage");
    QVBoxLayout* main = new QVBoxLayout(page);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    // ── Top area: browser (left) + selected (right) ───────────────────────
    QSplitter* splitter = new QSplitter(Qt::Horizontal, page);
    splitter->setHandleWidth(1);

    // Left: topic tree + question browser
    QWidget* leftPane = new QWidget(splitter);
    leftPane->setObjectName("builderLeft");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPane);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    QLabel* topicHdr = new QLabel("  Browse by Topic", leftPane);
    topicHdr->setObjectName("paneHeader");
    topicHdr->setFixedHeight(32);
    leftLayout->addWidget(topicHdr);

    m_topicTree = new QTreeWidget(leftPane);
    m_topicTree->setObjectName("topicTree");
    m_topicTree->setHeaderHidden(true);
    m_topicTree->setAnimated(true);
    m_topicTree->setMaximumHeight(160);
    connect(m_topicTree, &QTreeWidget::itemSelectionChanged,
            this, &CustomTestBuilderWidget::onTopicSelected);
    leftLayout->addWidget(m_topicTree);

    QLabel* qHdr = new QLabel("  Questions", leftPane);
    qHdr->setObjectName("paneHeader");
    qHdr->setFixedHeight(32);
    leftLayout->addWidget(qHdr);

    m_questionList = new QListWidget(leftPane);
    m_questionList->setObjectName("questionBrowser");
    m_questionList->setSpacing(2);
    m_questionList->setUniformItemSizes(false);
    leftLayout->addWidget(m_questionList, 1);

    m_addBtn = new QPushButton("➕  Add to Test", leftPane);
    m_addBtn->setObjectName("primaryButton");
    m_addBtn->setEnabled(false);
    connect(m_addBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onAddQuestionClicked);
    leftLayout->addWidget(m_addBtn);

    connect(m_questionList, &QListWidget::itemSelectionChanged, this, [this]() {
        m_addBtn->setEnabled(m_questionList->currentItem() != nullptr);
    });
    connect(m_questionList, &QListWidget::itemDoubleClicked, this, [this]() {
        onAddQuestionClicked();
    });

    splitter->addWidget(leftPane);
    splitter->setStretchFactor(0, 1);

    // Right: selected questions
    QWidget* rightPane = new QWidget(splitter);
    rightPane->setObjectName("builderRight");
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPane);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    QLabel* selHdr = new QLabel("  Selected Questions", rightPane);
    selHdr->setObjectName("paneHeader");
    selHdr->setFixedHeight(32);
    rightLayout->addWidget(selHdr);

    m_selectedList = new QListWidget(rightPane);
    m_selectedList->setObjectName("selectedList");
    m_selectedList->setSpacing(2);
    rightLayout->addWidget(m_selectedList, 1);

    // Move up/down/remove toolbar
    QHBoxLayout* orderBar = new QHBoxLayout();
    orderBar->setContentsMargins(8, 4, 8, 4);

    m_moveUpBtn = new QPushButton("↑", rightPane);
    m_moveUpBtn->setObjectName("iconButton");
    m_moveUpBtn->setFixedWidth(32);
    connect(m_moveUpBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onMoveUpClicked);
    orderBar->addWidget(m_moveUpBtn);

    m_moveDownBtn = new QPushButton("↓", rightPane);
    m_moveDownBtn->setObjectName("iconButton");
    m_moveDownBtn->setFixedWidth(32);
    connect(m_moveDownBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onMoveDownClicked);
    orderBar->addWidget(m_moveDownBtn);

    m_removeBtn = new QPushButton("🗑  Remove", rightPane);
    m_removeBtn->setObjectName("dangerButton");
    connect(m_removeBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onRemoveQuestionClicked);
    orderBar->addWidget(m_removeBtn);
    orderBar->addStretch();

    rightLayout->addLayout(orderBar);
    splitter->addWidget(rightPane);
    splitter->setStretchFactor(1, 1);

    main->addWidget(splitter, 1);

    // ── Bottom: Random Generator + Save/Launch/Export ─────────────────────
    QFrame* bottomBar = new QFrame(page);
    bottomBar->setObjectName("builderBottomBar");
    bottomBar->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* botLayout = new QHBoxLayout(bottomBar);
    botLayout->setContentsMargins(16, 10, 16, 10);
    botLayout->setSpacing(10);

    // Random generator group
    QLabel* genLabel = new QLabel("Random:", bottomBar);
    genLabel->setObjectName("toolLabel");
    botLayout->addWidget(genLabel);

    m_diffCombo = new QComboBox(bottomBar);
    m_diffCombo->setObjectName("difficultyCombo");
    m_diffCombo->addItem("Any Difficulty", 0);
    m_diffCombo->addItem("★ Beginner",     1);
    m_diffCombo->addItem("★★ Intermediate", 2);
    m_diffCombo->addItem("★★★ Advanced",    3);
    m_diffCombo->addItem("★★★★ Expert",     4);
    m_diffCombo->setFixedWidth(160);
    botLayout->addWidget(m_diffCombo);

    m_countSpin = new QSpinBox(bottomBar);
    m_countSpin->setObjectName("countSpin");
    m_countSpin->setRange(1, 50);
    m_countSpin->setValue(10);
    m_countSpin->setPrefix("Count: ");
    m_countSpin->setFixedWidth(110);
    botLayout->addWidget(m_countSpin);

    m_generateBtn = new QPushButton("🎲  Generate", bottomBar);
    m_generateBtn->setObjectName("secondaryButton");
    connect(m_generateBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onGenerateRandomClicked);
    botLayout->addWidget(m_generateBtn);

    // Separator
    QFrame* sep = new QFrame(bottomBar);
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Sunken);
    botLayout->addWidget(sep);

    // Title / Desc
    m_testTitleEdit = new QLineEdit(bottomBar);
    m_testTitleEdit->setObjectName("testTitleEdit");
    m_testTitleEdit->setPlaceholderText("Test title…");
    m_testTitleEdit->setFixedWidth(180);
    botLayout->addWidget(m_testTitleEdit);

    m_testDescEdit = new QLineEdit(bottomBar);
    m_testDescEdit->setObjectName("testDescEdit");
    m_testDescEdit->setPlaceholderText("Description (optional)…");
    m_testDescEdit->setFixedWidth(200);
    botLayout->addWidget(m_testDescEdit);

    // Action buttons
    m_saveBtn = new QPushButton("💾  Save", bottomBar);
    m_saveBtn->setObjectName("secondaryButton");
    connect(m_saveBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onSaveTestClicked);
    botLayout->addWidget(m_saveBtn);

    m_exportBtn = new QPushButton("📤  Export", bottomBar);
    m_exportBtn->setObjectName("secondaryButton");
    connect(m_exportBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onExportClicked);
    botLayout->addWidget(m_exportBtn);

    m_launchBtn = new QPushButton("▶  Launch", bottomBar);
    m_launchBtn->setObjectName("primaryButton");
    connect(m_launchBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onLaunchClicked);
    botLayout->addWidget(m_launchBtn);

    m_backBtn = new QPushButton("← My Tests", bottomBar);
    m_backBtn->setObjectName("quizNavButton");
    connect(m_backBtn, &QPushButton::clicked, this, [this]() {
        clearBuilder();
        populateMyTests();
        m_innerStack->setCurrentIndex(0);
    });
    botLayout->addWidget(m_backBtn);

    main->addWidget(bottomBar);
    m_innerStack->addWidget(page);
}

// ─────────────────────────────────────────────────────────────────────────────
// Data population
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::populateTopicTree()
{
    m_topicTree->clear();

    QTreeWidgetItem* allItem = new QTreeWidgetItem(m_topicTree);
    allItem->setText(0, "📋  All Topics");
    allItem->setData(0, Qt::UserRole, -1);

    const QList<TopicDTO> roots = m_repo.rootTopics();
    for (const auto& root : roots) {
        QTreeWidgetItem* item = new QTreeWidgetItem(m_topicTree);
        item->setText(0, QString("%1  %2").arg(root.icon, root.title));
        item->setData(0, Qt::UserRole, root.id);
    }
    m_topicTree->setCurrentItem(allItem);
}

void CustomTestBuilderWidget::populateQuestionBrowser(int topicId)
{
    m_questionList->clear();

    QList<QuestionDTO> questions;
    if (topicId < 0) {
        // All topics — gather from all quizzes
        const auto allTopics = m_repo.allTopics();
        for (const auto& t : allTopics) {
            const auto qs = m_repo.randomQuestions({t.id}, 100, 4);
            questions << qs;
        }
    } else {
        questions = m_repo.randomQuestions({topicId}, 100, 4);
    }

    // Deduplicate by id
    QSet<int> seen;
    for (const auto& q : questions) {
        if (seen.contains(q.id)) continue;
        seen.insert(q.id);

        QListWidgetItem* item = new QListWidgetItem(m_questionList);
        const QString diff = difficultyLabel(q.difficulty);
        item->setText(QString("[%1] %2").arg(diff, q.content.left(80)));
        item->setData(Qt::UserRole, q.id);
        item->setSizeHint(QSize(0, 44));
        item->setToolTip(q.content);
        m_questionList->addItem(item);
    }
}

void CustomTestBuilderWidget::populateMyTests()
{
    m_myTestsList->clear();
    const int userId = UserManager::instance().currentUser().id;
    m_myTests = m_repo.customTestsForUser(userId);

    if (m_myTests.isEmpty()) {
        QListWidgetItem* empty = new QListWidgetItem("No custom tests yet. Click '➕ New Test' to create one.");
        empty->setFlags(Qt::NoItemFlags);
        m_myTestsList->addItem(empty);
        return;
    }

    for (const auto& t : m_myTests) {
        QListWidgetItem* item = new QListWidgetItem(m_myTestsList);
        item->setText(QString("📝  %1\n    %2 questions  •  %3")
                      .arg(t.title)
                      .arg(t.questionCount)
                      .arg(t.description.isEmpty() ? "No description" : t.description));
        item->setData(Qt::UserRole, t.id);
        item->setSizeHint(QSize(0, 52));
        m_myTestsList->addItem(item);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::onTopicSelected()
{
    QTreeWidgetItem* item = m_topicTree->currentItem();
    if (!item) return;
    const int topicId = item->data(0, Qt::UserRole).toInt();
    populateQuestionBrowser(topicId);
}

void CustomTestBuilderWidget::onAddQuestionClicked()
{
    QListWidgetItem* item = m_questionList->currentItem();
    if (!item) return;
    const int qId = item->data(Qt::UserRole).toInt();
    if (qId <= 0) return;

    // Check not already added
    for (const auto& q : m_selectedQuestions)
        if (q.id == qId) return;

    QuestionDTO q = m_repo.questionById(qId);
    if (q.id < 0) return;
    addQuestionToSelected(q);
}

void CustomTestBuilderWidget::onRemoveQuestionClicked()
{
    const int row = m_selectedList->currentRow();
    if (row < 0 || row >= m_selectedQuestions.size()) return;
    m_selectedQuestions.removeAt(row);
    refreshSelectedList();
}

void CustomTestBuilderWidget::onMoveUpClicked()
{
    const int row = m_selectedList->currentRow();
    if (row <= 0 || row >= m_selectedQuestions.size()) return;
    m_selectedQuestions.swapItemsAt(row, row - 1);
    refreshSelectedList();
    m_selectedList->setCurrentRow(row - 1);
}

void CustomTestBuilderWidget::onMoveDownClicked()
{
    const int row = m_selectedList->currentRow();
    if (row < 0 || row >= m_selectedQuestions.size() - 1) return;
    m_selectedQuestions.swapItemsAt(row, row + 1);
    refreshSelectedList();
    m_selectedList->setCurrentRow(row + 1);
}

void CustomTestBuilderWidget::onGenerateRandomClicked()
{
    // Collect selected topics from tree (if any selected beyond "All")
    QList<int> topicIds;
    for (auto* item : m_topicTree->selectedItems()) {
        const int id = item->data(0, Qt::UserRole).toInt();
        if (id > 0) topicIds << id;
    }
    if (topicIds.isEmpty()) {
        // Use all topics
        for (const auto& t : m_repo.allTopics())
            topicIds << t.id;
    }

    const int count      = m_countSpin->value();
    const int maxDiff    = m_diffCombo->currentData().toInt();
    const int effectiveD = (maxDiff == 0) ? 4 : maxDiff;

    const auto questions = m_repo.randomQuestions(topicIds, count, effectiveD);
    if (questions.isEmpty()) {
        QMessageBox::information(this, "No Questions",
            "No questions found for the selected topics and difficulty.");
        return;
    }

    // Append (skip duplicates)
    for (const auto& q : questions) {
        bool dup = false;
        for (const auto& existing : m_selectedQuestions)
            if (existing.id == q.id) { dup = true; break; }
        if (!dup) m_selectedQuestions << q;
    }
    refreshSelectedList();
}

void CustomTestBuilderWidget::onSaveTestClicked()
{
    const QString title = m_testTitleEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Missing Title", "Please enter a title for the test.");
        return;
    }
    if (m_selectedQuestions.isEmpty()) {
        QMessageBox::warning(this, "No Questions", "Please add at least one question.");
        return;
    }

    const int userId = UserManager::instance().currentUser().id;
    const QString desc = m_testDescEdit->text().trimmed();

    // If editing existing test, delete old questions first
    if (m_editingTestId > 0) {
        m_repo.removeCustomTest(m_editingTestId);
    }

    const int testId = m_repo.createCustomTest(userId, title, desc);
    if (testId < 0) {
        QMessageBox::critical(this, "Save Failed", "Could not save the test to the database.");
        return;
    }

    for (int i = 0; i < m_selectedQuestions.size(); ++i)
        m_repo.addQuestionToCustomTest(testId, m_selectedQuestions[i].id, i);

    m_editingTestId = testId;
    QMessageBox::information(this, "Saved",
        QString("Test \"%1\" saved with %2 questions.")
        .arg(title).arg(m_selectedQuestions.size()));
}

void CustomTestBuilderWidget::onLaunchClicked()
{
    if (m_selectedQuestions.isEmpty()) {
        QMessageBox::warning(this, "No Questions", "Please add at least one question.");
        return;
    }
    emit launchCustomTest(m_selectedQuestions);
}

void CustomTestBuilderWidget::onExportClicked()
{
    if (m_selectedQuestions.isEmpty()) {
        QMessageBox::warning(this, "Nothing to Export", "Add questions before exporting.");
        return;
    }

    const QString defaultName = m_testTitleEdit->text().trimmed().isEmpty()
                                ? "custom_test"
                                : m_testTitleEdit->text().trimmed().replace(' ', '_');

    const QString path = QFileDialog::getSaveFileName(
        this, "Export Custom Test",
        defaultName + "." + QuizExporter::FILE_EXTENSION,
        QuizExporter::FILE_FILTER);
    if (path.isEmpty()) return;

    const bool ok = QuizExporter::exportToFile(
        m_selectedQuestions,
        m_testTitleEdit->text().trimmed(),
        m_testDescEdit->text().trimmed(),
        path);

    if (ok)
        QMessageBox::information(this, "Exported",
            QString("Test exported to:\n%1").arg(path));
    else
        QMessageBox::critical(this, "Export Failed",
            "Could not write the file. Check permissions.");
}

void CustomTestBuilderWidget::onImportClicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this, "Import Custom Test", QString(),
        QuizExporter::FILE_FILTER);
    if (path.isEmpty()) return;

    QString err;
    const auto questions = QuizExporter::importFromFile(path, &err);
    if (!err.isEmpty()) {
        QMessageBox::critical(this, "Import Failed", err);
        return;
    }

    // Switch to builder and populate
    clearBuilder();
    m_selectedQuestions = questions;
    refreshSelectedList();
    populateTopicTree();
    m_innerStack->setCurrentIndex(1);

    QMessageBox::information(this, "Imported",
        QString("Imported %1 questions.\nYou can now launch or save the test.")
        .arg(questions.size()));
}

void CustomTestBuilderWidget::onDeleteTestClicked()
{
    QListWidgetItem* item = m_myTestsList->currentItem();
    if (!item) return;
    const int testId = item->data(Qt::UserRole).toInt();

    const auto btn = QMessageBox::question(this, "Delete Test",
        "Are you sure you want to delete this test? This cannot be undone.");
    if (btn != QMessageBox::Yes) return;

    m_repo.removeCustomTest(testId);
    populateMyTests();
}

void CustomTestBuilderWidget::onLoadTestClicked()
{
    QListWidgetItem* item = m_myTestsList->currentItem();
    if (!item) return;
    const int testId = item->data(Qt::UserRole).toInt();

    const auto questions = m_repo.questionsForCustomTest(testId);
    if (questions.isEmpty()) {
        QMessageBox::information(this, "Empty Test",
            "This test has no questions. Edit it to add some.");
        return;
    }

    clearBuilder();
    m_selectedQuestions = questions;
    m_editingTestId     = testId;
    for (const auto& t : m_myTests) {
        if (t.id == testId) {
            m_testTitleEdit->setText(t.title);
            m_testDescEdit->setText(t.description);
            break;
        }
    }
    refreshSelectedList();
    populateTopicTree();
    m_innerStack->setCurrentIndex(1);
}

void CustomTestBuilderWidget::onNewTestClicked()
{
    clearBuilder();
    populateTopicTree();
    populateQuestionBrowser(-1);
    m_innerStack->setCurrentIndex(1);
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::addQuestionToSelected(const QuestionDTO& q)
{
    m_selectedQuestions << q;
    refreshSelectedList();
}

void CustomTestBuilderWidget::refreshSelectedList()
{
    m_selectedList->clear();
    for (int i = 0; i < m_selectedQuestions.size(); ++i) {
        const auto& q = m_selectedQuestions[i];
        QListWidgetItem* item = new QListWidgetItem(m_selectedList);
        item->setText(QString("%1. [%2] %3")
                      .arg(i + 1)
                      .arg(difficultyLabel(q.difficulty))
                      .arg(q.content.left(70)));
        item->setData(Qt::UserRole, q.id);
        item->setSizeHint(QSize(0, 40));
        item->setToolTip(q.content);
        m_selectedList->addItem(item);
    }
}

void CustomTestBuilderWidget::clearBuilder()
{
    m_selectedQuestions.clear();
    m_selectedList->clear();
    m_testTitleEdit->clear();
    m_testDescEdit->clear();
    m_editingTestId = -1;
}

QString CustomTestBuilderWidget::difficultyLabel(int d) const
{
    switch (d) {
    case 1: return "★";
    case 2: return "★★";
    case 3: return "★★★";
    case 4: return "★★★★";
    default: return "?";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        CustomTestBuilderWidget {
            background-color: %1;
        }
        #myTestsPage, #builderPage {
            background-color: %1;
        }
        #paneHeader {
            background-color: %2;
            color: %4;
            font-size: 12px;
            font-weight: bold;
            padding: 0 12px;
        }
        #sectionHeader {
            color: %4;
            font-size: 16px;
            font-weight: bold;
        }
        #myTestsList, #questionBrowser, #selectedList, #topicTree {
            background-color: %3;
            color: %4;
            border: 1px solid %5;
            border-radius: 4px;
        }
        #myTestsList::item:selected,
        #questionBrowser::item:selected,
        #selectedList::item:selected,
        #topicTree::item:selected {
            background-color: %6;
            color: white;
        }
        #primaryButton {
            background-color: %6;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
        }
        #primaryButton:hover { background-color: %7; }
        #secondaryButton {
            background-color: transparent;
            color: %4;
            border: 1px solid %5;
            border-radius: 4px;
            padding: 6px 14px;
            font-size: 13px;
        }
        #secondaryButton:hover { background-color: %8; }
        #dangerButton {
            background-color: transparent;
            color: %9;
            border: 1px solid %9;
            border-radius: 4px;
            padding: 6px 14px;
            font-size: 13px;
        }
        #dangerButton:hover { background-color: %9; color: white; }
        #quizNavButton {
            background-color: transparent;
            color: %4;
            border: 1px solid %5;
            border-radius: 4px;
            padding: 5px 12px;
        }
        #builderBottomBar {
            background-color: %2;
            border-top: 1px solid %5;
        }
        #testTitleEdit, #testDescEdit, #difficultyCombo, #countSpin {
            background-color: %3;
            color: %4;
            border: 1px solid %5;
            border-radius: 3px;
            padding: 4px 8px;
        }
        #iconButton {
            background-color: transparent;
            color: %4;
            border: 1px solid %5;
            border-radius: 3px;
        }
        #toolLabel {
            color: %10;
            font-size: 12px;
        }
    )")
    .arg(t.windowBackground.name())     // %1
    .arg(t.toolbarBackground.name())    // %2
    .arg(t.editorBackground.name())     // %3
    .arg(t.textPrimary.name())          // %4
    .arg(t.border.name())               // %5
    .arg(t.accent.name())               // %6
    .arg(t.accent.darker(120).name())   // %7
    .arg(t.accent.darker(160).name())   // %8
    .arg(t.error.name())                // %9
    .arg(t.textSecondary.name())        // %10
    );
}