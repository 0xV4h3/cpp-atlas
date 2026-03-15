#pragma once
#include <QStringList>
#include <QTextStream>

/**
 * @brief Argument handler and command dispatcher for the quiz_admin CLI tool.
 *
 * Usage:
 * @code
 *   quiz_admin [--db <path>] <command> [options]
 * @endcode
 *
 * Commands:
 *   stats
 *     Print database statistics (topics, quizzes, questions, users, sessions,
 *     applied content patches, current schema version).
 *
 *   validate --content-dir <dir>
 *     Discover *.sql patch files in <dir>, report applied/pending status,
 *     and run basic content integrity checks (MCQ without options, orphan
 *     options, MCQ without a correct answer).
 *
 * Global options:
 *   --db <path>    Path to the SQLite database file.
 *                  Defaults to the standard CppAtlas application data location.
 *   --help, -h     Print usage and exit.
 */
class QuizAdminCli
{
public:
    QuizAdminCli();

    /**
     * @brief Parse @p args and dispatch to the appropriate command.
     * @param args  Argument list including argv[0] (program name).
     * @return 0 on success, 1 on usage/argument error,
     *         2 on integrity warnings (validate command only).
     */
    int run(const QStringList& args);

private:
    // ── Database ─────────────────────────────────────────────────────────────
    bool openDatabase(const QString& dbPath);

    // ── Commands ─────────────────────────────────────────────────────────────
    int cmdStats();
    int cmdValidate(const QString& contentDir);

    // ── Helpers ──────────────────────────────────────────────────────────────
    void printUsage(QTextStream& out) const;

    QTextStream m_out;
    QTextStream m_err;
};
