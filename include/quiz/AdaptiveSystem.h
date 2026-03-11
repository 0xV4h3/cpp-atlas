#ifndef ADAPTIVESYSTEM_H
#define ADAPTIVESYSTEM_H

#include <QList>
#include <QString>
#include "quiz/QuizRepository.h"
#include "quiz/ProgressAnalyzer.h"

/**
 * @brief High-level adaptive learning recommendation engine.
 *
 * Wraps ProgressAnalyzer with actionable quiz suggestions.
 * Called after each session to update recommendations.
 *
 * Recommendation strategy:
 *   1. Topics with mastery_level < WEAK_THRESHOLD and >= MIN_ATTEMPTS → "review" recs
 *   2. Topics with 0 attempts → "explore" recs (up to MAX_NEW_TOPICS)
 *   3. Strong topics → "challenge" recs (link to advanced quiz)
 */
class AdaptiveSystem
{
public:
    static constexpr int MAX_RECOMMENDATIONS = 5;
    static constexpr int MAX_NEW_TOPICS       = 2;

    /**
     * @brief Run full adaptive analysis and update DB recommendations.
     * Clears old recs and inserts new ones via QuizRepository.
     * Should be called after every completed session.
     */
    void runAnalysis(int userId);

    /**
     * @brief Get the next recommended quiz for a user.
     * Returns -1 if no suitable quiz found.
     * Priority: weakest topic → easiest quiz for that topic.
     */
    int nextRecommendedQuizId(int userId) const;

    /**
     * @brief Get the recommended topic IDs for exploration (0-attempt topics).
     */
    QList<int> unexploredTopics(int userId) const;

private:
    QuizRepository m_repo;
    ProgressAnalyzer m_analyzer;
};

#endif // ADAPTIVESYSTEM_H
