#include "ui/QuizSelectionWidget.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QFrame>
#include <QScrollArea>
#include <QCompleter>
#include <QStringListModel>

QuizSelectionWidget::QuizSelectionWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    populateTopicTree();
    populateQuizList(-1);  // Show all on load
    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &QuizSelectionWidget::applyTheme);
}

void QuizSelectionWidget::refresh()
{
    m_topicTree->clear();
    populateTopicTree();
    populateQuizList(-1);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────
void QuizSelectionWidget::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);

    // ── Left: Topic Tree ───────────────────────────────────────────────────
    QWidget* leftPane = new QWidget(splitter);
    leftPane->setObjectName("topicPane");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPane);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    QLabel* topicHeader = new QLabel("  Topics", leftPane);
    topicHeader->setObjectName("paneHeader");
    topicHeader->setFixedHeight(36);
    leftLayout->addWidget(topicHeader);

    m_topicTree = new QTreeWidget(leftPane);
    m_topicTree->setObjectName("topicTree");
    m_topicTree->setHeaderHidden(true);
    m_topicTree->setAnimated(true);
    m_topicTree->setIndentation(16);
    connect(m_topicTree, &QTreeWidget::itemSelectionChanged,
            this, &QuizSelectionWidget::onTopicSelected);
    leftLayout->addWidget(m_topicTree, 1);

    splitter->addWidget(leftPane);
    splitter->setStretchFactor(0, 1);

    // ── Right: Quiz List + Detail ──────────────────────────────────────────
    QWidget* rightPane = new QWidget(splitter);
    rightPane->setObjectName("quizPane");
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPane);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    // Filter bar
    QWidget* filterBar = new QWidget(rightPane);
    filterBar->setObjectName("filterBar");
    filterBar->setFixedHeight(44);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterBar);
    filterLayout->setContentsMargins(12, 4, 12, 4);
    filterLayout->setSpacing(10);

    QLabel* diffLabel = new QLabel("Difficulty:", filterBar);
    diffLabel->setObjectName("filterLabel");
    m_difficultyCombo = new QComboBox(filterBar);
    m_difficultyCombo->setObjectName("filterCombo");
    m_difficultyCombo->addItem("All levels",      -1);
    m_difficultyCombo->addItem(QString::fromUtf8(u8"\u2605 Beginner"),                  1);
    m_difficultyCombo->addItem(QString::fromUtf8(u8"\u2605\u2605 Intermediate"),        2);
    m_difficultyCombo->addItem(QString::fromUtf8(u8"\u2605\u2605\u2605 Advanced"),      3);
    m_difficultyCombo->addItem(QString::fromUtf8(u8"\u2605\u2605\u2605\u2605 Expert"),  4);
    connect(m_difficultyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QuizSelectionWidget::onDifficultyFilterChanged);
    filterLayout->addWidget(diffLabel);
    filterLayout->addWidget(m_difficultyCombo);

    filterLayout->addSpacing(8);

    QLabel* titleLabel = new QLabel("Title:", filterBar);
    titleLabel->setObjectName("filterLabel");
    m_titleSearch = new QLineEdit(filterBar);
    m_titleSearch->setObjectName("titleSearch");
    m_titleSearch->setPlaceholderText("Search by title…");
    m_titleSearch->setFixedWidth(160);
    connect(m_titleSearch, &QLineEdit::textChanged,
            this, [this](const QString&) { onSearchChanged(); });
    filterLayout->addWidget(titleLabel);
    filterLayout->addWidget(m_titleSearch);

    filterLayout->addSpacing(8);

    QLabel* tagLabel = new QLabel("Tag:", filterBar);
    tagLabel->setObjectName("filterLabel");
    m_tagSearch = new QLineEdit(filterBar);
    m_tagSearch->setObjectName("tagSearch");
    m_tagSearch->setPlaceholderText("e.g. pointers, oop …");
    m_tagSearch->setFixedWidth(150);
    connect(m_tagSearch, &QLineEdit::textChanged,
            this, &QuizSelectionWidget::onTagSearchChanged);
    filterLayout->addWidget(tagLabel);
    filterLayout->addWidget(m_tagSearch);

    // Tag suggestion completer
    m_tagCompleter = new QCompleter(this);
    m_tagCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_tagCompleter->setFilterMode(Qt::MatchContains);
    m_tagSearch->setCompleter(m_tagCompleter);
    {
        QStringList tagNames;
        for (const auto& tg : m_repo.allTags()) tagNames << tg.name;
        m_tagCompleter->setModel(new QStringListModel(tagNames, m_tagCompleter));
    }
    // Lazily theme completer popup on first keypress
    connect(m_tagSearch, &QLineEdit::textChanged, this, [this](const QString&) {
        applyCompleterTheme();
    });

    filterLayout->addStretch();

    rightLayout->addWidget(filterBar);

    // Quiz list
    m_quizList = new QListWidget(rightPane);
    m_quizList->setObjectName("quizList");
    m_quizList->setSpacing(4);
    m_quizList->setUniformItemSizes(false);
    connect(m_quizList, &QListWidget::itemSelectionChanged,
            this, &QuizSelectionWidget::onQuizSelected);
    connect(m_quizList, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem*) { onStartClicked(); });
    rightLayout->addWidget(m_quizList, 1);

    // Detail panel
    m_detailPanel = new QWidget(rightPane);
    m_detailPanel->setObjectName("quizDetailPanel");
    m_detailPanel->setFixedHeight(130);
    QVBoxLayout* detailLayout = new QVBoxLayout(m_detailPanel);
    detailLayout->setContentsMargins(16, 10, 16, 10);
    detailLayout->setSpacing(4);

    m_detailTitle = new QLabel(m_detailPanel);
    m_detailTitle->setObjectName("detailTitle");
    m_detailTitle->setWordWrap(true);
    detailLayout->addWidget(m_detailTitle);

    m_detailDesc = new QLabel(m_detailPanel);
    m_detailDesc->setObjectName("detailDesc");
    m_detailDesc->setWordWrap(true);
    detailLayout->addWidget(m_detailDesc);

    QHBoxLayout* detailBottom = new QHBoxLayout();
    m_detailMeta = new QLabel(m_detailPanel);
    m_detailMeta->setObjectName("detailMeta");
    detailBottom->addWidget(m_detailMeta, 1);

    m_startBtn = new QPushButton("▶  Start Quiz", m_detailPanel);
    m_startBtn->setObjectName("startQuizButton");
    m_startBtn->setFixedSize(140, 36);
    m_startBtn->setEnabled(false);
    connect(m_startBtn, &QPushButton::clicked, this, &QuizSelectionWidget::onStartClicked);
    detailBottom->addWidget(m_startBtn);
    detailLayout->addLayout(detailBottom);

    rightLayout->addWidget(m_detailPanel);
    splitter->addWidget(rightPane);
    splitter->setStretchFactor(1, 3);

    mainLayout->addWidget(splitter, 1);
    splitter->setSizes({220, 580});
}

