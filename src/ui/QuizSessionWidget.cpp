#include "ui/QuizSessionWidget.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QScrollArea>
#include <QButtonGroup>
#include <QTimer>
#include <QFont>
#include <QStyle>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

QuizSessionWidget::QuizSessionWidget(QWidget* parent)
    : QWidget(parent)
    , m_engine(new QuizEngine(this))
    , m_radioGroup(new QButtonGroup(this))
{
    setupUi();
    applyTheme();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &QuizSessionWidget::applyTheme);

    // Wire engine signals
    connect(m_engine, &QuizEngine::questionChanged,
            this, &QuizSessionWidget::onQuestionChanged);
    connect(m_engine, &QuizEngine::questionTimerTick,
            this, &QuizSessionWidget::onTimerTick);
    connect(m_engine, &QuizEngine::questionTimedOut,
            this, &QuizSessionWidget::onQuestionTimedOut);
    connect(m_engine, &QuizEngine::sessionCompleted,
            this, &QuizSessionWidget::sessionCompleted);
    connect(m_engine, &QuizEngine::sessionAbandoned,
            this, &QuizSessionWidget::sessionAbandoned);

    m_radioGroup->setExclusive(true);
}

void QuizSessionWidget::startQuiz(int quizId, int userId,
                                   const QString& mode, bool shuffle)
{
    m_currentMode    = mode;
    m_selectedOptionId = -1;
    m_awaitingNext   = false;

    m_hintLabel->setVisible(false);
    m_feedbackLabel->setVisible(false);
    m_timerLabel->setVisible(false);

    m_engine->startSession(quizId, userId, mode, shuffle);
}

