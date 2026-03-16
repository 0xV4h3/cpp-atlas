#pragma once
#include <QString>

/**
 * @brief Stateless service that encapsulates answer-comparison logic.
 *
 * All question types that require text-based comparison should route through
 * this service so that the scoring path and the UI result display stay in sync.
 */
class AnswerEvaluationService
{
public:
    /**
     * @brief Canonical comparison for fill-in-the-blank answers.
     *
     * Normalisation pipeline applied to both sides before comparison:
     *   1. Trim leading/trailing whitespace
     *   2. Collapse internal whitespace sequences to a single space
     *   3. Case-insensitive comparison (Unicode-aware via Qt::CaseInsensitive)
     *
     * @param user     The answer string supplied by the user.
     * @param expected The correct answer token stored in the database option.
     * @return true if the normalised strings are equal (case-insensitive).
     */
    static bool isFillBlankMatch(const QString& user, const QString& expected);

private:
    /** Apply the canonical normalisation pipeline to a single string. */
    static QString normalizeText(const QString& text);
};
