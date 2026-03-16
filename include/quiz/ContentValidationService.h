#pragma once
#include <QString>
#include <QList>

/**
 * @brief Severity level of a validation finding.
 */
enum class ValidationSeverity {
    Error,    ///< Hard error — must be fixed before the patch can be applied.
    Warning   ///< Advisory — should be reviewed but does not block apply.
};

/**
 * @brief A single validation finding produced by ContentValidationService.
 */
struct ValidationFinding {
    ValidationSeverity severity = ValidationSeverity::Warning;
    QString            entityType;   ///< "quiz", "question", "option", …
    int                entityId  = -1;
    QString            message;
    QString            suggestedFix;
};

/**
 * @brief Stateless content-integrity validation service.
 *
 * Validates the current database state against a rule set and returns a
 * list of findings.  The caller decides whether to block or just warn.
 *
 * Rules checked:
 *  1. Quiz and question difficulty must be in [1..4].
 *  2. fill_blank questions must have at least one accepted short answer.
 *  3. fill_blank answer tokens must be ≤80 characters (warning if longer).
 *  4. fill_blank answer tokens that look like full sentences are flagged.
 *  5. MCQ questions must have at least one option and one correct option.
 *  6. Options must not be orphaned (parent question must exist).
 */
class ContentValidationService
{
public:
    /**
     * @brief Run all content validation rules against the open database.
     * @return List of findings (may be empty on a clean database).
     */
    QList<ValidationFinding> validate() const;

    /**
     * @brief Returns true if the finding list contains any hard Error entries.
     */
    static bool hasErrors(const QList<ValidationFinding>& findings);
};
