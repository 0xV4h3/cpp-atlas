#include "ui/CustomTestBuilderWidget.h"
#include "ui/ThemeManager.h"
#include "quiz/UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QSet>
#include <QStringListModel>

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
    emit subPageChanged(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::setupUi()
{
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    m_innerStack = new QStackedWidget(this);
    setupMyTestsPage();
    setupBuilderPage();
    main->addWidget(m_innerStack);
}

void CustomTestBuilderWidget::setupMyTestsPage()
{
    auto* page = new QWidget(m_innerStack);
    page->setObjectName("myTestsPage");
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(16);

    // Header row
    auto* hdr = new QHBoxLayout();
    auto* titleLbl = new QLabel("📁  My Custom Tests", page);
    titleLbl->setObjectName("sectionHeader");
    hdr->addWidget(titleLbl);
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

    // List
    m_myTestsList = new QListWidget(page);
    m_myTestsList->setObjectName("myTestsList");
    m_myTestsList->setSpacing(4);
    m_myTestsList->setUniformItemSizes(false);
    layout->addWidget(m_myTestsList, 1);

    // Bottom actions bar
    auto* actions = new QHBoxLayout();

    m_loadTestBtn = new QPushButton("▶  Launch Selected", page);
    m_loadTestBtn->setObjectName("primaryButton");
    m_loadTestBtn->setEnabled(false);
    connect(m_loadTestBtn, &QPushButton::clicked, this, &CustomTestBuilderWidget::onLoadTestClicked);
    actions->addWidget(m_loadTestBtn);

    m_editTestBtn = new QPushButton("✏  Edit", page);
    m_editTestBtn->setObjectName("secondaryButton");
    m_editTestBtn->setEnabled(false);
    connect(m_editTestBtn, &QPushButton::clicked, this, [this]() {
        QListWidgetItem* item = m_myTestsList->currentItem();
        if (!item) return;
        const int testId = item->data(Qt::UserRole).toInt();
        m_editingTestId  = testId;
        const auto questions = m_repo.questionsForCustomTest(testId);
        m_selectedQuestions = questions;
        for (const auto& t : m_myTests) {
            if (t.id == testId) {
                m_testTitleEdit->setText(t.title);
                m_testDescEdit->setText(t.description);
                m_originalTitle = t.title;
                m_originalDesc  = t.description;
                break;
            }
        }
        m_originalQuestionIds.clear();
        for (const auto& q : m_selectedQuestions)
            m_originalQuestionIds << q.id;
        m_hasChanges = false;
        refreshSelectedList();
        populateTopicTree();
        populateQuestionBrowser(-1);
        updateSaveButtonStates();
        m_innerStack->setCurrentIndex(1);
        emit subPageChanged(1);
    });
    actions->addWidget(m_editTestBtn);

    m_exportMyTestBtn = new QPushButton("📤  Export", page);
    m_exportMyTestBtn->setObjectName("secondaryButton");
    m_exportMyTestBtn->setEnabled(false);
    connect(m_exportMyTestBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onExportSelectedTestClicked);
    actions->addWidget(m_exportMyTestBtn);

    m_deleteTestBtn = new QPushButton("🗑  Delete", page);
    m_deleteTestBtn->setObjectName("dangerButton");
    m_deleteTestBtn->setEnabled(false);
    connect(m_deleteTestBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onDeleteTestClicked);
    actions->addWidget(m_deleteTestBtn);

    actions->addStretch();
    layout->addLayout(actions);

    // Enable/disable actions on selection change
    connect(m_myTestsList, &QListWidget::itemSelectionChanged, this, [this]() {
        const bool sel = !m_myTestsList->selectedItems().isEmpty();
        m_loadTestBtn->setEnabled(sel);
        m_editTestBtn->setEnabled(sel);
        m_exportMyTestBtn->setEnabled(sel);
        m_deleteTestBtn->setEnabled(sel);
    });
    connect(m_myTestsList, &QListWidget::itemDoubleClicked,
            this, [this]() { onLoadTestClicked(); });

    // Allow deselection by clicking on empty area
    m_myTestsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_myTestsList->installEventFilter(this);

    m_innerStack->addWidget(page);
}

void CustomTestBuilderWidget::setupBuilderPage()
{
    auto* page = new QWidget(m_innerStack);
    page->setObjectName("builderPage");
    auto* mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Main splitter (left | right) ─────────────────────────────────────────
    auto* splitter = new QSplitter(Qt::Horizontal, page);
    splitter->setHandleWidth(1);

    // ── LEFT PANE ────────────────────────────────────────────────────────────
    auto* leftPane = new QWidget(splitter);
    leftPane->setObjectName("builderLeft");
    auto* leftLayout = new QVBoxLayout(leftPane);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    // Topic header
    auto* topicHdr = new QLabel("  Browse by Topic", leftPane);
    topicHdr->setObjectName("paneHeader");
    topicHdr->setFixedHeight(32);
    leftLayout->addWidget(topicHdr);

    // Vertical sub-splitter: topic tree (top) / question browser (bottom)
    auto* leftSplit = new QSplitter(Qt::Vertical, leftPane);
    leftSplit->setHandleWidth(4);

    m_topicTree = new QTreeWidget(leftSplit);
    m_topicTree->setObjectName("topicTree");
    m_topicTree->setHeaderHidden(true);
    m_topicTree->setAnimated(true);
    m_topicTree->setMinimumHeight(80);
    connect(m_topicTree, &QTreeWidget::itemSelectionChanged,
            this, &CustomTestBuilderWidget::onTopicSelected);

    // Question browser + search
    auto* qPane = new QWidget(leftSplit);
    auto* qPaneLayout = new QVBoxLayout(qPane);
    qPaneLayout->setContentsMargins(0, 0, 0, 0);
    qPaneLayout->setSpacing(0);

    auto* qHdr = new QLabel("  Questions", qPane);
    qHdr->setObjectName("paneHeader");
    qHdr->setFixedHeight(32);
    qPaneLayout->addWidget(qHdr);

    // Split search bar: title + tag
    auto* searchRow = new QHBoxLayout();
    searchRow->setContentsMargins(4, 4, 4, 2);
    searchRow->setSpacing(4);

    m_questionTitleSearch = new QLineEdit(qPane);
    m_questionTitleSearch->setObjectName("titleSearch");
    m_questionTitleSearch->setPlaceholderText("Search by title…");
    connect(m_questionTitleSearch, &QLineEdit::textChanged,
            this, &CustomTestBuilderWidget::onQuestionSearchChanged);
    searchRow->addWidget(m_questionTitleSearch, 2);

    m_questionTagSearch = new QLineEdit(qPane);
    m_questionTagSearch->setObjectName("tagSearch");
    m_questionTagSearch->setPlaceholderText("Tag…");
    m_questionTagSearch->setFixedWidth(100);
    connect(m_questionTagSearch, &QLineEdit::textChanged,
            this, &CustomTestBuilderWidget::onQuestionSearchChanged);
    searchRow->addWidget(m_questionTagSearch);

    // Tag completer from DB
    m_questionTagCompleter = new QCompleter(this);
    m_questionTagCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_questionTagCompleter->setFilterMode(Qt::MatchContains);
    m_questionTagSearch->setCompleter(m_questionTagCompleter);
    {
        QStringList tagNames;
        for (const auto& t : m_repo.allTags()) tagNames << t.name;
        m_questionTagCompleter->setModel(new QStringListModel(tagNames, m_questionTagCompleter));
    }
    // Lazily theme completer popup on first keypress
    connect(m_questionTagSearch, &QLineEdit::textChanged, this, [this](const QString&) {
        applyCompleterTheme();
    });

    qPaneLayout->addLayout(searchRow);

    m_questionList = new QListWidget(qPane);
    m_questionList->setObjectName("questionBrowser");
    m_questionList->setSpacing(0);
    m_questionList->setWordWrap(true);
    m_questionList->setUniformItemSizes(false);
    qPaneLayout->addWidget(m_questionList, 1);

    // Add / Add All
    auto* addBar = new QHBoxLayout();
    addBar->setContentsMargins(4, 4, 4, 4);
    addBar->setSpacing(6);

    m_addBtn = new QPushButton("➕  Add to Test", qPane);
    m_addBtn->setObjectName("primaryButton");
    m_addBtn->setEnabled(false);
    connect(m_addBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onAddQuestionClicked);
    addBar->addWidget(m_addBtn, 1);

    m_addAllBtn = new QPushButton("➕➕  Add All", qPane);
    m_addAllBtn->setObjectName("secondaryButton");
    connect(m_addAllBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onAddAllClicked);
    addBar->addWidget(m_addAllBtn);

    qPaneLayout->addLayout(addBar);
    leftSplit->addWidget(m_topicTree);
    leftSplit->addWidget(qPane);
    leftSplit->setStretchFactor(0, 1);
    leftSplit->setStretchFactor(1, 3);

    leftLayout->addWidget(leftSplit, 1);

    splitter->addWidget(leftPane);
    splitter->setStretchFactor(0, 1);

    // ── RIGHT PANE ───────────────────────────────────────────────────────────
    auto* rightPane = new QWidget(splitter);
    rightPane->setObjectName("builderRight");
    auto* rightLayout = new QVBoxLayout(rightPane);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    auto* selHdr = new QLabel("  Selected Questions", rightPane);
    selHdr->setObjectName("paneHeader");
    selHdr->setFixedHeight(32);
    rightLayout->addWidget(selHdr);

    m_selectedList = new QListWidget(rightPane);
    m_selectedList->setObjectName("selectedList");
    m_selectedList->setWordWrap(true);
    m_selectedList->setUniformItemSizes(false);
    rightLayout->addWidget(m_selectedList, 1);

    // Order / Remove toolbar
    auto* orderBar = new QHBoxLayout();
    orderBar->setContentsMargins(8, 4, 8, 4);
    orderBar->setSpacing(6);

    m_moveUpBtn = new QPushButton("▲", rightPane);
    m_moveUpBtn->setObjectName("moveButton");
    m_moveUpBtn->setFixedWidth(32);
    connect(m_moveUpBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onMoveUpClicked);
    orderBar->addWidget(m_moveUpBtn);

    m_moveDownBtn = new QPushButton("▼", rightPane);
    m_moveDownBtn->setObjectName("moveButton");
    m_moveDownBtn->setFixedWidth(32);
    connect(m_moveDownBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onMoveDownClicked);
    orderBar->addWidget(m_moveDownBtn);

    m_removeBtn = new QPushButton("🗑  Remove", rightPane);
    m_removeBtn->setObjectName("dangerButton");
    connect(m_removeBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onRemoveQuestionClicked);
    orderBar->addWidget(m_removeBtn);

    m_removeAllBtn = new QPushButton("🗑  Remove All", rightPane);
    m_removeAllBtn->setObjectName("dangerButton");
    connect(m_removeAllBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onRemoveAllClicked);
    orderBar->addWidget(m_removeAllBtn);

    orderBar->addStretch();
    rightLayout->addLayout(orderBar);

    splitter->addWidget(rightPane);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter, 1);

    // ── BOTTOM BAR ───────────────────────────────────────────────────────────
    auto* bottomBar = new QFrame(page);
    bottomBar->setObjectName("builderBottomBar");
    bottomBar->setFrameShape(QFrame::NoFrame);
    auto* botLayout = new QHBoxLayout(bottomBar);
    botLayout->setContentsMargins(16, 8, 16, 8);
    botLayout->setSpacing(8);

    // Random generator
    auto* genLbl = new QLabel("Random:", bottomBar);
    genLbl->setObjectName("toolLabel");
    botLayout->addWidget(genLbl);

    m_diffCombo = new QComboBox(bottomBar);
    m_diffCombo->setObjectName("difficultyCombo");
    m_diffCombo->addItem("Any Difficulty", 0);
    m_diffCombo->addItem("★ Beginner",     1);
    m_diffCombo->addItem("★★ Intermediate", 2);
    m_diffCombo->addItem("★★★ Advanced",   3);
    m_diffCombo->addItem("★★★★ Expert",    4);
    m_diffCombo->setFixedWidth(155);
    botLayout->addWidget(m_diffCombo);

    m_countSpin = new QSpinBox(bottomBar);
    m_countSpin->setObjectName("countSpin");
    m_countSpin->setRange(1, 50);
    m_countSpin->setValue(10);
    m_countSpin->setPrefix("Count: ");
    m_countSpin->setFixedWidth(110);
    botLayout->addWidget(m_countSpin);

    m_generateBtn = new QPushButton("🎲  Generate", bottomBar);
    m_generateBtn->setObjectName("generateButton");
    connect(m_generateBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onGenerateRandomClicked);
    botLayout->addWidget(m_generateBtn);

    // Separator
    auto* sep = new QFrame(bottomBar);
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Sunken);
    botLayout->addWidget(sep);

    // Title / Desc
    m_testTitleEdit = new QLineEdit(bottomBar);
    m_testTitleEdit->setObjectName("testTitleEdit");
    m_testTitleEdit->setPlaceholderText("Test title…");
    m_testTitleEdit->setFixedWidth(175);
    botLayout->addWidget(m_testTitleEdit);

    m_testDescEdit = new QLineEdit(bottomBar);
    m_testDescEdit->setObjectName("testDescEdit");
    m_testDescEdit->setPlaceholderText("Description (optional)…");
    m_testDescEdit->setFixedWidth(195);
    botLayout->addWidget(m_testDescEdit);

    // Save (label set dynamically in updateSaveButtonStates)
    m_saveBtn = new QPushButton("💾  Save", bottomBar);
    m_saveBtn->setObjectName("secondaryButton");
    m_saveBtn->setEnabled(false);
    connect(m_saveBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onSaveTestClicked);
    botLayout->addWidget(m_saveBtn);

    // Save As — only relevant for Edit mode; hidden otherwise
    m_saveAsBtn = new QPushButton("💾  Save As New", bottomBar);
    m_saveAsBtn->setObjectName("secondaryButton");
    m_saveAsBtn->setEnabled(false);
    m_saveAsBtn->setVisible(false);
    connect(m_saveAsBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onSaveAsClicked);
    botLayout->addWidget(m_saveAsBtn);

    // Launch
    m_launchBtn = new QPushButton("▶  Launch", bottomBar);
    m_launchBtn->setObjectName("primaryButton");
    connect(m_launchBtn, &QPushButton::clicked,
            this, &CustomTestBuilderWidget::onLaunchClicked);
    botLayout->addWidget(m_launchBtn);

    mainLayout->addWidget(bottomBar);
    m_innerStack->addWidget(page);

    // ── Change-detection wiring ───────────────────────────────────────────────
    auto markChanged = [this]() {
        m_hasChanges = true;
        updateSaveButtonStates();
    };
    connect(m_testTitleEdit, &QLineEdit::textChanged, this, [this, markChanged](const QString&) {
        markChanged();
    });
    connect(m_testDescEdit, &QLineEdit::textChanged, this, [this, markChanged](const QString&) {
        markChanged();
    });

    // Enable add button on selection
    connect(m_questionList, &QListWidget::itemSelectionChanged, this, [this]() {
        m_addBtn->setEnabled(!m_questionList->selectedItems().isEmpty());
    });
    connect(m_questionList, &QListWidget::itemDoubleClicked, this, [this]() {
        onAddQuestionClicked();
    });

    // Enable remove/move buttons on selection
    connect(m_selectedList, &QListWidget::itemSelectionChanged, this, [this]() {
        const bool hasSel = !m_selectedList->selectedItems().isEmpty();
        m_removeBtn->setEnabled(hasSel);
        m_moveUpBtn->setEnabled(hasSel && m_selectedList->currentRow() > 0);
        m_moveDownBtn->setEnabled(hasSel && m_selectedList->currentRow() < m_selectedList->count() - 1);
    });
    m_removeBtn->setEnabled(false);
    m_moveUpBtn->setEnabled(false);
    m_moveDownBtn->setEnabled(false);

    // Allow deselection by clicking empty area
    m_questionList->installEventFilter(this);
    m_selectedList->installEventFilter(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// Data population
// ─────────────��───────────────────────────────────────────────────────────────
void CustomTestBuilderWidget::populateTopicTree()
{
    m_topicTree->clear();

    auto* allItem = new QTreeWidgetItem(m_topicTree);
    allItem->setText(0, "📋  All Topics");
    allItem->setData(0, Qt::UserRole, -1);

    // Get question counts per topic
    for (const auto& root : m_repo.rootTopics()) {
        const auto qs = m_repo.questionsForQuiz(
            m_repo.quizzesByTopic(root.id).isEmpty()
                ? -1
                : m_repo.quizzesByTopic(root.id).first().id);
        // Use randomQuestions with large limit to count
        const int count = m_repo.randomQuestions({root.id}, 1000, 4).size();
        auto* item = new QTreeWidgetItem(m_topicTree);
        item->setText(0, QString("%1  %2  (%3)")
                             .arg(root.icon, root.title)
                             .arg(count));
        item->setData(0, Qt::UserRole, root.id);
    }
    m_topicTree->setCurrentItem(allItem);
}

void CustomTestBuilderWidget::populateQuestionBrowser(int topicId)
{
    m_questionList->clear();

    const QString titleF = m_questionTitleSearch ? m_questionTitleSearch->text().trimmed() : QString();
    const QString tagF   = m_questionTagSearch   ? m_questionTagSearch->text().trimmed()   : QString();

    QList<QuestionDTO> questions;
    if (topicId < 0) {
        for (const auto& t : m_repo.allTopics()) {
            const auto qs = m_repo.randomQuestions({t.id}, 200, 4);
            questions << qs;
        }
    } else {
        questions = m_repo.randomQuestions({topicId}, 200, 4);
    }

    // Stable sort by id so list is consistent
    std::sort(questions.begin(), questions.end(),
              [](const QuestionDTO& a, const QuestionDTO& b) { return a.id < b.id; });

    // Deduplicate + filter
    QSet<int> seen;
    for (const auto& q : questions) {
        if (seen.contains(q.id)) continue;
        seen.insert(q.id);

        // Title filter (partial match on content)
        if (!titleF.isEmpty() && !q.content.contains(titleF, Qt::CaseInsensitive))
            continue;

        // Tag filter (partial match on any tag)
        if (!tagF.isEmpty()) {
            bool matchTag = false;
            for (const auto& tag : q.tags) {
                if (tag.contains(tagF, Qt::CaseInsensitive)) { matchTag = true; break; }
            }
            if (!matchTag) continue;
        }

        auto* item = new QListWidgetItem(m_questionList);
        const QString typeBadge = questionTypeLabel(q.type);
        const QString diff      = difficultyLabel(q.difficulty);
        item->setText(QString("[%1] %2  %3").arg(diff, typeBadge, q.content));
        item->setData(Qt::UserRole, q.id);
        item->setData(Qt::UserRole + 1, q.type);
        item->setSizeHint(QSize(0, 48));
        item->setToolTip(q.content + (q.codeSnippet.isEmpty() ? "" : "\n\n" + q.codeSnippet));
        m_questionList->addItem(item);
    }

    updateAddAllState();
}

void CustomTestBuilderWidget::populateMyTests()
{
    m_myTestsList->clear();
    const int userId = UserManager::instance().currentUser().id;
    m_myTests = m_repo.customTestsForUser(userId);

    if (m_myTests.isEmpty()) {
        auto* empty = new QListWidgetItem(
            "No custom tests yet. Click '➕ New Test' to create one.");
        empty->setFlags(Qt::NoItemFlags);
        m_myTestsList->addItem(empty);
        return;
    }

    for (const auto& t : m_myTests) {
        auto* item = new QListWidgetItem(m_myTestsList);
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
    auto* item = m_topicTree->currentItem();
    if (!item) return;
    const int topicId = item->data(0, Qt::UserRole).toInt();
    populateQuestionBrowser(topicId);
}

void CustomTestBuilderWidget::onQuestionSearchChanged()
{
    auto* item = m_topicTree->currentItem();
    const int topicId = item ? item->data(0, Qt::UserRole).toInt() : -1;
    populateQuestionBrowser(topicId);
}

void CustomTestBuilderWidget::onAddQuestionClicked()
{
    auto* item = m_questionList->currentItem();
    if (!item) return;
    const int qId = item->data(Qt::UserRole).toInt();
    if (qId <= 0) return;
    for (const auto& q : m_selectedQuestions)
        if (q.id == qId) return;
    QuestionDTO q = m_repo.questionById(qId);
    if (q.id < 0) return;
    addQuestionToSelected(q);
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onAddAllClicked()
{
    QSet<int> existing;
    for (const auto& q : m_selectedQuestions)
        existing.insert(q.id);

    for (int i = 0; i < m_questionList->count(); ++i) {
        auto* item = m_questionList->item(i);
        if (!item) continue;
        const int qId = item->data(Qt::UserRole).toInt();
        if (qId <= 0 || existing.contains(qId)) continue;
        QuestionDTO q = m_repo.questionById(qId);
        if (q.id < 0) continue;
        m_selectedQuestions << q;
        existing.insert(qId);
    }
    refreshSelectedList();
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onRemoveQuestionClicked()
{
    const int row = m_selectedList->currentRow();
    if (row < 0 || row >= m_selectedQuestions.size()) return;
    m_selectedQuestions.removeAt(row);
    refreshSelectedList();
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onRemoveAllClicked()
{
    if (m_selectedQuestions.isEmpty()) return;
    const auto btn = QMessageBox::question(this, "Remove All",
                                           "Remove all selected questions?");
    if (btn != QMessageBox::Yes) return;
    m_selectedQuestions.clear();
    refreshSelectedList();
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onMoveUpClicked()
{
    const int row = m_selectedList->currentRow();
    if (row <= 0 || row >= m_selectedQuestions.size()) return;
    m_selectedQuestions.swapItemsAt(row, row - 1);
    refreshSelectedList();
    m_selectedList->setCurrentRow(row - 1);
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onMoveDownClicked()
{
    const int row = m_selectedList->currentRow();
    if (row < 0 || row >= m_selectedQuestions.size() - 1) return;
    m_selectedQuestions.swapItemsAt(row, row + 1);
    refreshSelectedList();
    m_selectedList->setCurrentRow(row + 1);
    m_hasChanges = true;
    updateSaveButtonStates();
}

void CustomTestBuilderWidget::onGenerateRandomClicked()
{
    // Determine topic scope
    QList<int> topicIds;
    for (auto* item : m_topicTree->selectedItems()) {
        const int id = item->data(0, Qt::UserRole).toInt();
        if (id > 0) topicIds << id;
    }
    if (topicIds.isEmpty()) {
        for (const auto& t : m_repo.allTopics())
            topicIds << t.id;
    }

    const int requestedCount = m_countSpin->value();
    const int maxDiff = m_diffCombo->currentData().toInt();
    const int effectiveD = (maxDiff == 0) ? 4 : maxDiff;

    // Build set of already-selected IDs — we need exactly `requestedCount` NEW ones
    QSet<int> alreadySelected;
    for (const auto& q : m_selectedQuestions)
        alreadySelected.insert(q.id);

    // Request more than needed to compensate for duplicates;
    // repeat until we have enough or exhaust the pool.
    QList<QuestionDTO> newOnes;
    int attempts = 0;
    const int maxAttempts = 5;

    while (newOnes.size() < requestedCount && attempts < maxAttempts) {
        // Ask for (requestedCount - newOnes.size()) * 3 to reduce round trips
        const int fetchCount = (requestedCount - newOnes.size()) * 3;
        const auto candidates = m_repo.randomQuestions(topicIds, fetchCount, effectiveD);

        bool anyNew = false;
        for (const auto& q : candidates) {
            if (alreadySelected.contains(q.id)) continue;
            // Also skip if already in newOnes
            bool inNew = false;
            for (const auto& n : newOnes)
                if (n.id == q.id) { inNew = true; break; }
            if (inNew) continue;

            newOnes << q;
            alreadySelected.insert(q.id);
            anyNew = true;
            if (newOnes.size() >= requestedCount) break;
        }
        if (!anyNew) break; // pool exhausted
        ++attempts;
    }

    if (newOnes.isEmpty()) {
        QMessageBox::information(this, "No New Questions",
                                 "No additional questions found for the selected topics and difficulty.\n"
                                 "Try changing the topic or difficulty filter.");
        return;
    }

    for (const auto& q : newOnes)
        m_selectedQuestions << q;

    refreshSelectedList();
    m_hasChanges = true;
    updateSaveButtonStates();
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

    if (m_editingTestId > 0) {
        // Save Changes: remove old, recreate with same id logic (delete + insert)
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
    // Update snapshot
    m_originalTitle = title;
    m_originalDesc  = desc;
    m_originalQuestionIds.clear();
    for (const auto& q : m_selectedQuestions)
        m_originalQuestionIds << q.id;
    m_hasChanges = false;
    updateSaveButtonStates();

    QMessageBox::information(this, "Saved",
                             QString("Test \"%1\" saved (%2 questions).").arg(title).arg(m_selectedQuestions.size()));
}

void CustomTestBuilderWidget::onSaveAsClicked()
{
    // Require a *new* title different from the original
    const QString currentTitle = m_testTitleEdit->text().trimmed();
    if (currentTitle.isEmpty()) {
        QMessageBox::warning(this, "Missing Title",
                             "Please enter a new title for the copy.");
        return;
    }
    if (currentTitle == m_originalTitle) {
        QMessageBox::warning(this, "Title Unchanged",
                             "Please change the title to save as a new test.\n"
                             "The new title must differ from the original.");
        return;
    }
    if (m_selectedQuestions.isEmpty()) {
        QMessageBox::warning(this, "No Questions", "Please add at least one question.");
        return;
    }

    const int userId = UserManager::instance().currentUser().id;
    const QString desc = m_testDescEdit->text().trimmed();
    const int testId = m_repo.createCustomTest(userId, currentTitle, desc);
    if (testId < 0) {
        QMessageBox::critical(this, "Save Failed", "Could not save the new test.");
        return;
    }

    for (int i = 0; i < m_selectedQuestions.size(); ++i)
        m_repo.addQuestionToCustomTest(testId, m_selectedQuestions[i].id, i);

    QMessageBox::information(this, "Saved As New",
                             QString("New test \"%1\" created (%2 questions).")
                                 .arg(currentTitle).arg(m_selectedQuestions.size()));
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
    // Export current builder selection
    if (m_selectedQuestions.isEmpty()) {
        QMessageBox::warning(this, "Nothing to Export", "Add questions before exporting.");
        return;
    }

    const QString defaultName = m_testTitleEdit->text().trimmed().isEmpty()
                                    ? "custom_test"
                                    : m_testTitleEdit->text().trimmed().replace(' ', '_');

    QString path = QFileDialog::getSaveFileName(
        this, "Export Custom Test",
        defaultName + ".json",
        QuizExporter::FILE_FILTER);
    if (path.isEmpty()) return;

    // Auto-append .json if missing
    path = QuizExporter::ensureJsonExtension(path);

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

void CustomTestBuilderWidget::onExportSelectedTestClicked()
{
    // Export a saved test from My Tests page
    auto* item = m_myTestsList->currentItem();
    if (!item) return;
    const int testId = item->data(Qt::UserRole).toInt();
    if (testId <= 0) return;

    const auto questions = m_repo.questionsForCustomTest(testId);
    if (questions.isEmpty()) {
        QMessageBox::information(this, "Empty Test",
                                 "This test has no questions and cannot be exported.");
        return;
    }

    // Retrieve title/description from cached list
    QString testTitle, testDesc;
    for (const auto& t : m_myTests) {
        if (t.id == testId) {
            testTitle = t.title;
            testDesc  = t.description;
            break;
        }
    }

    const QString defaultName = testTitle.isEmpty()
                                    ? "custom_test"
                                    : testTitle.replace(' ', '_');

    QString path = QFileDialog::getSaveFileName(
        this, "Export Custom Test",
        defaultName + ".json",
        QuizExporter::FILE_FILTER);
    if (path.isEmpty()) return;

    path = QuizExporter::ensureJsonExtension(path);

    const bool ok = QuizExporter::exportToFile(questions, testTitle, testDesc, path);
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

    // Restore title/description from JSON header
    QString importedTitle, importedDesc;
    QuizExporter::readHeader(path, importedTitle, importedDesc);

    clearBuilder();
    m_selectedQuestions = questions;
    if (!importedTitle.isEmpty())
        m_testTitleEdit->setText(importedTitle);
    if (!importedDesc.isEmpty())
        m_testDescEdit->setText(importedDesc);
    refreshSelectedList();
    populateTopicTree();
    populateQuestionBrowser(-1);
    m_hasChanges = true;
    updateSaveButtonStates();
    m_innerStack->setCurrentIndex(1);
    emit subPageChanged(1);

    QMessageBox::information(this, "Imported",
                             QString("Imported %1 questions from \"%2\".\nYou can now launch or save the test.")
                                 .arg(questions.size())
                                 .arg(importedTitle.isEmpty() ? "file" : importedTitle));
}

void CustomTestBuilderWidget::onDeleteTestClicked()
{
    auto* item = m_myTestsList->currentItem();
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
    auto* item = m_myTestsList->currentItem();
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
    m_editingTestId     = -1;  // launch mode — not editing
    for (const auto& t : m_myTests) {
        if (t.id == testId) {
            m_testTitleEdit->setText(t.title);
            m_testDescEdit->setText(t.description);
            break;
        }
    }
    emit launchCustomTest(m_selectedQuestions);
}

void CustomTestBuilderWidget::onNewTestClicked()
{
    clearBuilder();
    populateTopicTree();
    populateQuestionBrowser(-1);
    m_hasChanges = false;
    updateSaveButtonStates();
    m_innerStack->setCurrentIndex(1);
    emit subPageChanged(1);
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
        auto* item = new QListWidgetItem(m_selectedList);
        const QString typeBadge = questionTypeLabel(q.type);
        item->setText(QString("%1. [%2] %3  %4")
                          .arg(i + 1)
                          .arg(difficultyLabel(q.difficulty))
                          .arg(typeBadge)
                          .arg(q.content));
        item->setData(Qt::UserRole, q.id);
        item->setSizeHint(QSize(0, 44));
        item->setToolTip(q.content);
        m_selectedList->addItem(item);
    }
    if (m_removeAllBtn)
        m_removeAllBtn->setEnabled(!m_selectedQuestions.isEmpty());
    updateAddAllState();
}

void CustomTestBuilderWidget::clearBuilder()
{
    m_selectedQuestions.clear();
    m_selectedList->clear();
    m_testTitleEdit->clear();
    m_testDescEdit->clear();
    m_editingTestId = -1;
    m_hasChanges    = false;
    m_originalQuestionIds.clear();
    m_originalTitle.clear();
    m_originalDesc.clear();
}

void CustomTestBuilderWidget::updateSaveButtonStates()
{
    const bool hasQuestions = !m_selectedQuestions.isEmpty();
    const bool hasTitle     = !m_testTitleEdit->text().trimmed().isEmpty();
    const bool isEditing    = (m_editingTestId > 0);

    if (isEditing) {
        // "Save Changes" label
        m_saveBtn->setText("💾  Save Changes");
        m_saveBtn->setEnabled(m_hasChanges && hasQuestions && hasTitle);

        // "Save As New" — visible only in edit mode
        m_saveAsBtn->setVisible(true);
        m_saveAsBtn->setEnabled(m_hasChanges && hasQuestions && hasTitle);
    } else {
        // New test — plain "Save"
        m_saveBtn->setText("💾  Save");
        m_saveBtn->setEnabled(hasQuestions && hasTitle);
        m_saveAsBtn->setVisible(false);
        m_saveAsBtn->setEnabled(false);
    }
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

QString CustomTestBuilderWidget::questionTypeLabel(const QString& type) const
{
    if (type == "mcq")          return "[MCQ]";
    if (type == "multi_select") return "[Multi]";
    if (type == "true_false")   return "[T/F]";
    if (type == "code_output")  return "[Code]";
    if (type == "fill_blank")   return "[Fill]";
    return "[?]";
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────

bool CustomTestBuilderWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto* me = dynamic_cast<QMouseEvent*>(event);
        if (me) {
            if (obj == m_myTestsList && !m_myTestsList->itemAt(me->pos()))
                m_myTestsList->clearSelection();
            else if (obj == m_questionList && !m_questionList->itemAt(me->pos()))
                m_questionList->clearSelection();
            else if (obj == m_selectedList && !m_selectedList->itemAt(me->pos()))
                m_selectedList->clearSelection();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CustomTestBuilderWidget::updateAddAllState()
{
    if (!m_addAllBtn || !m_questionList) return;

    QSet<int> selectedIds;
    for (const auto& q : m_selectedQuestions) selectedIds.insert(q.id);

    for (int i = 0; i < m_questionList->count(); ++i) {
        auto* item = m_questionList->item(i);
        if (!item) continue;
        if (!selectedIds.contains(item->data(Qt::UserRole).toInt())) {
            m_addAllBtn->setEnabled(true);
            return;
        }
    }
    m_addAllBtn->setEnabled(false);
}

void CustomTestBuilderWidget::applyCompleterTheme()
{
    if (!m_questionTagCompleter || !m_questionTagCompleter->popup()) return;
    const Theme& th = ThemeManager::instance()->currentTheme();
    m_questionTagCompleter->popup()->setStyleSheet(QString(
        "QAbstractItemView {"
        "  background-color: %1; color: %2;"
        "  border: 1px solid %3;"
        "  selection-background-color: %4; selection-color: white;"
        "  font-size: 12px; padding: 2px;"
        "}")
        .arg(th.panelBackground.name())
        .arg(th.textPrimary.name())
        .arg(th.border.name())
        .arg(th.accent.name()));
}

void CustomTestBuilderWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        CustomTestBuilderWidget { background-color: %1; }

        /* ── My Tests page ─────────────────────────────────────── */
        #myTestsPage { background-color: %1; }

        #sectionHeader {
            color: %5;
            font-size: 15px;
            font-weight: bold;
        }

        #myTestsList {
            background-color: %2;
            border: 1px solid %3;
            border-radius: 4px;
            color: %5;
        }
        #myTestsList::item {
            padding: 10px;
            border-bottom: 1px solid %3;
            background-color: %2;
            color: %5;
        }
        #myTestsList::item:hover {
            background-color: %6;
            border-radius: 4px;
        }
        #myTestsList::item:selected {
            background-color: %9;
            color: white;
        }

        /* ── Builder page ───────────────────────────────────────── */
        #builderPage  { background-color: %1; }

        /* Pane headers*/
        #paneHeader {
            background-color: %4;
            color: %5;
            font-size: 12px;
            font-weight: bold;
            padding-left: 10px;
            border-bottom: 1px solid %3;
        }

        /* Topic tree */
        #topicTree {
            background-color: %2;
            color: %5;
            border: none;
            font-size: 13px;
        }
        #topicTree::item { padding: 4px 6px; }
        #topicTree::item:hover {
            padding-left: 10px;
            background-color: %7;
        }
        #topicTree::item:selected { background-color: %6; color: white; }
        #topicTree::item:selected:hover {
            background-color: %6;
            color: white;
            padding-left: 10px;
        }

        /* Question browser list */
        #questionBrowser {
            background-color: %2;
            color: %5;
            border: none;
            font-size: 13px;
        }
        #questionBrowser::item {
            padding: 6px 8px;
            border-bottom: 1px solid %3;
            background-color: %2;
            color: %5;
        }
        #questionBrowser::item:hover    { background-color: %6; border-radius: 3px; }
        #questionBrowser::item:selected { background-color: %9; color: white; }

        /* Selected questions list */
        #selectedList {
            background-color: %2;
            color: %5;
            border: none;
            font-size: 13px;
        }
        #selectedList::item {
            padding: 6px 8px;
            border-bottom: 1px solid %3;
            background-color: %2;
            color: %5;
        }
        #selectedList::item:hover    { background-color: %6; border-radius: 3px; }
        #selectedList::item:selected { background-color: %9; color: white; }

        /* Search fields */
        #titleSearch, #tagSearch {
            background-color: %2;
            color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 4px 8px;
            font-size: 12px;
        }
        #titleSearch:focus, #tagSearch:focus { border-color: %6; }

        /* Bottom action bar — use toolbar/sidebar bg, NOT accent */
        #builderBottomBar {
            background-color: %4;
            border-top: 1px solid %3;
        }
        #randomBar {
            background-color: %4;
            border-top: 1px solid %3;
        }

        /* Primary button */
        #primaryButton {
            background-color: %6;
            color: white;
            border: none;
            border-radius: 4px;
            font-weight: bold;
            padding: 6px 16px;
        }
        #primaryButton:hover    { background-color: %9; }
        #primaryButton:disabled { background-color: %3; color: %8; border: 1px solid %3; }

        /* Secondary / outline button */
        #secondaryButton {
            background-color: transparent;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 6px 14px;
        }
        #secondaryButton:hover    { background-color: %7; border-color: %6; }
        #secondaryButton:disabled { color: %8; border-color: %3; }

        /* Danger button */
        #dangerButton {
            background-color: transparent;
            color: %10;
            border: 1px solid %10;
            border-radius: 4px;
            padding: 6px 14px;
        }
        #dangerButton:hover    { background-color: %10; color: white; }
        #dangerButton:disabled { color: %8; border-color: %3; }

        /* Generate button — same as secondary with accent hover */
        #generateButton {
            background-color: transparent;
            color: %6;
            border: 1px solid %6;
            border-radius: 4px;
            padding: 6px 14px;
            font-weight: bold;
        }
        #generateButton:hover    { background-color: %6; color: white; }
        #generateButton:pressed  { background-color: %9; color: white; }
        #generateButton:disabled { color: %8; border-color: %3; }

        /* Move up/down arrow buttons */
        #moveButton {
            background-color: transparent;
            color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 4px 8px;
            font-size: 14px;
        }
        #moveButton:hover    { background-color: %7; border-color: %6; }
        #moveButton:disabled { color: %8; border-color: %3; }

        /* Metadata labels */
        #difficultyCombo, #countSpin {
            background-color: %2;
            color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 3px 6px;
        }

        #testTitleEdit, #testDescEdit {
            background-color: %2;
            color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 4px 8px;
            font-size: 12px;
        }
        #testTitleEdit:focus, #testDescEdit:focus { border-color: %6; }

        QLabel { color: %5; }
        QScrollBar:vertical { background: %2; width: 8px; border-radius: 4px; }
        QScrollBar::handle:vertical { background: %3; border-radius: 4px; min-height: 20px; }
        QScrollBar::handle:vertical:hover { background: %6; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )")
                      .arg(t.windowBackground.name())      // %1
                      .arg(t.panelBackground.name())       // %2
                      .arg(t.border.name())                // %3
                      .arg(t.toolbarBackground.name())     // %4
                      .arg(t.textPrimary.name())           // %5
                      .arg(t.accent.name())                // %6
                      .arg(t.sidebarBackground.name())     // %7
                      .arg(t.textSecondary.name())         // %8
                      .arg(t.accent.lighter(115).name())   // %9
                      .arg(t.error.name())                 // %10
                  );

    applyCompleterTheme();
}
