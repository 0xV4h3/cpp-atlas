#pragma once
#include <QString>
#include <QStringList>

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

    /**
     * @brief Match a user answer against a list of accepted answers.
     *
     * Returns true if the user's normalised answer equals any of the
     * @p expectedAnswers tokens (case-insensitive, whitespace-collapsed).
     *
     * This is the preferred entry-point for fill_blank scoring when multiple
     * accepted answer tokens are stored in the @c fill_blank_answers table.
     * Falls back gracefully to the single-token path for the common case.
     *
     * @param user            Answer string supplied by the user.
     * @param expectedAnswers Non-empty list of accepted answer tokens.
     * @return true if any token matches.
     */
    static bool isFillBlankMatchAny(const QString& user,
                                    const QStringList& expectedAnswers);

private:
    /** Apply the canonical normalisation pipeline to a single string. */
    static QString normalizeText(const QString& text);
};
