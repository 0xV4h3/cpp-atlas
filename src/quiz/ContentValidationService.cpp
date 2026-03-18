#include "quiz/ContentValidationService.h"
#include "quiz/QuizDatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────

static QSqlDatabase cvDb()
{
    return QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
}

/** Returns true when the token looks sentence-like. */
static bool isSentenceLike(const QString& token)
{
    // Heuristic: contains a period, semicolon, or " or " with 5+ words
    const int wordCount = token.split(' ', Qt::SkipEmptyParts).size();
    if (token.contains('.') || token.contains(';'))
        return wordCount > 3;
    if (token.contains(" or ", Qt::CaseInsensitive) && wordCount > 4)
        return true;
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

bool ContentValidationService::hasErrors(const QList<ValidationFinding>& findings)
{
    for (const auto& f : findings)
        if (f.severity == ValidationSeverity::Error)
            return true;
    return false;
}

QList<ValidationFinding> ContentValidationService::validate() const
{
    QList<ValidationFinding> findings;
    QSqlDatabase db = cvDb();
    if (!db.isOpen()) {
        findings.append({ValidationSeverity::Error, "database", -1,
                         "Database is not open.", "Open the database before validating."});
        return findings;
    }

    // ── Rule 1: Difficulty range for quizzes ──────────────────────────────────
    {
        QSqlQuery q(db);
        q.exec("SELECT id, difficulty FROM quizzes WHERE difficulty < 1 OR difficulty > 4");
        while (q.next()) {
            const int id   = q.value(0).toInt();
            const int diff = q.value(1).toInt();
            findings.append({
                ValidationSeverity::Error, "quiz", id,
                QString("Difficulty %1 is outside [1..4]").arg(diff),
                "Set difficulty to a value between 1 and 4."
            });
        }
    }

    // ── Rule 1b: Difficulty range for questions ───────────────────────────────
    {
        QSqlQuery q(db);
        q.exec("SELECT id, difficulty FROM questions "
               "WHERE is_active = 1 AND (difficulty < 1 OR difficulty > 4)");
        while (q.next()) {
            const int id   = q.value(0).toInt();
            const int diff = q.value(1).toInt();
            findings.append({
                ValidationSeverity::Error, "question", id,
                QString("Difficulty %1 is outside [1..4]").arg(diff),
                "Set difficulty to a value between 1 and 4."
            });
        }
    }

    // ── Rule 2: fill_blank must have ≥1 row in fill_blank_answers ─────────────
    // Schema v4 guarantees the table exists; no fallback to options.
    {
        QSqlQuery q(db);
        q.exec("SELECT id FROM questions WHERE type = 'fill_blank' AND is_active = 1");
        while (q.next()) {
            const int qid = q.value(0).toInt();

            QSqlQuery chk(db);
            chk.prepare("SELECT 1 FROM fill_blank_answers "
                        "WHERE question_id = :qid AND is_active = 1 LIMIT 1");
            chk.bindValue(":qid", qid);
            const bool hasAnswer = chk.exec() && chk.next();

            if (!hasAnswer) {
                findings.append({
                    ValidationSeverity::Error, "question", qid,
                    "fill_blank question has no accepted answer.",
                    "Add at least one entry to fill_blank_answers."
                });
            }
        }
    }

    // ── Rule 3 & 4: fill_blank answer token quality ───────────────────────────
    {
        QSqlQuery q(db);
        q.exec("SELECT question_id, answer FROM fill_blank_answers WHERE is_active = 1");
        while (q.next()) {
            const int     qid    = q.value(0).toInt();
            const QString answer = q.value(1).toString();

            if (answer.length() > 80) {
                findings.append({
                    ValidationSeverity::Warning, "fill_blank_answers", qid,
                    QString("Answer token is %1 chars (>80): \"%2\"")
                        .arg(answer.length()).arg(answer.left(60) + "…"),
                    "Shorten to a canonical token (e.g. a keyword or short phrase)."
                });
            }

            if (isSentenceLike(answer)) {
                findings.append({
                    ValidationSeverity::Warning, "fill_blank_answers", qid,
                    QString("Answer token looks like a full sentence: \"%1\"")
                        .arg(answer.left(60)),
                    "Replace with a short keyword token; move explanation text to the explanation field."
                });
            }
        }
    }

    // ── Rule 5: MCQ questions must have ≥1 option and ≥1 correct option ───────
    {
        QSqlQuery q(db);
        q.exec(
            "SELECT id FROM questions q "
            "WHERE q.type = 'mcq' AND q.is_active = 1 "
            "AND NOT EXISTS (SELECT 1 FROM options o WHERE o.question_id = q.id)"
            );
        while (q.next()) {
            findings.append({
                ValidationSeverity::Error, "question", q.value(0).toInt(),
                "MCQ question has no options.",
                "Add at least one option."
            });
        }

        QSqlQuery q2(db);
        q2.exec(
            "SELECT id FROM questions q "
            "WHERE q.type = 'mcq' AND q.is_active = 1 "
            "AND NOT EXISTS "
            "(SELECT 1 FROM options o WHERE o.question_id = q.id AND o.is_correct = 1)"
            );
        while (q2.next()) {
            findings.append({
                ValidationSeverity::Error, "question", q2.value(0).toInt(),
                "MCQ question has no correct option.",
                "Mark at least one option as correct."
            });
        }
    }

    // ── Rule 6: Orphan options ────────────────────────────────────────────────
    {
        QSqlQuery q(db);
        q.exec(
            "SELECT id FROM options o "
            "WHERE NOT EXISTS (SELECT 1 FROM questions q WHERE q.id = o.question_id)"
            );
        while (q.next()) {
            findings.append({
                ValidationSeverity::Warning, "option", q.value(0).toInt(),
                "Option references a non-existent question.",
                "Delete orphaned option rows."
            });
        }
    }

    return findings;
}
