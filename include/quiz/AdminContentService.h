#pragma once
#include <QObject>
#include <QString>
#include <QVariantMap>

/**
 * @brief Result returned by every AdminContentService write operation.
 *
 * Check @p ok first; on failure @p message carries a human-readable
 * description.  @p affectedRows reports how many DB rows were changed
 * on success.
 */
struct AdminOpResult {
    bool    ok           = false;
    QString message;
    int     affectedRows = 0;
    int     entityId     = -1; ///< Primary key of created/updated entity (-1 when not applicable)
};

/**
 * @brief Transaction-safe service layer for admin content CRUD.
 *
 * All write paths run inside an explicit QSqlDatabase transaction with
 * automatic rollback on any error.  The UI layer must never issue raw
 * QSqlQuery writes against quiz content tables.
 *
 * Required payload keys per operation are documented on each method.
 * Unrecognised keys are silently ignored.
 *
 * Difficulty is always validated to the domain [1..4] before any write.
 */
class AdminContentService : public QObject
{
    Q_OBJECT
public:
    static AdminContentService& instance();

    // ── Questions ────────────────────────────────────────────────────────────

    /**
     * @brief Insert a new question row.
     *
     * Required keys: type, content, quiz_id or topic_id, difficulty.
     * Optional keys: code_snippet, explanation, hint, ref_url,
     *                time_limit, points, order_index.
     */
    AdminOpResult createQuestion(const QVariantMap& payload);

    /**
     * @brief Update an existing question row.
     *
     * @p patch may contain any subset of question column names.
     * difficulty, if present, is validated to [1..4].
     */
    AdminOpResult updateQuestion(int questionId, const QVariantMap& patch);

    /**
     * @brief Soft-delete a question (sets is_active = 0).
     *
     * @param reason  Optional audit reason stored in the deletion log table.
     */
    AdminOpResult softDeleteQuestion(int questionId, const QString& reason = QString());

    /**
     * @brief Restore a previously soft-deleted question (set is_active = 1).
     */
    AdminOpResult restoreQuestion(int questionId);

    // ── Options ──────────────────────────────────────────────────────────────

    /**
     * @brief Insert a new option for @p questionId.
     *
     * Required keys: content, is_correct.
     * Optional keys: code_snippet, order_index.
     */
    AdminOpResult createOption(int questionId, const QVariantMap& payload);

    /**
     * @brief Update an existing option row.
     *
     * @p patch may contain any subset of option column names.
     */
    AdminOpResult updateOption(int optionId, const QVariantMap& patch);

    /**
     * @brief Permanently delete an option row (no soft delete for options).
     */
    AdminOpResult deleteOption(int optionId);

    // ── Quizzes ──────────────────────────────────────────────────────────────

    /**
     * @brief Insert a new quiz row.
     *
     * Required keys: title, difficulty.
     * Optional keys: description, topic_id, time_limit, is_timed, type.
     */
    AdminOpResult createQuiz(const QVariantMap& payload);

    /**
     * @brief Update an existing quiz row.
     *
     * @p patch may contain any subset of quiz column names.
     * difficulty, if present, is validated to [1..4].
     */
    AdminOpResult updateQuiz(int quizId, const QVariantMap& patch);

    /**
     * @brief Soft-delete a quiz (sets is_active = 0).
     */
    AdminOpResult softDeleteQuiz(int quizId, const QString& reason = QString());

    /**
     * @brief Restore a previously soft-deleted quiz (set is_active = 1).
     */
    AdminOpResult restoreQuiz(int quizId);

    // ── Validation ───────────────────────────────────────────────────────────

    /**
     * @brief Light-weight pre-save validation of an entity payload.
     *
     * Returns AdminOpResult{ok=false, message=reason} on any validation
     * failure so the caller can surface errors before a DB write is attempted.
     *
     * Checks:
     *  - required fields present and non-empty
     *  - difficulty in [1..4]
     *  - type is a known question type (for question payloads)
     */
    AdminOpResult validateBeforeSave(const QVariantMap& entityPayload) const;

private:
    explicit AdminContentService(QObject* parent = nullptr);

    /** Validate difficulty value.  Returns empty string on success, error on failure. */
    static QString validateDifficulty(const QVariantMap& m);
};