void QuizSessionWidget::startCustomSession(const QList<QuestionDTO>& questions,
                                            int userId,
                                            const QString& mode)
{
    m_currentMode      = mode;
    m_selectedOptionId = -1;
    m_awaitingNext     = false;

    m_hintLabel->setVisible(false);
    m_feedbackLabel->setVisible(false);
    m_timerLabel->setVisible(false);

    m_engine->startCustomSession(questions, userId, mode);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Header bar ─────────────────────────────────────────────────────────
    QWidget* header = new QWidget(this);
    header->setObjectName("sessionHeader");
    header->setFixedHeight(44);
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 0, 16, 0);
    headerLayout->setSpacing(12);

    m_progressLabel = new QLabel("Question 1 / 1", header);
    m_progressLabel->setObjectName("progressLabel");
    headerLayout->addWidget(m_progressLabel);

    m_progressBar = new QProgressBar(header);
    m_progressBar->setObjectName("questionProgressBar");
    m_progressBar->setFixedHeight(6);
    m_progressBar->setTextVisible(false);
    m_progressBar->setRange(0, 100);
    headerLayout->addWidget(m_progressBar, 1);

    m_timerLabel = new QLabel("⏱ 0:00", header);
    m_timerLabel->setObjectName("timerLabel");
    m_timerLabel->setVisible(false);
    headerLayout->addWidget(m_timerLabel);

    m_scoreLabel = new QLabel("Score: 0", header);
    m_scoreLabel->setObjectName("scoreLabel");
    headerLayout->addWidget(m_scoreLabel);

    m_abandonBtn = new QPushButton("✕  Abandon", header);
    m_abandonBtn->setObjectName("abandonButton");
    connect(m_abandonBtn, &QPushButton::clicked,
            this, &QuizSessionWidget::onAbandonClicked);
    headerLayout->addWidget(m_abandonBtn);

    mainLayout->addWidget(header);

    // ── Content area (scrollable) ──────────────────────────────────────────
    QScrollArea* contentScroll = new QScrollArea(this);
    contentScroll->setWidgetResizable(true);
    contentScroll->setObjectName("contentScroll");
    contentScroll->setFrameShape(QFrame::NoFrame);

    QWidget* contentWidget = new QWidget(contentScroll);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(60, 30, 60, 20);
    contentLayout->setSpacing(16);

    // Question metadata (number + difficulty)
    QHBoxLayout* metaRow = new QHBoxLayout();
    m_questionNumLabel = new QLabel(contentWidget);
    m_questionNumLabel->setObjectName("questionNumLabel");
    m_difficultyLabel  = new QLabel(contentWidget);
    m_difficultyLabel->setObjectName("difficultyLabel");
    metaRow->addWidget(m_questionNumLabel);
    metaRow->addWidget(m_difficultyLabel);
    metaRow->addStretch();
    contentLayout->addLayout(metaRow);

    // Question text
    m_questionText = new QLabel(contentWidget);
    m_questionText->setObjectName("questionText");
    m_questionText->setWordWrap(true);
    m_questionText->setTextFormat(Qt::PlainText);
    contentLayout->addWidget(m_questionText);

    // Code snippet (hidden when empty)
    m_codeSnippetLabel = new QLabel(contentWidget);
    m_codeSnippetLabel->setObjectName("codeSnippet");
    m_codeSnippetLabel->setWordWrap(false);
    m_codeSnippetLabel->setVisible(false);
    m_codeSnippetLabel->setTextFormat(Qt::PlainText);
    contentLayout->addWidget(m_codeSnippetLabel);

    // Option area (filled dynamically)
    m_optionScroll = new QScrollArea(contentWidget);
    m_optionScroll->setObjectName("optionScroll");
    m_optionScroll->setWidgetResizable(true);
    m_optionScroll->setFrameShape(QFrame::NoFrame);
    m_optionScroll->setMaximumHeight(320);

    m_optionArea = new QWidget(m_optionScroll);
    m_optionLayout = new QVBoxLayout(m_optionArea);
    m_optionLayout->setContentsMargins(0, 8, 0, 8);
    m_optionLayout->setSpacing(8);
    m_optionScroll->setWidget(m_optionArea);
    contentLayout->addWidget(m_optionScroll);

    // Hint label (hidden until requested)
    m_hintLabel = new QLabel(contentWidget);
    m_hintLabel->setObjectName("hintLabel");
    m_hintLabel->setWordWrap(true);
    m_hintLabel->setVisible(false);
    contentLayout->addWidget(m_hintLabel);

    // Feedback label (hidden until answer submitted)
    m_feedbackLabel = new QLabel(contentWidget);
    m_feedbackLabel->setObjectName("feedbackLabel");
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setVisible(false);
    contentLayout->addWidget(m_feedbackLabel);

    contentLayout->addStretch();
    contentScroll->setWidget(contentWidget);
    mainLayout->addWidget(contentScroll, 1);

    // ── Footer controls ────────────────────────────────────────────────────
    QWidget* footer = new QWidget(this);
    footer->setObjectName("sessionFooter");
    footer->setFixedHeight(60);
    QHBoxLayout* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(60, 8, 60, 8);
    footerLayout->setSpacing(12);

    m_hintBtn = new QPushButton("💡  Hint", footer);
    m_hintBtn->setObjectName("hintButton");
    connect(m_hintBtn, &QPushButton::clicked,
            this, &QuizSessionWidget::onHintClicked);
    footerLayout->addWidget(m_hintBtn);

    m_skipBtn = new QPushButton("Skip →", footer);
    m_skipBtn->setObjectName("skipButton");
    connect(m_skipBtn, &QPushButton::clicked,
            this, &QuizSessionWidget::onSkipClicked);
    footerLayout->addWidget(m_skipBtn);

    footerLayout->addStretch();

    m_submitBtn = new QPushButton("Submit Answer  ↵", footer);
    m_submitBtn->setObjectName("submitButton");
    m_submitBtn->setEnabled(false);
    m_submitBtn->setMinimumWidth(180);
    connect(m_submitBtn, &QPushButton::clicked,
            this, &QuizSessionWidget::onSubmitClicked);
    footerLayout->addWidget(m_submitBtn);

    mainLayout->addWidget(footer);
}

// ─────────────────────────────────────────────────────────────────────────────
// Engine slots
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::onQuestionChanged(int index, int total)
{
    m_selectedOptionId = -1;
    m_awaitingNext     = false;
    m_hintLabel->setVisible(false);
    m_feedbackLabel->setVisible(false);
    m_submitBtn->setEnabled(false);
    m_hintBtn->setEnabled(true);
    m_skipBtn->setEnabled(true);

    updateProgressBar(index, total);
    m_scoreLabel->setText(QString("Score: %1").arg(m_engine->currentScore()));
    renderQuestion(m_engine->currentQuestion());

    const int tl = m_engine->currentQuestion().timeLimitSec;
    if (tl > 0) {
        m_timerLabel->setVisible(true);
        updateTimerDisplay(tl);
    } else {
        m_timerLabel->setVisible(false);
    }
}

