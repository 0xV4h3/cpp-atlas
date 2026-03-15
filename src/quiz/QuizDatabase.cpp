#include "quiz/QuizDatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>

QuizDatabase& QuizDatabase::instance()
{
    static QuizDatabase s_instance;
    return s_instance;
}

QuizDatabase::QuizDatabase(QObject* parent)
    : QObject(parent)
{
}

bool QuizDatabase::initialize()
{
    if (!openDatabase()) {
        qCritical() << "[QuizDatabase] Failed to open database:" << m_lastError.text();
        return false;
    }

    if (!applySchema()) {
        qCritical() << "[QuizDatabase] Failed to apply schema:" << m_lastError.text();
        return false;
    }

    if (needsSeed()) {
        qDebug() << "[QuizDatabase] Seeding initial data...";
        if (!applySeed()) {
            qWarning() << "[QuizDatabase] Seed failed (non-fatal):" << m_lastError.text();
            // Non-fatal: app works without seed data
        }
    }

    qDebug() << "[QuizDatabase] Initialized at:" << m_dbPath
             << "| Schema version:" << currentSchemaVersion();
    return true;
}

void QuizDatabase::shutdown()
{
    {
        QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME, false);
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(CONNECTION_NAME);
    qDebug() << "[QuizDatabase] Connection closed.";
}

bool QuizDatabase::isOpen() const
{
    return QSqlDatabase::database(CONNECTION_NAME, false).isOpen();
}

QSqlError QuizDatabase::lastError() const
{
    return m_lastError;
}

QString QuizDatabase::databasePath() const
{
    return m_dbPath;
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

bool QuizDatabase::openDatabase()
{
    // Store DB in platform-appropriate app data directory
    const QString dataDir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(dataDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        m_lastError = QSqlError("Cannot create app data directory: " + dataDir,
                                QString(), QSqlError::ConnectionError);
        return false;
    }

    m_dbPath = dataDir + QDir::separator() + "cppatlas.db";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    db.setDatabaseName(m_dbPath);

    if (!db.open()) {
        m_lastError = db.lastError();
        QSqlDatabase::removeDatabase(CONNECTION_NAME);
        return false;
    }

    // Enable WAL mode and foreign keys for better performance and integrity
    QSqlQuery pragma(db);
    pragma.exec("PRAGMA journal_mode=WAL;");
    pragma.exec("PRAGMA foreign_keys=ON;");
    pragma.exec("PRAGMA synchronous=NORMAL;");

    return true;
}

bool QuizDatabase::applySchema()
{
    if (!runSqlFile(":/db/schema.sql", false)) return false;
    return applyMigrations();
}

bool QuizDatabase::applyMigrations()
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);

    // Migration v2: add avatar_path column to users table.
    // New installs already have it from schema.sql; this runs for existing DBs.
    // Use PRAGMA table_info to check idempotently (safe to run every time).
    {
        QSqlQuery check(db);
        check.exec("PRAGMA table_info(users)");
        bool hasAvatarPath = false;
        while (check.next()) {
            if (check.value("name").toString() == "avatar_path") {
                hasAvatarPath = true;
                break;
            }
        }
        if (!hasAvatarPath) {
            QSqlQuery alter(db);
            if (!alter.exec("ALTER TABLE users ADD COLUMN avatar_path TEXT DEFAULT NULL")) {
                qWarning() << "[QuizDatabase] Migration v2 failed:"
                           << alter.lastError().text();
                return false;
            }
            qDebug() << "[QuizDatabase] Applied migration to schema v2";
        }
        QSqlQuery ver2(db);
        ver2.prepare("INSERT OR IGNORE INTO schema_version (version, description) "
                     "VALUES (2, 'Add avatar_path to users table')");
        ver2.exec();
    }

    // Migration v3: add content_patches table for incremental content tracking.
    {
        QSqlQuery check(db);
        if (!check.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='content_patches'")) {
            qWarning() << "[QuizDatabase] Migration v3 check failed:"
                       << check.lastError().text();
            return false;
        }
        if (!check.next()) {
            QSqlQuery create(db);
            if (!create.exec(
                    "CREATE TABLE content_patches ("
                    "  id          TEXT PRIMARY KEY,"
                    "  applied_at  DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "  description TEXT,"
                    "  checksum    TEXT"
                    ")")) {
                qWarning() << "[QuizDatabase] Migration v3 failed (content_patches table):"
                           << create.lastError().text();
                return false;
            }
            create.exec(
                "CREATE INDEX IF NOT EXISTS idx_content_patches_applied_at "
                "ON content_patches(applied_at)"
            );
            qDebug() << "[QuizDatabase] Applied migration to schema v3";
        }
        QSqlQuery ver3(db);
        ver3.prepare("INSERT OR IGNORE INTO schema_version (version, description) "
                     "VALUES (3, 'Add content_patches table')");
        ver3.exec();
    }

    return true;
}

