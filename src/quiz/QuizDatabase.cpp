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
    return runSqlFile(":/db/schema.sql");
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
    return runSqlFile(":/db/seed_data.sql");
}

bool QuizDatabase::runSqlFile(const QString& resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QSqlError("Cannot open resource: " + resourcePath,
                                QString(), QSqlError::ConnectionError);
        return false;
    }

    QTextStream stream(&file);
    const QString content = stream.readAll();
    file.close();

    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);

    // Split on semicolons, skip empty and comment-only statements
    const QStringList statements = content.split(';', Qt::SkipEmptyParts);

    db.transaction();
    for (const QString& rawStmt : statements) {
        // Strip comments and whitespace
        QString stmt = rawStmt;

        // Remove single-line SQL comments (-- ...)
        QStringList lines = stmt.split('\n');
        QStringList cleanLines;
        for (const QString& line : lines) {
            const QString trimmed = line.trimmed();
            if (!trimmed.startsWith("--") && !trimmed.isEmpty()) {
                cleanLines << line;
            }
        }
        stmt = cleanLines.join('\n').trimmed();

        if (stmt.isEmpty()) continue;

        QSqlQuery q(db);
        if (!q.exec(stmt)) {
            // Log but don't fail on "already exists" errors (IF NOT EXISTS handles most)
            QSqlError err = q.lastError();
            if (err.nativeErrorCode() != "1") { // 1 = generic, often "table already exists"
                qWarning() << "[QuizDatabase] SQL warning in" << resourcePath
                           << ":" << err.text()
                           << "\nStatement:" << stmt.left(120);
            }
        }
    }

    if (!db.commit()) {
        m_lastError = db.lastError();
        db.rollback();
        return false;
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