void QuizSessionWidget::onTimerTick(int secondsRemaining)
{
    updateTimerDisplay(secondsRemaining);
    // Turn red in last 10 seconds
    if (secondsRemaining <= 10) {
        m_timerLabel->setProperty("urgent", true);
        m_timerLabel->style()->unpolish(m_timerLabel);
        m_timerLabel->style()->polish(m_timerLabel);
    }
}

void QuizSessionWidget::onQuestionTimedOut()
{
    m_timerLabel->setText("⏱ 0:00");
    // Engine auto-submits blank — onQuestionChanged fires next
}

// ─────────────────────────────────────────────────────────────────────────────
// Answer interaction
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::onOptionSelected(int optionId)
{
    m_selectedOptionId = optionId;
    m_submitBtn->setEnabled(true);
}

void QuizSessionWidget::onSubmitClicked()
{
    if (m_awaitingNext) {
        // User acknowledged feedback — advance
        m_engine->submitAnswer(QString());   // engine already recorded answer
        return;
    }

    const QuestionDTO& q = m_engine->currentQuestion();

    QString answer;
    if (q.type == "multi_select") {
        QJsonArray arr;
        for (QCheckBox* cb : m_checkBoxes) {
            if (cb->isChecked())
                arr.append(cb->property("optionId").toInt());
        }
        answer = QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact));
    } else if (q.type == "fill_blank" && m_fillBlankEdit) {
        answer = m_fillBlankEdit->text().trimmed();
    } else {
        // MCQ / true_false / code_output — answer is option id as string
        answer = m_selectedOptionId > 0
                 ? QString::number(m_selectedOptionId) : QString();
    }

    if (answer.isEmpty() && q.type != "fill_blank") return;

    // Evaluate locally for immediate feedback before engine advances
    const bool correct = [&]() -> bool {
        if (q.type == "multi_select") {
            QJsonDocument doc = QJsonDocument::fromJson(answer.toUtf8());
            if (!doc.isArray()) return false;
            QJsonArray arr = doc.array();
            QSet<int> selected;
            for (const auto& v : arr) selected.insert(v.toInt());
            QSet<int> correctSet;
            for (const auto& opt : q.options)
                if (opt.isCorrect) correctSet.insert(opt.id);
            return selected == correctSet;
        }
        for (const auto& opt : q.options) {
            bool ok = false;
            int id = answer.toInt(&ok);
            if (ok && opt.id == id) return opt.isCorrect;
            if (!ok && opt.content.trimmed().compare(
                answer.trimmed(), Qt::CaseInsensitive) == 0) return opt.isCorrect;
        }
        return false;
    }();

    // Show feedback in practice mode; in exam mode skip explanation
    if (m_currentMode == "practice") {
        showFeedback(correct, q.explanation);
        m_awaitingNext = true;
        m_submitBtn->setText("Next Question  →");
        m_submitBtn->setEnabled(true);
        m_hintBtn->setEnabled(false);
        m_skipBtn->setEnabled(false);
    }

    m_engine->submitAnswer(answer);

    // Reset submit button text after engine advances (onQuestionChanged fires)
    m_submitBtn->setText("Submit Answer  ↵");
}

void QuizSessionWidget::onSkipClicked()
{
    m_engine->skipQuestion();
}

void QuizSessionWidget::onHintClicked()
{
    const QString hint = m_engine->requestHint();
    if (!hint.isEmpty()) {
        m_hintLabel->setText("💡  " + hint);
        m_hintLabel->setVisible(true);
    } else {
        m_hintLabel->setText("💡  No hint available for this question.");
        m_hintLabel->setVisible(true);
    }
    m_hintBtn->setEnabled(false);
}

void QuizSessionWidget::onAbandonClicked()
{
    m_engine->abandonSession();
}

