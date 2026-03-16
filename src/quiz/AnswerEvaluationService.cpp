#include "quiz/AnswerEvaluationService.h"

// ─────────────────────────────────────────────────────────────────────────────
// Private helper
// ─────────────────────────────────────────────────────────────────────────────

QString AnswerEvaluationService::normalizeText(const QString& text)
{
    // QString::simplified() trims outer whitespace AND collapses internal
    // whitespace sequences (spaces, tabs, newlines) to a single space.
    return text.simplified();
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

bool AnswerEvaluationService::isFillBlankMatch(const QString& user,
                                               const QString& expected)
{
    const QString normUser     = normalizeText(user);
    const QString normExpected = normalizeText(expected);
    return normUser.compare(normExpected, Qt::CaseInsensitive) == 0;
}
