#include "ui/QuizAdminPanel.h"

#include "quiz/ContentPatchService.h"
#include "quiz/QuizDatabase.h"
#include "quiz/QuizContentExporter.h"

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
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
    QSettings settings;
    const QString lastPatchDir = settings.value("admin/patchDir").toString();

    const QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Content Patches Directory"),
        lastPatchDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (dir.isEmpty()) {
        log(tr("[Content] Operation cancelled."));
        return;
    }
    settings.setValue("admin/patchDir", dir);

    log(tr("[Content] Discovering patches in: %1").arg(dir));

    ContentPatchService svc;
    const QList<ContentPatch> patches = svc.discoverPatches(dir);

    if (patches.isEmpty()) {
        log(tr("[Content] No *.sql patch files found."));
        statusBar()->showMessage(tr("No patches found."));
        return;
    }

    int alreadyApplied = 0;
    int pending = 0;
    for (const ContentPatch& p : patches) {
        if (svc.isPatchApplied(p.id)) ++alreadyApplied;
        else ++pending;
    }

    log(tr("[Content] Total: %1  Applied: %2  Pending: %3")
        .arg(patches.size()).arg(alreadyApplied).arg(pending));

    if (pending == 0) {
        log(tr("[Content] All patches already applied."));
        statusBar()->showMessage(tr("All patches already applied."));
        return;
    }

    QString patchError;
    if (!svc.applyPendingPatches(patches, &patchError)) {
        log(tr("[Content] ERROR: %1").arg(patchError));
        statusBar()->showMessage(tr("Content update failed."));
        QMessageBox::critical(this, tr("Apply Content"), patchError);
        return;
    }

    log(tr("[Content] Successfully applied %1 patch(es).").arg(pending));
    statusBar()->showMessage(tr("Applied %1 patch(es).").arg(pending));
}

void QuizAdminPanel::onValidateContent()
{
    log(tr("[Validation] Running content integrity checks..."));

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    if (!db.isOpen()) {
        log(tr("[Validation] ERROR: database is not open."));
        statusBar()->showMessage(tr("Validation failed: database not open."));
        return;
    }

    auto queryInt = [&](const QString& sql) -> int {
        QSqlQuery q(db);
        if (q.exec(sql) && q.next()) return q.value(0).toInt();
        return -1;
    };

    const int mcqNoOptions = queryInt(
        "SELECT COUNT(*) FROM questions q "
        "WHERE q.type = 'mcq' AND q.is_active = 1 "
        "AND NOT EXISTS (SELECT 1 FROM options o WHERE o.question_id = q.id)");

    const int orphanOptions = queryInt(
        "SELECT COUNT(*) FROM options o "
        "WHERE NOT EXISTS (SELECT 1 FROM questions q WHERE q.id = o.question_id)");

    const int mcqNoCorrect = queryInt(
        "SELECT COUNT(*) FROM questions q "
        "WHERE q.type = 'mcq' AND q.is_active = 1 "
        "AND NOT EXISTS ("
        "  SELECT 1 FROM options o WHERE o.question_id = q.id AND o.is_correct = 1)");

    log(tr("[Validation] MCQ without options : %1").arg(mcqNoOptions >= 0 ? QString::number(mcqNoOptions) : "n/a"));
    log(tr("[Validation] Orphan options      : %1").arg(orphanOptions >= 0 ? QString::number(orphanOptions) : "n/a"));
    log(tr("[Validation] MCQ without correct : %1").arg(mcqNoCorrect >= 0 ? QString::number(mcqNoCorrect) : "n/a"));

    const bool ok = (mcqNoOptions == 0 && orphanOptions == 0 && mcqNoCorrect == 0);
    if (ok) {
        log(tr("[Validation] Result: OK"));
        statusBar()->showMessage(tr("Validation passed."));
    } else {
        log(tr("[Validation] Result: WARNINGS FOUND"));
        statusBar()->showMessage(tr("Validation: warnings found."));
    }
}

void QuizAdminPanel::onExportBackup()
{
    const QString outFile = QFileDialog::getSaveFileName(
        this,
        tr("Export Quiz Content Backup"),
        QString("cppatlas_backup_%1.sql").arg(
            QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        tr("SQL Files (*.sql);;All Files (*)")
    );
    if (outFile.isEmpty()) {
        log(tr("[Export] Operation cancelled."));
        return;
    }

    log(tr("[Export] Exporting to: %1").arg(outFile));

    int totalRows = 0;
    QString exportError;
    if (!QuizContentExporter::exportToFile(outFile, &totalRows, &exportError)) {
        log(tr("[Export] ERROR: %1").arg(exportError));
        statusBar()->showMessage(tr("Export failed."));
        QMessageBox::critical(this, tr("Export Backup"), exportError);
        return;
    }

    log(tr("[Export] Total rows: %1. File: %2").arg(totalRows).arg(outFile));
    statusBar()->showMessage(tr("Export complete: %1 row(s).").arg(totalRows));
}

void QuizAdminPanel::refreshStats()
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    if (!db.isOpen()) {
        log(tr("[Stats] Database not open."));
        statusBar()->showMessage(tr("Stats: database not open."));
        return;
    }

    auto qi = [&](const QString& sql) -> QString {
        QSqlQuery q(db);
        if (q.exec(sql) && q.next()) return QString::number(q.value(0).toInt());
        return "n/a";
    };

    log(tr("[Stats] === Database Statistics ==="));
    log(tr("[Stats] Database path  : %1").arg(QuizDatabase::instance().databasePath()));
    log(tr("[Stats] Schema version : %1").arg(qi("SELECT MAX(version) FROM schema_version")));
    log(tr("[Stats] Topics         : %1").arg(qi("SELECT COUNT(*) FROM topics")));
    log(tr("[Stats] Quizzes        : %1").arg(qi("SELECT COUNT(*) FROM quizzes WHERE is_active = 1")));
    log(tr("[Stats] Questions      : %1").arg(qi("SELECT COUNT(*) FROM questions WHERE is_active = 1")));
    log(tr("[Stats] Tags           : %1").arg(qi("SELECT COUNT(*) FROM tags")));
    log(tr("[Stats] Users          : %1").arg(qi("SELECT COUNT(*) FROM users")));
    log(tr("[Stats] Admins         : %1").arg(qi("SELECT COUNT(*) FROM users WHERE is_admin = 1")));
    log(tr("[Stats] Sessions       : %1").arg(qi("SELECT COUNT(*) FROM quiz_sessions")));
    log(tr("[Stats] Patches applied: %1").arg(qi("SELECT COUNT(*) FROM content_patches")));
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