bool QuizDatabase::needsSeed() const
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
    QSqlQuery q(db);
    q.exec("SELECT COUNT(*) FROM questions");
    if (q.next()) {
        return q.value(0).toInt() == 0;
    }
    return true;
}

bool QuizDatabase::applySeed()
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
    db.transaction();
    const bool ok = runSqlFile(":/db/seed_data.sql", false);
    if (ok) db.commit();
    else    db.rollback();
    return ok;
}

bool QuizDatabase::runSqlFile(const QString& resourceOrFilePath, bool strict)
{
    QFile file(resourceOrFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QSqlError("Cannot open resource: " + resourceOrFilePath,
                                QString(), QSqlError::ConnectionError);
        return false;
    }
    const QString sql = QTextStream(&file).readAll();
    file.close();

    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);

    // ── SQL-aware split: respects single-quoted strings and -- comments ───────
    // Rule: ';' is a statement terminator ONLY when outside a quoted string.
    // Single-quoted strings: '...' — the only literal type in SQLite.
    // Escaped apostrophe inside string: '' (two single quotes) — NOT end of string.
    // Line comments: -- until newline — ';' inside is ignored.
    QStringList stmts;
    QString     cur;
    bool        inStr     = false;  // inside '...'
    bool        inComment = false;  // inside -- comment

    for (int i = 0, n = sql.size(); i < n; ++i) {
        const QChar c = sql[i];

        if (c == '\n') {
            inComment = false;          // newline ends line comment
            cur += c;
            continue;
        }
        if (inComment) { cur += c; continue; }

        if (!inStr) {
            if (c == '-' && i+1 < n && sql[i+1] == '-') {
                inComment = true;
                cur += c;
                continue;
            }
            if (c == '\'') { inStr = true;  cur += c; continue; }
            if (c == ';')  {
                const QString s = cur.trimmed();
                if (!s.isEmpty()) stmts << s;
                cur.clear();
                continue;
            }
        } else {
            // Inside string: '' = escaped quote (stay in string), else end string
            if (c == '\'' && i+1 < n && sql[i+1] == '\'') {
                cur += c; cur += sql[++i]; // consume both, stay inStr
                continue;
            }
            if (c == '\'') { inStr = false; cur += c; continue; }
        }
        cur += c;
    }
    if (const QString s = cur.trimmed(); !s.isEmpty()) stmts << s;

    // ── Execute ───────────────────────────────────────────────────────────────
    for (const QString& stmt : stmts) {
        // Skip pure-comment blocks that survived (start with --)
        // Trim leading whitespace+comments line by line
        QString clean;
        for (const QString& line : stmt.split('\n')) {
            const QString t = line.trimmed();
            if (!t.startsWith("--")) clean += line + '\n';
        }
        clean = clean.trimmed();
        if (clean.isEmpty()) continue;

        QSqlQuery q(db);
        if (!q.exec(clean)) {
            if (strict) {
                m_lastError = q.lastError();
                qWarning() << "[QuizDatabase] Error in" << resourceOrFilePath
                           << ":" << m_lastError.text()
                           << "\n  Statement:" << clean.left(120);
                return false;
            }
            qWarning() << "[QuizDatabase] Warning in" << resourceOrFilePath
                       << ":" << q.lastError().text()
                       << "\n  Statement:" << clean.left(120);
        }
    }
    return true;
}

int QuizDatabase::currentSchemaVersion() const
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
    QSqlQuery q(db);
    q.exec("SELECT MAX(version) FROM schema_version");
    if (q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}
