#include "QuizAdminCli.h"

#include "quiz/QuizDatabase.h"
#include "quiz/ContentPatchService.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

QuizAdminCli::QuizAdminCli()
    : m_out(stdout)
    , m_err(stderr)
{
}

// ─────────────────────────────────────────────────────────────────────────────
// run — parse args and dispatch
// ─────────────────────────────────────────────────────────────────────────────

int QuizAdminCli::run(const QStringList& args)
{
    QString     dbPath;
    QString     command;
    QStringList commandArgs;

    // Skip args[0] (program name)
    for (int i = 1; i < args.size(); ++i) {
        const QString& arg = args[i];

        if (arg == "--help" || arg == "-h") {
            printUsage(m_out);
            return 0;
        }

        if (arg == "--db") {
            if (++i >= args.size()) {
                m_err << "error: --db requires a path argument\n";
                m_err.flush();
                return 1;
            }
            dbPath = args[i];
            continue;
        }

        if (command.isEmpty()) {
            command = arg;
        } else {
            commandArgs << arg;
        }
    }

    if (command.isEmpty()) {
        m_err << "error: no command specified\n\n";
        m_err.flush();
        printUsage(m_err);
        return 1;
    }

    if (!openDatabase(dbPath))
        return 2;

    if (command == "stats")
        return cmdStats();

    if (command == "validate") {
        QString contentDir;
        for (int i = 0; i < commandArgs.size(); ++i) {
            if (commandArgs[i] == "--content-dir" && i + 1 < commandArgs.size()) {
                contentDir = commandArgs[++i];
            }
        }
        if (contentDir.isEmpty()) {
            m_err << "error: validate requires --content-dir <dir>\n";
            m_err.flush();
            return 1;
        }
        return cmdValidate(contentDir);
    }

    if (command == "apply-content") {
        QString contentDir;
        for (int i = 0; i < commandArgs.size(); ++i) {
            if (commandArgs[i] == "--content-dir" && i + 1 < commandArgs.size()) {
                contentDir = commandArgs[++i];
            }
        }
        if (contentDir.isEmpty()) {
            m_err << "error: apply-content requires --content-dir <dir>\n";
            m_err.flush();
            return 1;
        }
        return cmdApplyContent(contentDir);
    }

    if (command == "export") {
        QString outFile;
        for (int i = 0; i < commandArgs.size(); ++i) {
            if (commandArgs[i] == "--out" && i + 1 < commandArgs.size()) {
                outFile = commandArgs[++i];
            }
        }
        if (outFile.isEmpty()) {
            m_err << "error: export requires --out <file>\n";
            m_err.flush();
            return 1;
        }
        return cmdExport(outFile);
    }

    m_err << "error: unknown command '" << command << "'\n\n";
    m_err.flush();
    printUsage(m_err);
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// openDatabase
// ─────────────────────────────────────────────────────────────────────────────

bool QuizAdminCli::openDatabase(const QString& dbPath)
{
    QString resolvedPath = dbPath;
    if (resolvedPath.isEmpty()) {
        const QString dataDir =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        resolvedPath = dataDir + QDir::separator() + "cppatlas.db";
    }

    if (!QFileInfo::exists(resolvedPath)) {
        m_err << "error: database file not found: " << resolvedPath << "\n";
        m_err.flush();
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QuizDatabase::CONNECTION_NAME);
    db.setDatabaseName(resolvedPath);
    if (!db.open()) {
        m_err << "error: cannot open database: " << db.lastError().text() << "\n";
        m_err.flush();
        return false;
    }

    // Read-safe pragmas
    QSqlQuery pragma(db);
    if (!pragma.exec("PRAGMA foreign_keys=ON;")) {
        m_err << "warning: failed to enable foreign keys: "
              << pragma.lastError().text() << "\n";
    }

    m_out << "Database: " << resolvedPath << "\n";
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

namespace {

/// Execute @p sql against the CppAtlas connection and return the single integer
/// result, or -1 on error.
int queryInt(const QString& sql)
{
    QSqlQuery q(QSqlDatabase::database(QuizDatabase::CONNECTION_NAME));
    if (q.exec(sql) && q.next())
        return q.value(0).toInt();
    return -1;
}

/// Format a count value: returns the number as string, or @p fallback on error (-1).
QString fmtCount(int n, const QString& fallback = "n/a")
{
    return n >= 0 ? QString::number(n) : fallback;
}

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
// cmdStats
// ─────────────────────────────────────────────────────────────────────────────

int QuizAdminCli::cmdStats()
{
    m_out << "\n=== CppAtlas Quiz Database Statistics ===\n\n";

    const int schemaVer = queryInt("SELECT MAX(version) FROM schema_version");
    m_out << "Schema version : " << fmtCount(schemaVer, "unknown") << "\n\n";

    m_out << "Content:\n";
    m_out << "  Topics    : " << fmtCount(queryInt("SELECT COUNT(*) FROM topics")) << "\n";
    m_out << "  Quizzes   : " << fmtCount(queryInt("SELECT COUNT(*) FROM quizzes WHERE is_active = 1")) << "\n";
    m_out << "  Questions : " << fmtCount(queryInt("SELECT COUNT(*) FROM questions WHERE is_active = 1")) << "\n";
    m_out << "  Tags      : " << fmtCount(queryInt("SELECT COUNT(*) FROM tags")) << "\n\n";

    m_out << "Users:\n";
    m_out << "  Total  : " << fmtCount(queryInt("SELECT COUNT(*) FROM users")) << "\n";
    m_out << "  Admins : " << fmtCount(queryInt("SELECT COUNT(*) FROM users WHERE is_admin = 1")) << "\n\n";

    m_out << "Sessions:\n";
    m_out << "  Total     : " << fmtCount(queryInt("SELECT COUNT(*) FROM quiz_sessions")) << "\n";
    m_out << "  Completed : " << fmtCount(queryInt("SELECT COUNT(*) FROM quiz_sessions WHERE is_complete = 1")) << "\n\n";

    const int patches = queryInt("SELECT COUNT(*) FROM content_patches");
    m_out << "Content Patches:\n";
    m_out << "  Applied : " << fmtCount(patches, "n/a (table not found)") << "\n";

    m_out.flush();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmdValidate
// ─────────────────────────────────────────────────────────────────────────────

int QuizAdminCli::cmdValidate(const QString& contentDir)
{
    m_out << "\n=== CppAtlas Quiz Content Validation ===\n\n";
    m_out << "Content directory : " << contentDir << "\n\n";

    // ── Patch status ──────────────────────────────────────────────────────────
    ContentPatchService svc;
    const QList<ContentPatch> patches = svc.discoverPatches(contentDir);

    if (patches.isEmpty()) {
        m_out << "No *.sql patch files found in: " << contentDir << "\n";
        m_out.flush();
        return 0;
    }

    m_out << "Patches (" << patches.size() << " total):\n";
    int appliedCount = 0, pendingCount = 0;
    for (const ContentPatch& p : patches) {
        const bool applied = svc.isPatchApplied(p.id);
        if (applied) ++appliedCount; else ++pendingCount;
        m_out << "  " << p.id << ".sql  ["
              << (applied ? "APPLIED" : "PENDING") << "]\n";
    }
    m_out << "\n  Applied : " << appliedCount
          << "  Pending : " << pendingCount
          << "  Total : "   << patches.size() << "\n\n";

    // ── Content integrity ─────────────────────────────────────────────────────
    m_out << "Content integrity:\n";

    const int mcqNoOptions = queryInt(
        "SELECT COUNT(*) FROM questions q "
        "WHERE q.type = 'mcq' AND q.is_active = 1 "
        "AND NOT EXISTS ("
        "  SELECT 1 FROM options o WHERE o.question_id = q.id"
        ")");
    m_out << "  MCQ questions without options  : " << fmtCount(mcqNoOptions) << "\n";

    const int orphanOptions = queryInt(
        "SELECT COUNT(*) FROM options o "
        "WHERE NOT EXISTS ("
        "  SELECT 1 FROM questions q WHERE q.id = o.question_id"
        ")");
    m_out << "  Orphan options                 : " << fmtCount(orphanOptions) << "\n";

    const int mcqNoCorrect = queryInt(
        "SELECT COUNT(*) FROM questions q "
        "WHERE q.type = 'mcq' AND q.is_active = 1 "
        "AND NOT EXISTS ("
        "  SELECT 1 FROM options o WHERE o.question_id = q.id AND o.is_correct = 1"
        ")");
    m_out << "  MCQ questions without correct  : " << fmtCount(mcqNoCorrect) << "\n\n";

    const bool hasWarnings =
        (mcqNoOptions > 0 || orphanOptions > 0 || mcqNoCorrect > 0);
    m_out << "Result: " << (hasWarnings ? "WARNINGS FOUND" : "OK") << "\n";
    m_out.flush();
    return hasWarnings ? 2 : 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmdApplyContent
// ─────────────────────────────────────────────────────────────────────────────

int QuizAdminCli::cmdApplyContent(const QString& contentDir)
{
    m_out << "\n=== CppAtlas Apply Content Patches ===\n\n";
    m_out << "Content directory : " << contentDir << "\n\n";

    ContentPatchService svc;
    const QList<ContentPatch> patches = svc.discoverPatches(contentDir);

    if (patches.isEmpty()) {
        m_out << "No *.sql patch files found in: " << contentDir << "\n";
        m_out.flush();
        return 0;
    }

    int alreadyApplied = 0;
    int pending = 0;
    for (const ContentPatch& p : patches) {
        if (svc.isPatchApplied(p.id)) ++alreadyApplied;
        else ++pending;
    }

    m_out << "Patches total   : " << patches.size() << "\n";
    m_out << "Already applied : " << alreadyApplied << "\n";
    m_out << "Pending         : " << pending << "\n\n";

    if (pending == 0) {
        m_out << "All patches already applied. Nothing to do.\n";
        m_out.flush();
        return 0;
    }

    QString patchError;
    if (!svc.applyPendingPatches(patches, &patchError)) {
        m_err << "error: " << patchError << "\n";
        m_err.flush();
        return 2;
    }

    m_out << "Applied now     : " << pending << "\n";
    m_out << "\nResult: OK\n";
    m_out.flush();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmdExport
// ─────────────────────────────────────────────────────────────────────────────

int QuizAdminCli::cmdExport(const QString& outFile)
{
    m_out << "\n=== CppAtlas Content Export ===\n\n";

    QFile f(outFile);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        m_err << "error: cannot open output file: " << outFile << "\n";
        m_err.flush();
        return 2;
    }
    QTextStream out(&f);

    // Header
    out << "-- CppAtlas quiz content export\n";
    out << "-- Generated: "
        << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n\n";

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);

    // Tables to export with their ORDER BY clause (deterministic ordering)
    struct TableSpec {
        QString name;
        QString orderBy;
    };
    const QList<TableSpec> tables = {
        { "topics",        "ORDER BY id" },
        { "tags",          "ORDER BY id" },
        { "quizzes",       "ORDER BY id" },
        { "questions",     "ORDER BY id" },
        { "options",       "ORDER BY id" },
        { "question_tags", "ORDER BY question_id, tag_id" },
        { "quiz_tags",     "ORDER BY quiz_id, tag_id" },
    };

    int totalRows = 0;
    for (const TableSpec& spec : tables) {
        out << "-- Table: " << spec.name << "\n";

        QSqlQuery q(db);
        if (!q.exec("SELECT * FROM " + spec.name + " " + spec.orderBy)) {
            m_err << "error: failed to query table '" << spec.name << "': "
                  << q.lastError().text() << "\n";
            m_err.flush();
            return 2;
        }

        const QSqlRecord rec = q.record();
        const int fieldCount = rec.count();
        int tableRows = 0;

        while (q.next()) {
            out << "INSERT INTO " << spec.name << " (";
            for (int i = 0; i < fieldCount; ++i) {
                if (i > 0) out << ", ";
                out << rec.fieldName(i);
            }
            out << ") VALUES (";
            for (int i = 0; i < fieldCount; ++i) {
                if (i > 0) out << ", ";
                const QVariant val = q.value(i);
                if (val.isNull()) {
                    out << "NULL";
                } else {
                    const QMetaType::Type mt = static_cast<QMetaType::Type>(val.typeId());
                    if (mt == QMetaType::Int || mt == QMetaType::LongLong
                            || mt == QMetaType::UInt || mt == QMetaType::ULongLong) {
                        out << val.toLongLong();
                    } else {
                        out << "'" << val.toString().replace("'", "''") << "'";
                    }
                }
            }
            out << ");\n";
            ++tableRows;
        }
        out << "\n";
        totalRows += tableRows;
        m_out << "  " << spec.name << ": " << tableRows << " row(s)\n";
    }

    out.flush();
    f.close();

    m_out << "\nTotal rows exported : " << totalRows << "\n";
    m_out << "Output file         : " << outFile << "\n";
    m_out << "\nResult: OK\n";
    m_out.flush();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// printUsage
// ─────────────────────────────────────────────────────────────────────────────

void QuizAdminCli::printUsage(QTextStream& out) const
{
    out << "Usage: quiz_admin [--db <path>] <command> [options]\n"
           "\n"
           "Global options:\n"
           "  --db <path>   Path to the SQLite database file.\n"
           "                Defaults to the standard CppAtlas application data location.\n"
           "  --help, -h    Show this help message.\n"
           "\n"
           "Commands:\n"
           "  stats\n"
           "    Print database statistics:\n"
           "      schema version, topics, quizzes, questions, tags,\n"
           "      users, sessions, applied content patches.\n"
           "\n"
           "  validate --content-dir <dir>\n"
           "    Discover *.sql patch files in <dir> (sorted lexicographically),\n"
           "    report applied/pending status for each patch, and run basic content\n"
           "    integrity checks (MCQ without options, orphan options, MCQ without\n"
           "    a correct answer).\n"
           "    Exit code: 0 = OK, 1 = usage error, 2 = warnings found.\n"
           "\n"
           "  apply-content --content-dir <dir>\n"
           "    Discover pending *.sql patch files in <dir> and apply them in\n"
           "    lexicographic order.  Stops on first failure.\n"
           "    Exit code: 0 = success, 1 = usage error, 2 = apply failure.\n"
           "\n"
           "  export --out <file>\n"
           "    Export content tables to a SQL dump file.\n"
           "    Tables: topics, tags, quizzes, questions, options,\n"
           "            question_tags, quiz_tags.\n"
           "    Exit code: 0 = success, 1 = usage error, 2 = export failure.\n"
           "\n"
           "Examples:\n"
           "  quiz_admin stats\n"
           "  quiz_admin --db /var/data/cppatlas.db stats\n"
           "  quiz_admin validate --content-dir ./patches\n"
           "  quiz_admin apply-content --content-dir ./patches\n"
           "  quiz_admin --db /var/data/cppatlas.db apply-content --content-dir ./patches\n"
           "  quiz_admin export --out backup.sql\n"
           "  quiz_admin --db /var/data/cppatlas.db export --out backup.sql\n";
    out.flush();
}
