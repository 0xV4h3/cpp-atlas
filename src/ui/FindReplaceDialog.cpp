#include "ui/FindReplaceDialog.h"
#include "ui/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShortcut>
#include <QKeyEvent>

FindReplaceDialog::FindReplaceDialog(Mode mode, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(mode == Find ? "Find" : "Find / Replace");
    setModal(false);
    setupUi(mode);

    applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &FindReplaceDialog::applyTheme);
}

void FindReplaceDialog::setupUi(Mode initialMode)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    // ── Row 0: toggle ▸ + Find field + ↓ ↑ ✕ ─────────────────────────────
    QHBoxLayout* findRow = new QHBoxLayout();
    findRow->setSpacing(6);

    m_toggleModeBtn = new QPushButton(this);
    m_toggleModeBtn->setObjectName("frToggle");
    m_toggleModeBtn->setFixedSize(24, 24);
    m_toggleModeBtn->setFlat(true);
    m_toggleModeBtn->setToolTip("Toggle Replace (Ctrl+H)");
    connect(m_toggleModeBtn, &QPushButton::clicked, this, &FindReplaceDialog::toggleMode);
    findRow->addWidget(m_toggleModeBtn);

    m_findEdit = new QLineEdit(this);
    m_findEdit->setPlaceholderText("Find");
    m_findEdit->setMinimumWidth(260);
    connect(m_findEdit, &QLineEdit::returnPressed, this, &FindReplaceDialog::findNext);
    findRow->addWidget(m_findEdit, 1);

    m_findNextBtn = new QPushButton("↓", this);
    m_findNextBtn->setObjectName("frSmall");
    m_findNextBtn->setMinimumWidth(30);
    m_findNextBtn->setToolTip("Find Next (Enter)");
    connect(m_findNextBtn, &QPushButton::clicked, this, &FindReplaceDialog::findNext);
    findRow->addWidget(m_findNextBtn);

    m_findPrevBtn = new QPushButton("↑", this);
    m_findPrevBtn->setObjectName("frSmall");
    m_findPrevBtn->setMinimumWidth(30);
    m_findPrevBtn->setToolTip("Find Previous");
    connect(m_findPrevBtn, &QPushButton::clicked, this, &FindReplaceDialog::findPrevious);
    findRow->addWidget(m_findPrevBtn);

    m_closeBtn = new QPushButton("✕", this);
    m_closeBtn->setObjectName("frClose");
    m_closeBtn->setMinimumWidth(30);
    m_closeBtn->setFlat(true);
    m_closeBtn->setToolTip("Close");
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::close);
    findRow->addWidget(m_closeBtn);

    mainLayout->addLayout(findRow);

    // ── Row 1: Replace field + Replace / All buttons ──────────────────────
    m_replaceRow = new QWidget(this);
    QHBoxLayout* repLayout = new QHBoxLayout(m_replaceRow);
    repLayout->setContentsMargins(30, 0, 0, 0);  // indent under toggle button
    repLayout->setSpacing(6);

    m_replaceEdit = new QLineEdit(m_replaceRow);
    m_replaceEdit->setPlaceholderText("Replace");
    repLayout->addWidget(m_replaceEdit, 1);

    m_replaceBtn = new QPushButton("Replace", m_replaceRow);
    m_replaceBtn->setObjectName("frAction");
    m_replaceBtn->setMinimumWidth(80);
    connect(m_replaceBtn, &QPushButton::clicked, this, &FindReplaceDialog::replaceNext);
    repLayout->addWidget(m_replaceBtn);

    m_replaceAllBtn = new QPushButton("All", m_replaceRow);
    m_replaceAllBtn->setObjectName("frAction");
    m_replaceAllBtn->setMinimumWidth(44);
    m_replaceAllBtn->setToolTip("Replace All");
    connect(m_replaceAllBtn, &QPushButton::clicked, this, &FindReplaceDialog::replaceAll);
    repLayout->addWidget(m_replaceAllBtn);

    mainLayout->addWidget(m_replaceRow);

    // ── Row 2: Options checkboxes ─────────────────────────────────────────
    QHBoxLayout* optRow = new QHBoxLayout();
    optRow->setContentsMargins(30, 0, 0, 0);
    optRow->setSpacing(16);

    m_caseSensitive = new QCheckBox("Aa  Case", this);
    m_caseSensitive->setObjectName("frOption");
    m_caseSensitive->setToolTip("Case Sensitive");
    optRow->addWidget(m_caseSensitive);

    m_wholeWord = new QCheckBox("\\b  Word", this);
    m_wholeWord->setObjectName("frOption");
    m_wholeWord->setToolTip("Whole Word");
    optRow->addWidget(m_wholeWord);

    m_regexCheck = new QCheckBox(".*  Regex", this);
    m_regexCheck->setObjectName("frOption");
    m_regexCheck->setToolTip("Regular Expression");
    optRow->addWidget(m_regexCheck);

    optRow->addStretch();
    mainLayout->addLayout(optRow);

    // Ctrl+H toggles replace mode
    QShortcut* sh = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_H), this);
    connect(sh, &QShortcut::activated, this, &FindReplaceDialog::toggleMode);

    // Set initial mode
    m_replaceVisible = (initialMode == Replace);
    setReplaceVisible(m_replaceVisible);

    setMinimumWidth(460);
    adjustSize();
}

