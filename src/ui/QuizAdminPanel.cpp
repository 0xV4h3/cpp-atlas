#include "ui/QuizAdminPanel.h"

#include "quiz/ContentPatchService.h"
#include "quiz/QuizDatabase.h"
#include "quiz/QuizContentExporter.h"
#include "quiz/ContentValidationService.h"
#include "quiz/AdminPatchWorkflowService.h"
#include "ui/AdminQuestionEditorDialog.h"
#include "ui/AdminContentMaintenanceWidget.h"

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

    QWidget* contentTab     = new QWidget;
    QWidget* validationTab  = new QWidget;
    QWidget* exportTab      = new QWidget;
    QWidget* statsTab       = new QWidget;
    QWidget* maintenanceTab = new QWidget;

    setupContentTab(contentTab);
    setupValidationTab(validationTab);
    setupExportTab(exportTab);
    setupStatsTab(statsTab);
    setupMaintenanceTab(maintenanceTab);

    m_tabs->addTab(contentTab,     tr("Content"));
    m_tabs->addTab(validationTab,  tr("Validation"));
    m_tabs->addTab(exportTab,      tr("Export"));
    m_tabs->addTab(statsTab,       tr("Stats"));
    m_tabs->addTab(maintenanceTab, tr("Maintenance"));

    mainLayout->addWidget(m_tabs);

    // ── Log view ─────────────────────────────────────────────────────────────
    QLabel* logLabel = new QLabel(tr("Activity log:"), central);
    mainLayout->addWidget(logLabel);

    m_logView = new QTextEdit(central);
    m_logView->setObjectName("adminLogView");
    m_logView->setReadOnly(true);
    m_logView->setPlaceholderText(tr("Admin operations will be logged here."));
    mainLayout->addWidget(m_logView, 1);

    // ── Status bar ───────────────────────────────────────────────────────────
    statusBar()->showMessage(tr("Administrator panel ready."));
}

void QuizAdminPanel::setupContentTab(QWidget* tab)
{
    QPushButton* applyBtn = new QPushButton(tr("Apply Content Updates"), tab);
    applyBtn->setObjectName("adminApplyBtn");
    connect(applyBtn, &QPushButton::clicked, this, &QuizAdminPanel::onApplyContentUpdates);

    createAdminTabLayout(tab,
        tr("Apply incremental SQL content patches to the quiz database.\n"
           "Patches are discovered from the configured patches directory,\n"
           "sorted lexicographically, and applied only once."),
        applyBtn);
}

void QuizAdminPanel::setupValidationTab(QWidget* tab)
{
    QPushButton* validateBtn = new QPushButton(tr("Validate Content"), tab);
    validateBtn->setObjectName("adminValidateBtn");
    connect(validateBtn, &QPushButton::clicked, this, &QuizAdminPanel::onValidateContent);

    createAdminTabLayout(tab,
        tr("Validate the consistency of quiz content: check for questions\n"
           "without options, orphaned records, and broken references."),
        validateBtn);
}

void QuizAdminPanel::setupExportTab(QWidget* tab)
{
    QPushButton* exportBtn = new QPushButton(tr("Export Backup"), tab);
    exportBtn->setObjectName("adminExportBtn");
    connect(exportBtn, &QPushButton::clicked, this, &QuizAdminPanel::onExportBackup);

    createAdminTabLayout(tab,
        tr("Export a full backup of quiz content (questions, options,\n"
           "topics, tags) to a SQL dump file."),
        exportBtn);
}

void QuizAdminPanel::setupStatsTab(QWidget* tab)
{
    QPushButton* refreshBtn = new QPushButton(tr("Refresh Stats"), tab);
    refreshBtn->setObjectName("adminRefreshBtn");
    connect(refreshBtn, &QPushButton::clicked, this, &QuizAdminPanel::refreshStats);

    createAdminTabLayout(tab,
        tr("Live database statistics: question counts, session counts,\n"
           "user counts, and applied content patches."),
        refreshBtn);
}

// ─────────────────────────────────────────────────────────────────────────────
// Tab layout helper
// ─────────────────────────────────────────────────────────────────────────────

