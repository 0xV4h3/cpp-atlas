#include "quiz/AdminPatchWorkflowService.h"
#include "quiz/QuizDatabase.h"
#include "quiz/ContentPatchService.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Singleton
// ─────────────────────────────────────────────────────────────────────────────

AdminPatchWorkflowService& AdminPatchWorkflowService::instance()
{
    static AdminPatchWorkflowService inst;
    return inst;
}

AdminPatchWorkflowService::AdminPatchWorkflowService(QObject* parent)
    : QObject(parent)
{}

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────

static QSqlDatabase wfDb()
{
    return QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
}

static QString snapshotDir()
{
    const QString base = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir dir(base);
    dir.mkpath("snapshots");
    return dir.absoluteFilePath("snapshots");
}

void AdminPatchWorkflowService::writeJournalEntry(const QString& patchId,
                                                   const QString& action,
                                                   const QString& snapshotPath,
                                                   const QString& status,
                                                   const QString& details) const
{
    QSqlDatabase db = wfDb();
    // Silently skip journal write if the table hasn't been created yet
    // (i.e. the 2026_03_16_admin_patch_journal patch hasn't been applied).
    QSqlQuery check(db);
    check.exec("SELECT name FROM sqlite_master "
               "WHERE type='table' AND name='admin_patch_journal'");
    if (!check.next()) return;

    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO admin_patch_journal "
        "(patch_id, action, snapshot_path, status, details) "
        "VALUES (:pid, :act, :snap, :stat, :det)"
    );
    q.bindValue(":pid",  patchId);
    q.bindValue(":act",  action);
    q.bindValue(":snap", snapshotPath);
    q.bindValue(":stat", status);
    q.bindValue(":det",  details);
    if (!q.exec())
        qWarning() << "[AdminPatchWorkflowService] journal write failed:"
                   << q.lastError().text();
}

// ─────────────────────────────────────────────────────────────────────────────
// Snapshot
// ─────────────────────────────────────────────────────────────────────────────

QString AdminPatchWorkflowService::createSnapshot(const QString& label) const
{
    const QString srcPath = QuizDatabase::instance().databasePath();
    if (srcPath.isEmpty() || srcPath == ":memory:") {
        qWarning() << "[AdminPatchWorkflowService] Cannot snapshot in-memory database.";
        return QString();
    }

    const QString ts   = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    const QString name = QString("cppatlas_%1_%2.db.bak").arg(label, ts);
    const QString dest = QDir(snapshotDir()).absoluteFilePath(name);

    if (!QFile::copy(srcPath, dest)) {
        qWarning() << "[AdminPatchWorkflowService] Failed to copy snapshot to" << dest;
        return QString();
    }

    qDebug() << "[AdminPatchWorkflowService] Snapshot created:" << dest;
    return dest;
}

// ─────────────────────────────────────────────────────────────────────────────
// Apply
// ─────────────────────────────────────────────────────────────────────────────

PatchWorkflowResult AdminPatchWorkflowService::applyPatch(const QString& patchId,
                                                           const QString& patchFilePath)
{
    // 1. Create pre-apply snapshot
    const QString snap = createSnapshot(patchId);
    if (snap.isEmpty() && !QuizDatabase::instance().databasePath().isEmpty()
                       && QuizDatabase::instance().databasePath() != ":memory:") {
        const QString msg = "Failed to create pre-apply snapshot — aborting for safety.";
        writeJournalEntry(patchId, "APPLY", QString(), "FAIL", msg);
        return {false, msg, QString()};
    }

    // 2. Apply the patch via ContentPatchService
    ContentPatchService svc;
    const QList<ContentPatch> patches = svc.discoverPatches(
        QFileInfo(patchFilePath).absolutePath());

    // Filter to the single requested patch
    ContentPatch target;
    for (const ContentPatch& p : patches) {
        if (p.id == patchId) { target = p; break; }
    }
    if (target.id.isEmpty()) {
        // Try direct application by path
        target.id   = patchId;
        target.path = patchFilePath;
    }

    if (svc.isPatchApplied(patchId)) {
        const QString msg = QString("Patch '%1' is already applied.").arg(patchId);
        writeJournalEntry(patchId, "APPLY", snap, "FAIL", msg);
        return {false, msg, snap};
    }

    QString applyError;
    const QList<ContentPatch> single = {target};
    const bool ok = svc.applyPendingPatches(single, &applyError);

    if (!ok) {
        // Auto-restore snapshot
        const PatchWorkflowResult restoreResult = restoreSnapshot(snap);
        const QString details = QString("Apply failed: %1. Auto-restore %2.")
            .arg(applyError, restoreResult.ok ? "succeeded" : "failed");
        writeJournalEntry(patchId, "APPLY", snap, "FAIL", details);
        return {false, details, snap};
    }

    writeJournalEntry(patchId, "APPLY", snap, "OK",
                      QString("Applied successfully."));
    return {true, QString("Patch '%1' applied.").arg(patchId), snap};
}

