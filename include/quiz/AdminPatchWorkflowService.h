#pragma once
#include <QObject>
#include <QString>

/**
 * @brief Result of a patch workflow operation (apply or rollback).
 */
struct PatchWorkflowResult {
    bool    ok           = false;
    QString message;
    QString snapshotPath;  ///< Path to the pre-apply snapshot (on success / for rollback)
};

/**
 * @brief Safe patch apply / rollback service with pre-apply snapshots.
 *
 * Every apply operation:
 *  1. Creates a timestamped SQLite file-copy snapshot.
 *  2. Delegates to ContentPatchService for the actual SQL execution.
 *  3. Writes a record to the @c admin_patch_journal table (OK or FAIL).
 *  4. On failure: automatically restores the snapshot and records ROLLBACK.
 *
 * Snapshot files are written to QStandardPaths::AppDataLocation/snapshots/.
 * Each snapshot is named @c cppatlas_<patchId>_<timestamp>.db.bak.
 *
 * Usage:
 * @code
 *   AdminPatchWorkflowService& svc = AdminPatchWorkflowService::instance();
 *   PatchWorkflowResult r = svc.applyPatch(patch);
 *   if (!r.ok) { ... }
 *   // Later:
 *   svc.rollbackLastPatch();
 * @endcode
 */
class AdminPatchWorkflowService : public QObject
{
    Q_OBJECT
public:
    static AdminPatchWorkflowService& instance();

    /**
     * @brief Create a named snapshot of the current database file.
     *
     * @param label  Human-readable label included in the snapshot filename.
     * @return Path to the created snapshot file, or empty on failure.
     */
    QString createSnapshot(const QString& label = "manual") const;

    /**
     * @brief Apply a single patch with pre-apply snapshot + journal entry.
     *
     * The @p patchId and @p patchFilePath come from a ContentPatch struct.
     * On failure the snapshot is automatically restored.
     */
    PatchWorkflowResult applyPatch(const QString& patchId,
                                   const QString& patchFilePath);

    /**
     * @brief Rollback the most recently applied patch by restoring its snapshot.
     *
     * Looks up the latest OK APPLY record in @c admin_patch_journal and
     * restores the associated snapshot.
     *
     * @return PatchWorkflowResult with ok=true on success.
     */
    PatchWorkflowResult rollbackLastPatch();

    /**
     * @brief Restore the database from a specific snapshot file.
     *
     * @param snapshotPath  Absolute path to the .db.bak snapshot file.
     * @return PatchWorkflowResult with ok=true on success.
     */
    PatchWorkflowResult restoreSnapshot(const QString& snapshotPath);

    /**
     * @brief Return the last N journal entries as formatted log lines.
     *
     * Format: "[created_at] action patch_id — status  details"
     */
    QStringList journalTail(int n = 20) const;

private:
    explicit AdminPatchWorkflowService(QObject* parent = nullptr);

    void writeJournalEntry(const QString& patchId,
                           const QString& action,
                           const QString& snapshotPath,
                           const QString& status,
                           const QString& details) const;
};