void FindReplaceDialog::toggleMode()
{
    m_replaceVisible = !m_replaceVisible;
    setReplaceVisible(m_replaceVisible);
}

void FindReplaceDialog::setReplaceVisible(bool visible)
{
    m_replaceRow->setVisible(visible);
    m_toggleModeBtn->setText(visible ? "\xe2\x96\xbe" : "\xe2\x96\xb8"); // ▾ / ▸
    setWindowTitle(visible ? "Find / Replace" : "Find");
    adjustSize();
}

// Keyboard: Escape closes; Shift+Enter = find previous
void FindReplaceDialog::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) { close(); return; }
    if (e->key() == Qt::Key_Return && (e->modifiers() & Qt::ShiftModifier)) {
        emit findPrevious(); return;
    }
    QDialog::keyPressEvent(e);
}

QString FindReplaceDialog::findText()      const { return m_findEdit->text(); }
QString FindReplaceDialog::replaceText()   const { return m_replaceEdit ? m_replaceEdit->text() : QString(); }
bool    FindReplaceDialog::caseSensitive() const { return m_caseSensitive->isChecked(); }
bool    FindReplaceDialog::wholeWord()     const { return m_wholeWord->isChecked(); }
bool    FindReplaceDialog::useRegex()      const { return m_regexCheck->isChecked(); }

void FindReplaceDialog::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();

    setStyleSheet(QString(R"(
        FindReplaceDialog {
            background-color: %1;
            color: %2;
        }
        QLineEdit {
            background-color: %3;
            color: %2;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 4px 6px;
            font-size: 13px;
        }
        QLineEdit:focus { border: 1px solid %5; }

        /* Small navigation buttons */
        QPushButton#frSmall, QPushButton#frToggle {
            background-color: %3;
            color: %2;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 2px 4px;
            font-size: 14px;
        }
        QPushButton#frSmall:hover, QPushButton#frToggle:hover {
            background-color: %5;
            color: white;
            border-color: %5;
        }

        /* Close button */
        QPushButton#frClose {
            background-color: transparent;
            color: %6;
            border: none;
            font-size: 12px;
            padding: 2px 4px;
        }
        QPushButton#frClose:hover {
            background-color: %7;
            color: white;
        }

        /* Replace / All action buttons */
        QPushButton#frAction {
            background-color: %5;
            color: white;
            border: none;
            border-radius: 3px;
            padding: 4px 10px;
            font-size: 13px;
        }
        QPushButton#frAction:hover  { background-color: %8; }
        QPushButton#frAction:pressed { background-color: %9; }

        /* Checkboxes — explicit styling so they're visible in dark themes */
        QCheckBox#frOption {
            color: %2;
            font-size: 12px;
            spacing: 6px;
        }
        QCheckBox#frOption::indicator {
            width: 14px;
            height: 14px;
            border: 1px solid %4;
            border-radius: 3px;
            background-color: %3;
        }
        QCheckBox#frOption::indicator:checked {
            background-color: %5;
            border-color: %5;
        }
        QCheckBox#frOption::indicator:hover {
            border-color: %5;
        }

        QLabel { color: %2; }
    )")
                      .arg(t.panelBackground.name())       // %1 dialog bg
                      .arg(t.textPrimary.name())           // %2 text
                      .arg(t.editorBackground.name())      // %3 input bg
                      .arg(t.border.name())                // %4 border
                      .arg(t.accent.name())                // %5 accent
                      .arg(t.textSecondary.name())         // %6 close btn color
                      .arg(t.error.name())                 // %7 close hover
                      .arg(t.accent.lighter(115).name())   // %8 btn hover
                      .arg(t.accent.darker(115).name())    // %9 btn pressed
                  );
}
