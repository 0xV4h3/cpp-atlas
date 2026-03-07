#ifndef QUIZEXPORTER_H
#define QUIZEXPORTER_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>

#include "quiz/QuizRepository.h"

/**
 * @brief Import / Export utility for custom tests in CppAtlas.
 *
 * Format: JSON — human-readable, version-tagged, portable between users.
 *
 * Schema (version 1):
 * {
 *   "cppatlas_export": 1,
 *   "title": "My Custom Test",
 *   "description": "...",
 *   "exported_at": "ISO-8601",
 *   "questions": [
 *     {
 *       "id": 7,
 *       "type": "mcq",
 *       "content": "...",
 *       "code_snippet": "...",
 *       "explanation": "...",
 *       "difficulty": 2,
 *       "points": 10,
 *       "hint": "...",
 *       "ref_url": "...",
 *       "tags": ["oop", "classes"],
 *       "options": [
 *         { "content": "...", "is_correct": true },
 *         ...
 *       ]
 *     },
 *     ...
 *   ]
 * }
 *
 * Usage:
 *   // Export
 *   bool ok = QuizExporter::exportToFile(questions, title, desc, "/path/to/file.json");
 *
 *   // Import
 *   QString err;
 *   auto questions = QuizExporter::importFromFile("/path/to/file.json", &err);
 *   if (!err.isEmpty()) { ... }
 */
class QuizExporter
{
public:
    QuizExporter() = delete;  // static utility class

    static constexpr int FORMAT_VERSION = 1;
    static constexpr const char* FILE_EXTENSION = "cppatlas";
    static constexpr const char* FILE_FILTER =
        "CppAtlas Test (*.cppatlas);;JSON Files (*.json);;All Files (*)";

    /**
     * @brief Export a list of questions to a .cppatlas JSON file.
     * @param questions   Questions to export (with options and tags populated).
     * @param title       Custom test title (stored in the JSON header).
     * @param description Optional description.
     * @param filePath    Absolute path to write — should end in .cppatlas.
     * @return true on success; false if file could not be written.
     */
    static bool exportToFile(const QList<QuestionDTO>& questions,
                              const QString& title,
                              const QString& description,
                              const QString& filePath);

    /**
     * @brief Import questions from a .cppatlas JSON file.
     *
     * The returned QuestionDTO objects have their options and tags populated
     * from the JSON. The id field is set to -1 (not from DB) so the caller
     * can treat them as transient questions for a custom session.
     *
     * @param filePath  Absolute path to read.
     * @param error     If non-null, receives a human-readable error on failure.
     * @return Populated list on success; empty list on error.
     */
    static QList<QuestionDTO> importFromFile(const QString& filePath,
                                              QString* error = nullptr);

    /**
     * @brief Serialize a single question to a QJsonObject.
     * Used internally and in unit tests.
     */
    static QJsonObject questionToJson(const QuestionDTO& q);

    /**
     * @brief Deserialize a single question from a QJsonObject.
     * Used internally and in unit tests.
     */
    static QuestionDTO questionFromJson(const QJsonObject& obj);

private:
    static QJsonArray  optionsToJson(const QList<OptionDTO>& options);
    static QList<OptionDTO> optionsFromJson(const QJsonArray& arr);
};

#endif // QUIZEXPORTER_H