// ─────────────────────────────────────────────────────────────────────────────
// Data population
// ─────────────────────────────────────────────────────────────────────────────
void QuizSelectionWidget::populateTopicTree()
{
    m_topicTree->clear();

    // "All Quizzes" root item
    QTreeWidgetItem* allItem = new QTreeWidgetItem(m_topicTree);
    allItem->setText(0, "📋  All Quizzes");
    allItem->setData(0, Qt::UserRole, -1);
    allItem->setExpanded(true);

    const QList<TopicDTO> roots = m_repo.rootTopics();
    for (const auto& root : roots) {
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_topicTree);
        rootItem->setText(0, QString("%1  %2").arg(root.icon, root.title));
        rootItem->setData(0, Qt::UserRole, root.id);
        rootItem->setToolTip(0, root.description);

        const QList<TopicDTO> children = m_repo.childTopics(root.id);
        for (const auto& child : children) {
            QTreeWidgetItem* childItem = new QTreeWidgetItem(rootItem);
            childItem->setText(0, QString("  %1  %2").arg(child.icon, child.title));
            childItem->setData(0, Qt::UserRole, child.id);
            childItem->setToolTip(0, child.description);
        }
    }

    m_topicTree->setCurrentItem(allItem);
}

void QuizSelectionWidget::populateQuizList(int topicId)
{
    m_quizList->clear();
    clearQuizDetail();

    const int diffFilter = m_difficultyCombo->currentData().toInt();
    const QString titleFilter = m_titleSearch ? m_titleSearch->text().trimmed().toLower() : QString();
    const QString tagFilter   = m_tagSearch   ? m_tagSearch->text().trimmed().toLower()   : QString();

    QList<QuizDTO> quizzes = (topicId < 0)
                              ? m_repo.allActiveQuizzes()
                              : m_repo.quizzesByTopic(topicId);

    // Apply filters
    m_currentQuizzes.clear();
    for (const auto& qz : quizzes) {
        if (diffFilter > 0 && qz.difficulty != diffFilter) continue;
        if (!titleFilter.isEmpty() && !qz.title.toLower().contains(titleFilter)) continue;
        if (!tagFilter.isEmpty()) {
            bool tagMatch = false;
            for (const auto& tag : qz.tags) {
                if (tag.toLower().contains(tagFilter)) { tagMatch = true; break; }
            }
            if (!tagMatch) continue;
        }
        m_currentQuizzes << qz;
    }

    if (m_currentQuizzes.isEmpty()) {
        QListWidgetItem* empty = new QListWidgetItem("No quizzes found for this filter.");
        empty->setFlags(Qt::NoItemFlags);
        m_quizList->addItem(empty);
        return;
    }

    for (const auto& qz : m_currentQuizzes) {
        QListWidgetItem* item = new QListWidgetItem(m_quizList);

        const QString stars = difficultyStars(qz.difficulty);
        const QString tags  = qz.tags.isEmpty()
                              ? "" : "  🏷 " + qz.tags.mid(0,3).join(", ");

        item->setText(QString("%1  %2\n    %3  •  %4 questions%5")
                      .arg(stars)
                      .arg(qz.title)
                      .arg(difficultyLabel(qz.difficulty))
                      .arg(qz.questionCount)
                      .arg(tags));
        item->setData(Qt::UserRole, qz.id);
        item->setSizeHint(QSize(0, 56));
        m_quizList->addItem(item);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────
void QuizSelectionWidget::onTopicSelected()
{
    QTreeWidgetItem* item = m_topicTree->currentItem();
    if (!item) return;
    const int topicId = item->data(0, Qt::UserRole).toInt();
    populateQuizList(topicId < 0 ? -1 : topicId);
}

void QuizSelectionWidget::onQuizSelected()
{
    QListWidgetItem* item = m_quizList->currentItem();
    if (!item) { clearQuizDetail(); return; }
    const int quizId = item->data(Qt::UserRole).toInt();
    if (quizId <= 0) { clearQuizDetail(); return; }

    for (const auto& qz : m_currentQuizzes) {
        if (qz.id == quizId) {
            showQuizDetail(qz);
            return;
        }
    }
}

void QuizSelectionWidget::onStartClicked()
{
    if (m_selectedQuizId > 0)
        emit quizSelected(m_selectedQuizId);
}

void QuizSelectionWidget::onDifficultyFilterChanged(int /*index*/)
{
    QTreeWidgetItem* curr = m_topicTree->currentItem();
    const int topicId = curr ? curr->data(0, Qt::UserRole).toInt() : -1;
    populateQuizList(topicId);
}

void QuizSelectionWidget::onSearchChanged()
{
    QTreeWidgetItem* curr = m_topicTree->currentItem();
    const int topicId = curr ? curr->data(0, Qt::UserRole).toInt() : -1;
    populateQuizList(topicId);
}

void QuizSelectionWidget::onTagSearchChanged(const QString& /*text*/)
{
    onSearchChanged();
}

// ─────────────────────────────────────────────────────────────────────────────
// Detail panel
// ─────────────────────────────────────────────────────────────────────────────
void QuizSelectionWidget::showQuizDetail(const QuizDTO& quiz)
{
    m_selectedQuizId = quiz.id;
    m_detailTitle->setText(quiz.title);
    m_detailDesc->setText(quiz.description);

    const QString timedStr = quiz.isTimed
        ? QString("  ⏱ %1 sec/question").arg(quiz.timeLimitSec) : "";
    const QString tagStr = quiz.tags.isEmpty()
        ? "" : "  🏷 " + quiz.tags.join(", ");

    m_detailMeta->setText(
        QString("%1  •  %2 questions  •  %3%4%5")
        .arg(difficultyLabel(quiz.difficulty))
        .arg(quiz.questionCount)
        .arg(quiz.type)
        .arg(timedStr)
        .arg(tagStr));

    m_startBtn->setEnabled(true);
}

void QuizSelectionWidget::clearQuizDetail()
{
    m_selectedQuizId = -1;
    m_detailTitle->setText("Select a quiz to see details");
    m_detailDesc->clear();
    m_detailMeta->clear();
    m_startBtn->setEnabled(false);
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────
QString QuizSelectionWidget::difficultyLabel(int d) const
{
    switch (d) {
        case 1: return "Beginner";
        case 2: return "Intermediate";
        case 3: return "Advanced";
        case 4: return "Expert";
        default: return "Expert";
    }
}

QString QuizSelectionWidget::difficultyStars(int d) const
{
    const QString star  = QString::fromUtf8(u8"\u2605");
    const QString empty = QString::fromUtf8(u8"\u2606");
    QString s;
    for (int i = 1; i <= 4; ++i) s += (i <= d ? star : empty);
    return s;
}

void QuizSelectionWidget::applyCompleterTheme()
{
    if (!m_tagCompleter || !m_tagCompleter->popup()) return;
    const Theme& th = ThemeManager::instance()->currentTheme();
    m_tagCompleter->popup()->setStyleSheet(QString(
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

void QuizSelectionWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        QuizSelectionWidget { background-color: %1; }
        #topicPane   { background-color: %2; border-right: 1px solid %3; }
        #quizPane    { background-color: %1; }
        #paneHeader  {
            background-color: %4;
            color: %5;
            font-size: 13px;
            font-weight: bold;
            padding-left: 12px;
            border-bottom: 1px solid %3;
        }
        #topicTree {
            background-color: %2;
            color: %5;
            border: none;
            font-size: 13px;
        }
        #topicTree::item { padding: 4px 6px; }
        #topicTree::item:hover    { padding-left: 10px; background-color: %7; }
        #topicTree::item:selected { background-color: %6; color: white; }
        #topicTree::item:selected:hover {
            background-color: %6;
            color: white;
            padding-left: 10px;
        }
        #filterBar {
            background-color: %4;
            border-bottom: 1px solid %3;
        }
        #filterLabel { color: %8; font-size: 12px; }
        #filterCombo, #titleSearch, #tagSearch {
            background-color: %1;
            color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 3px 6px;
            font-size: 12px;
        }
        #titleSearch:focus, #tagSearch:focus { border-color: %6; }
        #quizList {
            background-color: %1;
            color: %5;
            border: none;
            font-size: 13px;
        }
        #quizList::item {
            background-color: %2;
            border-radius: 4px;
            padding: 8px 10px;
            margin: 2px 8px;
            color: %5;
            border-bottom: 1px solid %3;
        }
        #quizList::item:selected { background-color: %9; color: white; }
        #quizList::item:hover    { background-color: %6; border-radius: 4px; }
        #quizDetailPanel {
            background-color: %4;
            border-top: 1px solid %3;
        }
        #detailTitle {
            color: %5;
            font-size: 14px;
            font-weight: bold;
        }
        #detailDesc { color: %8; font-size: 12px; }
        #detailMeta { color: %8; font-size: 11px; }
        #startQuizButton {
            background-color: %6;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
            font-weight: bold;
        }
        #startQuizButton:hover    { background-color: %9; }
        #startQuizButton:disabled { background-color: %3; color: %8; }
        QLabel { color: %5; }
    )")
    .arg(t.windowBackground.name())        // %1
    .arg(t.panelBackground.name())         // %2
    .arg(t.border.name())                  // %3
    .arg(t.toolbarBackground.name())       // %4
    .arg(t.textPrimary.name())             // %5
    .arg(t.accent.name())                  // %6
    .arg(t.sidebarBackground.name())       // %7
    .arg(t.textSecondary.name())           // %8
    .arg(t.accent.lighter(115).name())     // %9
    );

    // Theme tag completer popup (if already visible)
    applyCompleterTheme();
}