QVBoxLayout* QuizAdminPanel::createAdminTabLayout(QWidget* tab,
                                                   const QString& description,
                                                   QPushButton* actionBtn)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    QLabel* info = new QLabel(description, tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    layout->addWidget(actionBtn, 0, Qt::AlignLeft);

    return layout;
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

    ContentValidationService svc;
    const QList<ValidationFinding> findings = svc.validate();

    if (findings.isEmpty()) {
        log(tr("[Validation] Result: OK — no issues found."));
        statusBar()->showMessage(tr("Validation passed."));
        return;
    }

    int errors   = 0;
    int warnings = 0;
    for (const ValidationFinding& f : findings) {
        const QString sev = (f.severity == ValidationSeverity::Error)
                            ? tr("ERROR") : tr("WARN ");
        if (f.severity == ValidationSeverity::Error) ++errors;
        else ++warnings;

        log(tr("[Validation] %1 [%2 id=%3] %4 → %5")
            .arg(sev, f.entityType).arg(f.entityId)
            .arg(f.message, f.suggestedFix));
    }

    log(tr("[Validation] Summary: %1 error(s), %2 warning(s).").arg(errors).arg(warnings));
    if (errors > 0) {
        statusBar()->showMessage(tr("Validation: %1 error(s) found.").arg(errors));
    } else {
        statusBar()->showMessage(tr("Validation: %1 warning(s) found.").arg(warnings));
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

// ─────────────────────────────────────────────────────────────────────────────
// Maintenance tab
// ─────────────────────────────────────────────────────────────────────────────

void QuizAdminPanel::setupMaintenanceTab(QWidget* tab)
{
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    QLabel* info = new QLabel(
        tr("Content Maintenance: create or edit questions and quizzes, "
           "and manage patch workflow with rollback support."), tab);
    info->setWordWrap(true);
    layout->addWidget(info);

    // ── AdminContentMaintenanceWidget ─────────────────────────────────────────
    m_maintenanceWidget = new AdminContentMaintenanceWidget(tab);
    connect(m_maintenanceWidget, &AdminContentMaintenanceWidget::operationCompleted,
            this, [this](const QString& msg) {
                log(tr("[Maintenance] %1").arg(msg));
                statusBar()->showMessage(msg);
            });
    layout->addWidget(m_maintenanceWidget, 1);

    // ── Patch workflow buttons ────────────────────────────────────────────────
    QHBoxLayout* patchBtnLayout = new QHBoxLayout;

    QPushButton* rollbackBtn = new QPushButton(tr("Rollback Last Patch"), tab);
    rollbackBtn->setObjectName("adminRollbackBtn");
    connect(rollbackBtn, &QPushButton::clicked,
            this, &QuizAdminPanel::onRollbackLastPatch);
    patchBtnLayout->addWidget(rollbackBtn);

    QPushButton* journalBtn = new QPushButton(tr("Show Journal History"), tab);
    journalBtn->setObjectName("adminJournalBtn");
    connect(journalBtn, &QPushButton::clicked,
            this, &QuizAdminPanel::onShowJournalHistory);
    patchBtnLayout->addWidget(journalBtn);

    patchBtnLayout->addStretch(1);
    layout->addLayout(patchBtnLayout);
}

// ─────────────────────────────────────────────────────────────────────────────
// Maintenance slots
// ─────────────────────────────────────────────────────────────────────────────

void QuizAdminPanel::onOpenQuestionEditor()
{
    AdminQuestionEditorDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        log(tr("[Maintenance] %1").arg(dlg.resultMessage()));
        statusBar()->showMessage(tr("Question saved."));
    } else {
        log(tr("[Maintenance] Question editor cancelled."));
    }
}

void QuizAdminPanel::onRollbackLastPatch()
{
    const QMessageBox::StandardButton btn = QMessageBox::question(
        this, tr("Rollback Last Patch"),
        tr("This will restore the database to the snapshot taken before the "
           "last applied patch.\n\nAre you sure?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    if (btn != QMessageBox::Yes) {
        log(tr("[Maintenance] Rollback cancelled by user."));
        return;
    }

    log(tr("[Maintenance] Rolling back last patch..."));
    const PatchWorkflowResult r = AdminPatchWorkflowService::instance().rollbackLastPatch();
    if (r.ok) {
        log(tr("[Maintenance] Rollback succeeded. %1").arg(r.message));
        statusBar()->showMessage(tr("Rollback succeeded."));
    } else {
        log(tr("[Maintenance] Rollback FAILED: %1").arg(r.message));
        statusBar()->showMessage(tr("Rollback failed."));
        QMessageBox::critical(this, tr("Rollback Failed"), r.message);
    }
}

void QuizAdminPanel::onShowJournalHistory()
{
    const QStringList lines =
        AdminPatchWorkflowService::instance().journalTail(30);

    if (lines.isEmpty()) {
        log(tr("[Maintenance] Journal is empty (no patch operations recorded yet)."));
        statusBar()->showMessage(tr("Journal empty."));
        return;
    }

    log(tr("[Maintenance] === Recent Admin Actions ==="));
    for (const QString& line : lines)
        log(tr("[Journal] %1").arg(line));
    statusBar()->showMessage(tr("Journal loaded — see log."));
}
