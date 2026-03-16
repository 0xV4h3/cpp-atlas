#include <QtTest/QtTest>
#include "quiz/AnswerEvaluationService.h"

class FillBlankEvaluationTest : public QObject
{
    Q_OBJECT

private slots:
    // ── Basic match cases ────────────────────────────────────────────────────

    void exactMatch()
    {
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("atomic", "atomic"));
    }

    void trailingSpacesUserAnswer()
    {
        QVERIFY(AnswerEvaluationService::isFillBlankMatch(" Atomic ", "atomic"));
    }

    void leadingSpacesExpected()
    {
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("atomic", " atomic "));
    }

    void caseInsensitiveUpper()
    {
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("ATOMIC", "atomic"));
    }

    void caseInsensitiveMixed()
    {
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("AtOmIc", "atomic"));
    }

    // ── Internal whitespace collapse ─────────────────────────────────────────

    void internalWhitespaceCollapse()
    {
        // Multiple internal spaces should be treated as one
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("std atomic", "std atomic"));
    }

    void internalWhitespaceMismatch()
    {
        // Extra spaces that reduce to same token should still match
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("std  atomic", "std atomic"));
    }

    // ── Non-matching cases ───────────────────────────────────────────────────

    void differentTokenNoMatch()
    {
        QVERIFY(!AnswerEvaluationService::isFillBlankMatch("std::atomic", "atomic"));
    }

    void partialWordNoMatch()
    {
        QVERIFY(!AnswerEvaluationService::isFillBlankMatch("atom", "atomic"));
    }

    void emptyUserAnswerNoMatch()
    {
        QVERIFY(!AnswerEvaluationService::isFillBlankMatch("", "atomic"));
    }

    void emptyExpectedNoMatch()
    {
        QVERIFY(!AnswerEvaluationService::isFillBlankMatch("atomic", ""));
    }

    void bothEmptyMatch()
    {
        // Two empty strings normalise to the same token — treated as equal.
        QVERIFY(AnswerEvaluationService::isFillBlankMatch("", ""));
    }
};

QTEST_MAIN(FillBlankEvaluationTest)
#include "test_fill_blank_evaluation.moc"
