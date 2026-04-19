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
 * Format: JSON — human-readable header, encrypted answer data.
 *
 * Schema (version 1):
 * {
 *   "cppatlas_export": 1,
 *   "title": "My Custom Test",
 *   "description": "...",
 *   "exported_at": "ISO-8601",
 *   "answers_encrypted": true,        ← NEW in v1; false/absent = legacy plain
 *   "questions": [
 *     {
 *       "id": 7,
 *       "type": "mcq",
 *       "content": "...",             ← plaintext (not secret)
 *       "code_snippet": "...",        ← plaintext
 *       "explanation": "...",         ← plaintext (shown after answering)
 *       "difficulty": 2,
 *       "points": 10,
 *       "hint": "...",
 *       "ref_url": "...",
 *       "tags": ["oop", "classes"],
 *       "options": [
 *         {
 *           "content": "...",         ← plaintext
 *           "code_snippet": "...",
 *           "c": "<encrypted blob>"   ← is_correct, Base64(salt||cipher)
 *         },
 *         ...
 *       ],
 *       "accepted_answers_enc": "<encrypted blob>"  ← fill_blank answers
 *     },
 *     ...
 *   ]
 * }
 *
 * Encryption: ExportCrypto (XOR-stream with SHA-256-derived key + random salt).
 * The question text and explanation remain plaintext because they are shown
 * to students anyway.  Only the answer-bearing fields are encrypted.
 *
 * Backwards compatibility: files without "answers_encrypted":true (or with
 * the old "is_correct" key) are imported as plaintext and re-exported with
 * encryption the next time.
 */
class QuizExporter
{
public:
    QuizExporter() = delete;

    static constexpr int FORMAT_VERSION = 1;
    static constexpr const char* FILE_EXTENSION = "json";
    static constexpr const char* FILE_FILTER =
        "CppAtlas Quiz Test (*.json);;All Files (*)";

    /** Ensure filePath ends with .json. */
    static QString ensureJsonExtension(const QString& filePath);

    /** Read title/description from export header without loading all questions. */
    static bool readHeader(const QString& filePath,
                           QString& outTitle,
                           QString& outDescription);

    /**
     * @brief Export questions to a .json file with encrypted answers.
     * @param questions   Questions to export (with options and tags populated).
     * @param title       Custom test title.
     * @param description Optional description.
     * @param filePath    Absolute path to write — should end in .json.
     * @return true on success.
     */
    static bool exportToFile(const QList<QuestionDTO>& questions,
                             const QString& title,
                             const QString& description,
                             const QString& filePath);

    /**
     * @brief Import questions from a .json export file.
     *
     * Answers are decrypted transparently.  Returned QuestionDTO objects
     * have id = -1 (transient, not from DB).
     *
     * @param filePath  Absolute path to read.
     * @param error     Human-readable error on failure (may be nullptr).
     * @return Populated list on success; empty list on error.
     */
    static QList<QuestionDTO> importFromFile(const QString& filePath,
                                             QString* error = nullptr);

    /** Serialise a single question (answers encrypted). */
    static QJsonObject questionToJson(const QuestionDTO& q);

    /**
     * @brief Deserialise a single question.
     * @param obj               JSON object from the file.
     * @param answersEncrypted  True if the file was written with encryption.
     */
    static QuestionDTO questionFromJson(const QJsonObject& obj,
                                        bool answersEncrypted = false);

private:
    /** Serialise options with encrypted is_correct flags. */
    static QJsonArray  optionsToJson(const QList<OptionDTO>& options);

    /**
     * @brief Deserialise options.
     * @param answersEncrypted  If true, the "c" key is decrypted; otherwise
     *                          the legacy "is_correct" key is used.
     */
    static QList<OptionDTO> optionsFromJson(const QJsonArray& arr,
                                            bool answersEncrypted = false);
};

#endif // QUIZEXPORTER_H
