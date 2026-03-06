#ifndef PROGRESSANALYZER_H
#define PROGRESSANALYZER_H

#include <QList>
#include "quiz/QuizRepository.h"

/**
 * @brief Analyzes user_topic_stats and generates adaptive recommendations.
 *
 * Called automatically by QuizEngine after every completed session.
 * Can also be called on demand (e.g., for the user dashboard).
 *
 * Weak area threshold: mastery_level < 0.60 (configurable)
 * Strong area threshold: mastery_level >= 0.85
 *
 * After analysis, unread recommendations are written to the DB.
 * The UI reads them from QuizRepository::recommendationsForUser().
 */
class ProgressAnalyzer
{
public:
    static constexpr double WEAK_THRESHOLD   = 0.60;
    static constexpr double STRONG_THRESHOLD = 0.85;
    static constexpr int    MIN_ATTEMPTS_FOR_ANALYSIS = 2;

    /**
     * @brief Compute weak areas and insert new recommendations into DB.
     * Clears old unread recommendations before inserting new ones.
     */
    void generateRecommendations(int userId) const;

    /**
     * @brief Returns topics where mastery_level < WEAK_THRESHOLD.
     * Requires at least MIN_ATTEMPTS_FOR_ANALYSIS attempts.
     */
    QList<UserTopicStatDTO> weakTopics(int userId) const;

    /**
     * @brief Returns topics where mastery_level >= STRONG_THRESHOLD.
     */
    QList<UserTopicStatDTO> strongTopics(int userId) const;

    /**
     * @brief Returns all topic stats, sorted by mastery_level ascending.
     * Useful for the radar chart (passes normalized 0.0–1.0 values).
     */
    QList<UserTopicStatDTO> allTopicStatsSorted(int userId) const;

    /**
     * @brief Overall mastery score across all attempted topics (0–100).
     */
    int overallScore(int userId) const;

private:
    QuizRepository m_repo;
};

#endif // PROGRESSANALYZER_H