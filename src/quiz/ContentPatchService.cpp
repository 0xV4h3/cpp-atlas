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

    // Run the SQL file through QuizDatabase using strict mode
    // We call the private method indirectly through a temporary approach:
    // QuizDatabase::runSqlFile is private, so we replicate the call here
    // using the public API surface. The actual execution is delegated to
    // QuizDatabase by applying the file via the same connection.

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString msg = QString("Cannot open patch file: %1").arg(filePath);
        if (error) *error = msg;
        qWarning() << "[ContentPatchService]" << msg;
        return false;
    }
    const QString sql = QString::fromUtf8(f.readAll());
    f.close();

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);

    // ── SQL-aware split (same logic as QuizDatabase::runSqlFile) ─────────────
    QStringList stmts;
    QString cur;
    bool inStr     = false;
    bool inComment = false;

    for (int i = 0, n = sql.size(); i < n; ++i) {
        const QChar c = sql[i];
        if (c == '\n') {
            inComment = false;
            cur += c;
            continue;
        }
        if (inComment) { cur += c; continue; }
        if (!inStr) {
            if (c == '-' && i + 1 < n && sql[i + 1] == '-') {
                inComment = true; cur += c; continue;
            }
            if (c == '\'') { inStr = true;  cur += c; continue; }
            if (c == ';') {
                const QString s = cur.trimmed();
                if (!s.isEmpty()) stmts << s;
                cur.clear();
                continue;
            }
        } else {
            if (c == '\'' && i + 1 < n && sql[i + 1] == '\'') {
                cur += c; cur += sql[++i]; continue;
            }
            if (c == '\'') { inStr = false; cur += c; continue; }
        }
        cur += c;
    }
    if (const QString s = cur.trimmed(); !s.isEmpty()) stmts << s;

    // ── Execute ───────────────────────────────────────────────────────────────
    for (const QString& stmt : stmts) {
        QString clean;
        for (const QString& line : stmt.split('\n')) {
            if (!line.trimmed().startsWith("--")) clean += line + '\n';
        }
        clean = clean.trimmed();
        if (clean.isEmpty()) continue;

        QSqlQuery q(db);
        if (!q.exec(clean)) {
            const QString msg = QString("SQL error in %1: %2\n  Statement: %3")
                                    .arg(filePath, q.lastError().text(), clean.left(120));
            if (strict) {
                if (error) *error = msg;
                qWarning() << "[ContentPatchService]" << msg;
                return false;
            }
            qWarning() << "[ContentPatchService] Warning —" << msg;
        }
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

        db.commit();
        qDebug() << "[ContentPatchService] Applied patch:" << patch.id;
    }

    return true;
}