// ─────────────────────────────────────────────────────────────────────────────
// Rollback / restore
// ─────────────────────────────────────────────────────────────────────────────

PatchWorkflowResult AdminPatchWorkflowService::rollbackLastPatch()
{
    QSqlDatabase db = wfDb();

    // Locate the most recent successful APPLY journal entry
    QSqlQuery q(db);
    q.exec(
        "SELECT patch_id, snapshot_path FROM admin_patch_journal "
        "WHERE action = 'APPLY' AND status = 'OK' "
        "ORDER BY id DESC LIMIT 1"
    );

    if (!q.next()) {
        return {false, "No applied patch found in journal.", QString()};
    }

    const QString patchId  = q.value(0).toString();
    const QString snapPath = q.value(1).toString();

    if (snapPath.isEmpty()) {
        return {false, "Journal entry for last patch has no snapshot path.", QString()};
    }

    const PatchWorkflowResult r = restoreSnapshot(snapPath);
    if (r.ok) {
        // Remove the applied patch record so it can be re-applied later
        QSqlQuery del(db);
        del.prepare("DELETE FROM content_patches WHERE id = :id");
        del.bindValue(":id", patchId);
        del.exec();
        writeJournalEntry(patchId, "ROLLBACK", snapPath, "OK",
                          "Rollback from snapshot succeeded.");
    } else {
        writeJournalEntry(patchId, "ROLLBACK", snapPath, "FAIL", r.message);
    }

    return r;
}

PatchWorkflowResult AdminPatchWorkflowService::restoreSnapshot(
    const QString& snapshotPath)
{
    if (snapshotPath.isEmpty() || !QFile::exists(snapshotPath)) {
        return {false, QString("Snapshot file not found: %1").arg(snapshotPath),
                snapshotPath};
    }

    const QString dbPath = QuizDatabase::instance().databasePath();
    if (dbPath.isEmpty() || dbPath == ":memory:") {
        return {false, "Cannot restore to in-memory database.", snapshotPath};
    }

    // Close all connections before overwriting the file
    QuizDatabase::instance().shutdown();

    // Remove current db and copy snapshot over
    QFile::remove(dbPath);
    const bool copied = QFile::copy(snapshotPath, dbPath);
    if (!copied) {
        // Attempt to re-open even on failure so the app doesn't deadlock
        QuizDatabase::instance().initialize();
        return {false, QString("Failed to copy snapshot to %1").arg(dbPath), snapshotPath};
    }

    // Re-open database
    if (!QuizDatabase::instance().initialize()) {
        return {false, "Snapshot copied but failed to re-open database.", snapshotPath};
    }

    qDebug() << "[AdminPatchWorkflowService] Restored snapshot:" << snapshotPath;
    return {true, "Snapshot restored successfully.", snapshotPath};
}

// ─────────────────────────────────────────────────────────────────────────────
// Journal tail
// ─────────────────────────────────────────────────────────────────────────────

QStringList AdminPatchWorkflowService::journalTail(int n) const
{
    QStringList lines;
    QSqlDatabase db = wfDb();

    QSqlQuery check(db);
    check.exec("SELECT name FROM sqlite_master "
               "WHERE type='table' AND name='admin_patch_journal'");
    if (!check.next()) return lines;

    QSqlQuery q(db);
    q.prepare(
        "SELECT created_at, action, patch_id, status, details "
        "FROM admin_patch_journal "
        "ORDER BY id DESC LIMIT :n"
    );
    q.bindValue(":n", n);
    if (!q.exec()) return lines;

    while (q.next()) {
        lines.prepend(QString("[%1] %2 %3 — %4  %5")
            .arg(q.value(0).toString(),
                 q.value(1).toString(),
                 q.value(2).toString(),
                 q.value(3).toString(),
                 q.value(4).toString()));
    }
    return lines;
}