// ─────────────────────────────────────────────────────────────────────────────
// Question rendering
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::renderQuestion(const QuestionDTO& q)
{
    // Stars for difficulty
    const QStringList stars = {"", "⭐", "⭐⭐", "⭐⭐⭐", "⭐⭐⭐⭐"};
    const int clamped = qBound(1, q.difficulty, 4);
    m_difficultyLabel->setText(stars[clamped]);
    m_questionText->setText(q.content);

    if (!q.codeSnippet.isEmpty()) {
        m_codeSnippetLabel->setText(q.codeSnippet);
        m_codeSnippetLabel->setVisible(true);
    } else {
        m_codeSnippetLabel->setVisible(false);
    }

    if (q.type == "true_false")      renderTrueFalse(q);
    else if (q.type == "multi_select") renderMultiSelect(q);
    else if (q.type == "fill_blank")   renderFillBlank(q);
    else                               renderMCQ(q);   // mcq + code_output
}

void QuizSessionWidget::renderMCQ(const QuestionDTO& q)
{
    clearOptionArea();
    // Clear old radio buttons from group
    for (QAbstractButton* btn : m_radioGroup->buttons())
        m_radioGroup->removeButton(btn);

    for (const OptionDTO& opt : q.options) {
        QRadioButton* rb = new QRadioButton(m_optionArea);
        rb->setObjectName("optionRadio");
        rb->setText(opt.content);
        if (!opt.codeSnippet.isEmpty())
            rb->setText(opt.content + "\n" + opt.codeSnippet);
        rb->setProperty("optionId", opt.id);

        m_radioGroup->addButton(rb, opt.id);
        m_optionLayout->addWidget(rb);

        connect(rb, &QRadioButton::clicked, this, [this, opt]() {
            onOptionSelected(opt.id);
        });
    }
    m_optionLayout->addStretch();
}

void QuizSessionWidget::renderTrueFalse(const QuestionDTO& q)
{
    clearOptionArea();
    for (QAbstractButton* btn : m_radioGroup->buttons())
        m_radioGroup->removeButton(btn);

    QHBoxLayout* tfLayout = new QHBoxLayout();
    tfLayout->setSpacing(20);

    for (const OptionDTO& opt : q.options) {
        QPushButton* btn = new QPushButton(opt.content, m_optionArea);
        btn->setObjectName("tfButton");
        btn->setMinimumSize(160, 64);
        btn->setCheckable(true);
        btn->setProperty("optionId", opt.id);

        connect(btn, &QPushButton::clicked, this, [this, btn, opt, &q]() {
            // Deselect siblings
            const auto children = m_optionArea->findChildren<QPushButton*>("tfButton");
            for (auto* b : children) {
                if (b != btn) b->setChecked(false);
            }
            btn->setChecked(true);
            onOptionSelected(opt.id);
            Q_UNUSED(q);
        });
        tfLayout->addWidget(btn);
    }

    QWidget* tfContainer = new QWidget(m_optionArea);
    tfContainer->setLayout(tfLayout);
    m_optionLayout->addWidget(tfContainer);
    m_optionLayout->addStretch();
}

void QuizSessionWidget::renderMultiSelect(const QuestionDTO& q)
{
    clearOptionArea();
    m_checkBoxes.clear();

    for (const OptionDTO& opt : q.options) {
        QCheckBox* cb = new QCheckBox(opt.content, m_optionArea);
        cb->setObjectName("optionCheckBox");
        cb->setProperty("optionId", opt.id);
        m_checkBoxes << cb;
        m_optionLayout->addWidget(cb);

        // Enable submit as soon as any box is checked
        connect(cb, &QCheckBox::stateChanged, this, [this]() {
            bool any = false;
            for (QCheckBox* c : m_checkBoxes) any |= c->isChecked();
            m_submitBtn->setEnabled(any);
        });
    }
    m_optionLayout->addStretch();
}

void QuizSessionWidget::renderFillBlank(const QuestionDTO& q)
{
    Q_UNUSED(q);
    clearOptionArea();
    m_fillBlankEdit = new QLineEdit(m_optionArea);
    m_fillBlankEdit->setObjectName("fillBlankEdit");
    m_fillBlankEdit->setPlaceholderText("Type your answer here…");
    m_fillBlankEdit->setMinimumHeight(40);
    connect(m_fillBlankEdit, &QLineEdit::textChanged, this, [this](const QString& t) {
        m_submitBtn->setEnabled(!t.trimmed().isEmpty());
    });
    connect(m_fillBlankEdit, &QLineEdit::returnPressed,
            this, &QuizSessionWidget::onSubmitClicked);
    m_optionLayout->addWidget(m_fillBlankEdit);
    m_optionLayout->addStretch();
    m_fillBlankEdit->setFocus();
}

