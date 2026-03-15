#pragma once
#include <QString>
#include <QList>

/**
 * @brief Represents a single incremental SQL content patch.
 *
 * The patch id is derived from the filename without extension.
 * Patches are sorted and applied lexicographically.
 */
struct ContentPatch {
    QString id;           ///< Unique patch identifier (filename without extension)
    QString path;         ///< Absolute filesystem path to the .sql file
    QString checksum;     ///< SHA-256 hex digest of the file contents
    QString description;  ///< Human-readable description (optional)
};

/**
 * @brief Service for discovering and applying incremental SQL content patches.
 *
 * Patches are *.sql files in a directory, applied in lexicographic filename
 * order.  Each successfully applied patch is recorded in the
 * `content_patches` database table so it is never re-applied.
 *
 * Usage:
 * @code
 *   ContentPatchService svc;
 *   auto patches = svc.discoverPatches("/path/to/patches");
 *   QString error;
 *   if (!svc.applyPendingPatches(patches, &error)) {
 *       qWarning() << "Patch error:" << error;
 *   }
 * @endcode
 */
class ContentPatchService
{
public:
    /**
     * @brief Discover all *.sql patch files in @p dir, sorted lexicographically.
     * The patch id is the filename without extension.
     */
    QList<ContentPatch> discoverPatches(const QString& dir) const;

    /**
     * @brief Returns the ids of all patches that have already been applied.
     */
    QList<QString> appliedPatchIds() const;

    /**
     * @brief Returns true if a patch with the given id has already been applied.
     */
    bool isPatchApplied(const QString& patchId) const;

    /**
     * @brief Apply a single SQL patch file.
     * @param filePath  Absolute path to the .sql file.
     * @param strict    When true, the first SQL error stops execution.
     * @param error     If non-null, receives an error description on failure.
     * @return true on success.
     */
    bool applySqlPatchFile(const QString& filePath, bool strict,
                           QString* error = nullptr) const;

    /**
     * @brief Apply all pending (not yet applied) patches from @p patches.
     *
     * Each patch is wrapped in a transaction. A failed patch is rolled back
     * and is not recorded as applied.  Processing stops at the first failure.
     *
     * @param patches  Ordered list of patches (typically from discoverPatches()).
     * @param error    If non-null, receives an error description on failure.
     * @return true if all pending patches were applied successfully.
     */
    bool applyPendingPatches(const QList<ContentPatch>& patches,
                             QString* error = nullptr);
};
