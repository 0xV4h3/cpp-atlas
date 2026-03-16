#include <QtTest/QtTest>
#include "quiz/AnswerEvaluationService.h"

/**
 * @brief Tests for fill_blank recovery and multi-alias matching.
 *
 * Covers:
 *  - Short token accepted (e.g. "break")
 *  - Aliases accepted via isFillBlankMatchAny
 *  - Long explanation text NOT used as scoring key
 *  - Case-insensitive and whitespace-normalized matching
 */
class FillBlankRecoveryTest : public QObject
{
    Q_OBJECT

private slots:
    // ── Short token accepted ─────────────────────────────────────────────────

    void shortTokenExact()
    {
        const QStringList keys = {"break"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("break", keys));
    }

    void shortTokenCaseInsensitive()
    {
        const QStringList keys = {"break"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("BREAK", keys));
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("Break", keys));
    }

    void shortTokenTrimmed()
    {
        const QStringList keys = {"break"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("  break  ", keys));
    }

    // ── Aliases accepted ─────────────────────────────────────────────────────

    void aliasFirstInList()
    {
        const QStringList keys = {"continue", "break", "return"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("continue", keys));
    }

    void aliasLastInList()
    {
        const QStringList keys = {"continue", "break", "return"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("return", keys));
    }

    void aliasCaseInsensitive()
    {
        const QStringList keys = {"std::vector", "vector"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("STD::VECTOR", keys));
    }

    void aliasNoMatch()
    {
        const QStringList keys = {"break", "continue"};
        QVERIFY(!AnswerEvaluationService::isFillBlankMatchAny("goto", keys));
    }

    // ── Long explanation text NOT used as scoring key ─────────────────────────

    void longSentenceNotAccepted()
    {
        // Only the short token "break" is in the accepted list.
        // A long explanation sentence should NOT match.
        const QStringList keys = {"break"};
        const QString longSentence =
            "The break statement exits the nearest enclosing loop or switch "
            "statement immediately, continuing execution after the loop body.";
        QVERIFY(!AnswerEvaluationService::isFillBlankMatchAny(longSentence, keys));
    }

    void longSentenceExactlyInListMatches()
    {
        // Conversely, if someone (mistakenly) adds the full sentence as a key,
        // it SHOULD match only when the user types exactly that — which is the
        // broken scenario the validation service flags.
        const QStringList keys = {"The break statement exits the nearest loop."};
        const QString userAnswer = "the break statement exits the nearest loop.";
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny(userAnswer, keys));
    }

    // ── Edge cases ────────────────────────────────────────────────────────────

    void emptyKeyList()
    {
        QVERIFY(!AnswerEvaluationService::isFillBlankMatchAny("break", {}));
    }

    void emptyUserAnswerNoMatch()
    {
        const QStringList keys = {"break"};
        QVERIFY(!AnswerEvaluationService::isFillBlankMatchAny("", keys));
    }

    void emptyKeyInList()
    {
        // An empty key in the list should match an empty user answer
        const QStringList keys = {""};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("", keys));
    }

    void whiteSpaceCollapseAlias()
    {
        const QStringList keys = {"std atomic"};
        QVERIFY(AnswerEvaluationService::isFillBlankMatchAny("std  atomic", keys));
    }
};

QTEST_MAIN(FillBlankRecoveryTest)
#include "test_fill_blank_recovery.moc"