void QuizSessionWidget::clearOptionArea()
{
    m_fillBlankEdit = nullptr;
    m_checkBoxes.clear();

    // Remove all child widgets from option layout
    QLayoutItem* item;
    while ((item = m_optionLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Feedback
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::showFeedback(bool correct, const QString& explanation)
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    const QString bgColor = correct
        ? t.success.darker(170).name()
        : t.error.darker(200).name();
    const QString borderColor = correct ? t.success.name() : t.error.name();
    const QString icon  = correct ? "✅" : "❌";
    const QString title = correct ? "Correct!" : "Incorrect";

    QString text = QString("<b>%1 %2</b>").arg(icon, title);
    if (!explanation.isEmpty())
        text += QString("<br><span style='font-size:12px; color:%1;'>%2</span>")
                .arg(t.textSecondary.name(), explanation);

    m_feedbackLabel->setText(text);
    m_feedbackLabel->setTextFormat(Qt::RichText);
    m_feedbackLabel->setStyleSheet(
        QString("background-color: %1; border: 1px solid %2; "
                "border-radius: 6px; padding: 10px 14px; color: %3;")
        .arg(bgColor, borderColor, t.textPrimary.name()));
    m_feedbackLabel->setVisible(true);
}

// ─────────────────────────────────────────────────────────────────────────────
// Progress & Timer
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::updateProgressBar(int index, int total)
{
    m_progressLabel->setText(
        QString("Question %1 / %2").arg(index + 1).arg(total));
    m_questionNumLabel->setText(
        QString("Q%1").arg(index + 1));
    if (total > 0)
        m_progressBar->setValue(static_cast<int>(100.0 * index / total));
}

void QuizSessionWidget::updateTimerDisplay(int seconds)
{
    m_timerLabel->setText("⏱  " + formatTime(seconds));
}

QString QuizSessionWidget::formatTime(int seconds) const
{
    return QString("%1:%2")
        .arg(seconds / 60)
        .arg(seconds % 60, 2, 10, QChar('0'));
}

// ─────────────────────────────────────────────────────────────────────────────
// Keyboard
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_awaitingNext) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Space) {
            onSubmitClicked();
            return;
        }
    }

    const int key = event->key();
    // 1-4: select MCQ option by position
    if (key >= Qt::Key_1 && key <= Qt::Key_4) {
        const int idx = key - Qt::Key_1;
        const auto radios = m_optionArea->findChildren<QRadioButton*>("optionRadio");
        if (idx < radios.size()) {
            radios[idx]->setChecked(true);
            onOptionSelected(radios[idx]->property("optionId").toInt());
        }
    } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        if (m_submitBtn->isEnabled()) onSubmitClicked();
    } else if (key == Qt::Key_H) {
        if (m_hintBtn->isEnabled()) onHintClicked();
    } else if (key == Qt::Key_S) {
        if (m_skipBtn->isEnabled()) onSkipClicked();
    }
    QWidget::keyPressEvent(event);
}

