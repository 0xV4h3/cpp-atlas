#include "quiz/ContentPatchService.h"
#include "quiz/QuizDatabase.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
// ─────────────────────────────────────────────────────────────────────────────
// Discovery
// ─────────────────────────────────────────────────────────────────────────────

QList<ContentPatch> ContentPatchService::discoverPatches(const QString& dir) const
{
    QDir patchDir(dir);
    if (!patchDir.exists()) {
        qWarning() << "[ContentPatchService] Patch directory does not exist:" << dir;
        return {};
    }

    // Enumerate *.sql files, sorted lexicographically by filename
    const QStringList entries = patchDir.entryList(
        QStringList() << "*.sql",
        QDir::Files,
        QDir::Name
    );

    QList<ContentPatch> patches;
    patches.reserve(entries.size());

    for (const QString& filename : entries) {
        const QString filePath = patchDir.absoluteFilePath(filename);
        const QString patchId  = QFileInfo(filename).completeBaseName();

        // Compute SHA-256 checksum
        QString checksum;
        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly)) {
            checksum = QString::fromLatin1(
                QCryptographicHash::hash(f.readAll(), QCryptographicHash::Sha256).toHex()
            );
            f.close();
        }

        patches.append(ContentPatch{patchId, filePath, checksum, QString()});
    }

    return patches;
}

// ─────────────────────────────────────────────────────────────────────────────
// Applied-patch queries
// ─────────────────────────────────────────────────────────────────────────────

QList<QString> ContentPatchService::appliedPatchIds() const
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT id FROM content_patches ORDER BY applied_at ASC");
    if (!q.exec()) {
        qWarning() << "[ContentPatchService] Failed to query applied patches:"
                   << q.lastError().text();
        return {};
    }

    QList<QString> ids;
    while (q.next()) {
        ids << q.value(0).toString();
    }
    return ids;
}

bool ContentPatchService::isPatchApplied(const QString& patchId) const
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT 1 FROM content_patches WHERE id = :id");
    q.bindValue(":id", patchId);
    return q.exec() && q.next();
}

// ─────────────────────────────────────────────────────────────────────────────
// Application
// ─────────────────────────────────────────────────────────────────────────────

bool ContentPatchService::applySqlPatchFile(const QString& filePath, bool strict,
                                            QString* error) const
{
    if (!QuizDatabase::instance().isOpen()) {
        if (error) *error = "Database is not open.";
        return false;
    }

    // Delegate to the single canonical SQL-file execution path in QuizDatabase,
    // which owns the SQL parser.  This avoids maintaining a duplicate parser.
    if (!QuizDatabase::instance().runSqlFile(filePath, strict)) {
        if (error) *error = QuizDatabase::instance().lastError().text();
        return false;
    }
    return true;
}

bool ContentPatchService::applyPendingPatches(const QList<ContentPatch>& patches,
                                              QString* error)
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);

    for (const ContentPatch& patch : patches) {
        if (isPatchApplied(patch.id)) {
            qDebug() << "[ContentPatchService] Skipping already-applied patch:" << patch.id;
            continue;
        }

        qDebug() << "[ContentPatchService] Applying patch:" << patch.id;

        db.transaction();

        QString patchError;
        const bool ok = applySqlPatchFile(patch.path, /*strict=*/true, &patchError);

        if (!ok) {
            db.rollback();
            const QString msg = QString("Patch '%1' failed: %2").arg(patch.id, patchError);
            if (error) *error = msg;
            qWarning() << "[ContentPatchService]" << msg;
            return false;
        }

        // Record the patch as applied
        QSqlQuery record(db);
        record.prepare(
            "INSERT INTO content_patches (id, description, checksum) "
            "VALUES (:id, :desc, :checksum)"
        );
        record.bindValue(":id",       patch.id);
        record.bindValue(":desc",     patch.description);
        record.bindValue(":checksum", patch.checksum);

        if (!record.exec()) {
            db.rollback();
            const QString msg = QString("Failed to record patch '%1': %2")
                                    .arg(patch.id, record.lastError().text());
            if (error) *error = msg;
            qWarning() << "[ContentPatchService]" << msg;
            return false;
        }

        if (!db.commit()) {
            db.rollback();
            const QString msg = QString("Failed to commit patch '%1': %2")
                                    .arg(patch.id, db.lastError().text());
            if (error) *error = msg;
            qWarning() << "[ContentPatchService]" << msg;
            return false;
        }
        qDebug() << "[ContentPatchService] Applied patch:" << patch.id;
    }

    return true;
}
