#ifndef QUIZCONTENTEXPORTER_H
#define QUIZCONTENTEXPORTER_H

#include <QString>

/**
 * @brief SQL-dump export for the CppAtlas quiz content tables.
 *
 * Exports the canonical set of content tables to a plain-SQL file using
 * deterministic ordering so the output is reproducible.
 *
 * Tables exported: topics, tags, quizzes, questions, options,
 *                  question_tags, quiz_tags
 *
 * This is distinct from QuizExporter which handles JSON import/export of
 * individual question sets for custom tests.
 *
 * Usage:
 * @code
 *   int rowsExported = 0;
 *   QString err;
 *   bool ok = QuizContentExporter::exportToFile("/path/backup.sql",
 *                                               &rowsExported, &err);
 * @endcode
 */
class QuizContentExporter
{
public:
    QuizContentExporter() = delete;  // static utility class

    /**
     * @brief Export content tables to a SQL dump file.
     *
     * Writes INSERT statements for topics, tags, quizzes, questions, options,
     * question_tags, and quiz_tags using ORDER BY id (or composite key for
     * junction tables) to ensure deterministic output.
     *
     * @param filePath       Absolute path to write.  Created/overwritten.
     * @param outRowsTotal   If non-null, receives the total number of INSERT
     *                       rows written on success.
     * @param error          If non-null, receives a human-readable error
     *                       message on failure.
     * @return true on success; false on file-open or SQL error.
     */
    static bool exportToFile(const QString& filePath,
                             int*    outRowsTotal = nullptr,
                             QString* error       = nullptr);
};

#endif // QUIZCONTENTEXPORTER_H