// ─────────────────────────────────────────────────────────────────────────────
// Theme
// ─────────────────────────────────────────────────────────────────────────────
void QuizSessionWidget::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();
    setStyleSheet(QString(R"(
        QuizSessionWidget { background-color: %1; }

        #sessionHeader {
            background-color: %2;
            border-bottom: 1px solid %3;
        }
        #progressLabel, #scoreLabel { color: %4; font-size: 12px; }
        #timerLabel {
            color: %5;
            font-size: 13px;
            font-weight: bold;
            min-width: 60px;
        }
        #timerLabel[urgent="true"] { color: %6; }
        #questionProgressBar {
            background-color: %3;
            border: none;
            border-radius: 3px;
        }
        #questionProgressBar::chunk {
            background-color: %7;
            border-radius: 3px;
        }
        #abandonButton {
            background-color: transparent;
            color: %6;
            border: 1px solid %6;
            border-radius: 3px;
            padding: 3px 10px;
            font-size: 12px;
        }
        #abandonButton:hover { background-color: %6; color: white; }

        #contentScroll, #optionScroll { background-color: %1; border: none; }
        #contentScroll > QWidget, #contentScroll > QWidget > QWidget,
        #optionScroll > QWidget, #optionScroll > QWidget > QWidget {
            background-color: %1;
        }

        #questionNumLabel { color: %8; font-size: 12px; }
        #difficultyLabel  { color: %5; font-size: 12px; }
        #questionText {
            color: %4;
            font-size: 15px;
            line-height: 1.5;
        }
        #codeSnippet {
            background-color: %9;
            color: %10;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 13px;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 12px 16px;
        }

        #optionRadio {
            color: %4;
            font-size: 14px;
            padding: 10px 14px;
            background-color: %11;
            border: 1px solid %3;
            border-radius: 6px;
            spacing: 10px;
        }
        #optionRadio:hover    { border-color: %7; background-color: %12; }
        #optionRadio:checked  { border-color: %7; background-color: %13; }

        #optionCheckBox {
            color: %4;
            font-size: 14px;
            padding: 10px 14px;
            background-color: %11;
            border: 1px solid %3;
            border-radius: 6px;
            spacing: 10px;
        }
        #optionCheckBox:hover   { border-color: %7; background-color: %12; }
        #optionCheckBox:checked { border-color: %7; background-color: %13; }

        #tfButton {
            background-color: %11;
            color: %4;
            border: 2px solid %3;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        #tfButton:hover   { border-color: %7; background-color: %12; }
        #tfButton:checked { border-color: %7; background-color: %13; color: white; }

        #fillBlankEdit {
            background-color: %11;
            color: %4;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 14px;
        }
        #fillBlankEdit:focus { border-color: %7; }

        #hintLabel {
            background-color: %14;
            color: %4;
            border: 1px solid %5;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 13px;
        }

        #sessionFooter {
            background-color: %2;
            border-top: 1px solid %3;
        }
        #hintButton, #skipButton {
            background-color: transparent;
            color: %8;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 6px 16px;
            font-size: 13px;
        }
        #hintButton:hover { color: %5; border-color: %5; }
        #skipButton:hover { color: %4; border-color: %4; }
        #hintButton:disabled, #skipButton:disabled { color: %3; border-color: %3; }

        #submitButton {
            background-color: %7;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
        }
        #submitButton:hover    { background-color: %15; }
        #submitButton:disabled { background-color: %3; color: %8; }
    )")
    .arg(t.windowBackground.name())         // %1
    .arg(t.toolbarBackground.name())        // %2
    .arg(t.border.name())                   // %3
    .arg(t.textPrimary.name())              // %4
    .arg(t.warning.name())                  // %5  (timer normal)
    .arg(t.error.name())                    // %6  (timer urgent + abandon)
    .arg(t.accent.name())                   // %7  (progress, submit, selected)
    .arg(t.textSecondary.name())            // %8
    .arg(t.editorBackground.name())         // %9  (code bg)
    .arg(t.editorForeground.name())         // %10 (code fg)
    .arg(t.panelBackground.name())          // %11 (option card bg)
    .arg(t.sidebarBackground.name())        // %12 (option hover)
    .arg(t.accent.darker(150).name())       // %13 (option selected bg)
    .arg(t.sidebarBackground.name())        // %14 (hint bg)
    .arg(t.accent.lighter(115).name())      // %15 (submit hover)
    );

    // Force viewport backgrounds — QSS child selectors don't always propagate
    if (m_optionScroll) {
        if (m_optionScroll->viewport())
            m_optionScroll->viewport()->setStyleSheet(
                QString("background-color: %1;").arg(t.windowBackground.name())
            );
        if (m_optionArea)
            m_optionArea->setStyleSheet(
                QString("background-color: %1;").arg(t.panelBackground.name())
            );
    }
    // contentScroll is a local widget — find it by object name
    auto* cScroll = findChild<QScrollArea*>("contentScroll");
    if (cScroll && cScroll->viewport())
        cScroll->viewport()->setStyleSheet(
            QString("background-color: %1;").arg(t.windowBackground.name())
        );
}
