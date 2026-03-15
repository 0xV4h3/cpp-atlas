#include "ui/QuizAdminPanel.h"

#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QDateTime>
#include <QMenuBar>
#include <QStatusBar>

QuizAdminPanel::QuizAdminPanel(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    refreshStats();
}

void QuizAdminPanel::setupUi()
{
    setWindowTitle(tr("CppAtlas — Administrator Panel"));
    resize(900, 650);

    // ── Central widget ───────────────────────────────────────────────────────
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    // ── Prominent mode label ─────────────────────────────────────────────────
    m_modeLabel = new QLabel(tr("⚙ Administrator mode"), central);
    m_modeLabel->setObjectName("adminModeLabel");
    m_modeLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont = m_modeLabel->font();
    labelFont.setPointSize(labelFont.pointSize() + 2);
    labelFont.setBold(true);
    m_modeLabel->setFont(labelFont);
    m_modeLabel->setStyleSheet("color: #e06c75; background: #2b2b2b; "
                               "border: 1px solid #e06c75; border-radius: 4px; "
                               "padding: 6px;");
    mainLayout->addWidget(m_modeLabel);

    // ── Tabs ─────────────────────────────────────────────────────────────────
    m_tabs = new QTabWidget(central);
    m_tabs->setObjectName("adminTabs");

    QWidget* contentTab    = new QWidget;
    QWidget* validationTab = new QWidget;
    QWidget* exportTab     = new QWidget;
    QWidget* statsTab      = new QWidget;

    setupContentTab(contentTab);
    setupValidationTab(validationTab);
    setupExportTab(exportTab);
    setupStatsTab(statsTab);

    m_tabs->addTab(contentTab,    tr("Content"));
    m_tabs->addTab(validationTab, tr("Validation"));
    m_tabs->addTab(exportTab,     tr("Export"));
    m_tabs->addTab(statsTab,      tr("Stats"));

    mainLayout->addWidget(m_tabs, 1);

    // ── Log view ─────────────────────────────────────────────────────────────
    QLabel* logLabel = new QLabel(tr("Activity log:"), central);
    mainLayout->addWidget(logLabel);

    m_logView = new QTextEdit(central);
    m_logView->setObjectName("adminLogView");
    m_logView->setReadOnly(true);
    m_logView->setMaximumHeight(130);
    m_logView->setPlaceholderText(tr("Admin operations will be logged here."));
    mainLayout->addWidget(m_logView);

    // ── Status bar ───────────────────────────────────────────────────────────
    statusBar()->showMessage(tr("Administrator panel ready."));
}

void QuizAdminPanel::setupContentTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    QLabel* info = new QLabel(
        tr("Apply incremental SQL content patches to the quiz database.\n"
           "Patches are discovered from the configured patches directory,\n"
           "sorted lexicographically, and applied only once."),
        tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    layout->addStretch(1);

    QPushButton* applyBtn = new QPushButton(tr("Apply Content Updates"), tab);
    applyBtn->setObjectName("adminApplyBtn");
    connect(applyBtn, &QPushButton::clicked, this, &QuizAdminPanel::onApplyContentUpdates);
    layout->addWidget(applyBtn, 0, Qt::AlignLeft);
}

void QuizAdminPanel::setupValidationTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    QLabel* info = new QLabel(
        tr("Validate the consistency of quiz content: check for questions\n"
           "without options, orphaned records, and broken references."),
        tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    layout->addStretch(1);

    QPushButton* validateBtn = new QPushButton(tr("Validate Content"), tab);
    validateBtn->setObjectName("adminValidateBtn");
    connect(validateBtn, &QPushButton::clicked, this, &QuizAdminPanel::onValidateContent);
    layout->addWidget(validateBtn, 0, Qt::AlignLeft);
}

void QuizAdminPanel::setupExportTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    QLabel* info = new QLabel(
        tr("Export a full backup of quiz content (questions, options,\n"
           "topics, tags) to a SQL dump file."),
        tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    layout->addStretch(1);

    QPushButton* exportBtn = new QPushButton(tr("Export Backup"), tab);
    exportBtn->setObjectName("adminExportBtn");
    connect(exportBtn, &QPushButton::clicked, this, &QuizAdminPanel::onExportBackup);
    layout->addWidget(exportBtn, 0, Qt::AlignLeft);
}

void QuizAdminPanel::setupStatsTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    QLabel* info = new QLabel(
        tr("Live database statistics: question counts, session counts,\n"
           "user counts, and applied content patches."),
        tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    layout->addStretch(1);

    QPushButton* refreshBtn = new QPushButton(tr("Refresh Stats"), tab);
    refreshBtn->setObjectName("adminRefreshBtn");
    connect(refreshBtn, &QPushButton::clicked, this, &QuizAdminPanel::refreshStats);
    layout->addWidget(refreshBtn, 0, Qt::AlignLeft);
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void QuizAdminPanel::onApplyContentUpdates()
{
    log(tr("[Content] Apply content updates requested — patch system not yet configured."));
    statusBar()->showMessage(tr("Content update: no patches directory configured."));
}

void QuizAdminPanel::onValidateContent()
{
    log(tr("[Validation] Content validation requested — not yet implemented."));
    statusBar()->showMessage(tr("Validation: not yet implemented."));
}

void QuizAdminPanel::onExportBackup()
{
    log(tr("[Export] Backup export requested — not yet implemented."));
    statusBar()->showMessage(tr("Export: not yet implemented."));
}

void QuizAdminPanel::refreshStats()
{
    log(tr("[Stats] Statistics refreshed at %1.").arg(
        QDateTime::currentDateTime().toString("hh:mm:ss")));
    statusBar()->showMessage(tr("Stats refreshed."));
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

void QuizAdminPanel::log(const QString& message)
{
    if (!m_logView) return;
    const QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logView->append(QString("[%1] %2").arg(timestamp, message));
}
