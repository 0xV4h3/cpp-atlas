#include "quiz/AdaptiveSystem.h"
#include <QDebug>
#include <QSet>

void AdaptiveSystem::runAnalysis(int userId)
{
    // Delegate weak-area recs to ProgressAnalyzer (already handles DB write)
    m_analyzer.generateRecommendations(userId);

    // Additionally: find unexplored topics and insert "explore" recommendations
    const QList<int> newTopics = unexploredTopics(userId);
    int count = 0;
    for (int topicId : newTopics) {
        if (count >= MAX_NEW_TOPICS) break;
        const TopicDTO topic = m_repo.topicById(topicId);
        if (topic.id < 0) continue;
        const QString refUrl = topic.refUrl.isEmpty() ? topic.refUrl2 : topic.refUrl;
        const QString reason = QString(
            "You haven't explored \"%1\" yet — try it to expand your C++ knowledge!"
        ).arg(topic.title);
        m_repo.insertRecommendation(userId, topicId, reason, refUrl);
        ++count;
    }

    qDebug() << "[AdaptiveSystem] Analysis complete for user" << userId;
}

int AdaptiveSystem::nextRecommendedQuizId(int userId) const
{
    const QList<UserTopicStatDTO> weak = m_analyzer.weakTopics(userId);
    for (const auto& stat : weak) {
        if (stat.topicId < 0) continue;
        const QList<QuizDTO> quizzes = m_repo.quizzesByTopic(stat.topicId);
        if (!quizzes.isEmpty()) {
            // Return easiest quiz for this weak topic
            int bestId = quizzes.first().id;
            int bestDiff = quizzes.first().difficulty;
            for (const auto& q : quizzes) {
                if (q.difficulty < bestDiff) {
                    bestId = q.id;
                    bestDiff = q.difficulty;
                }
            }
            return bestId;
        }
    }
    return -1;
}

QList<int> AdaptiveSystem::unexploredTopics(int userId) const
{
    QList<int> result;
    const QList<UserTopicStatDTO> allStats = m_repo.userTopicStats(userId);
    const QList<TopicDTO> allTopics = m_repo.allTopics();

    // Build set of attempted topic IDs
    QSet<int> attempted;
    for (const auto& s : allStats) {
        if (s.attempts > 0) attempted.insert(s.topicId);
    }

    for (const auto& topic : allTopics) {
        if (!attempted.contains(topic.id)) {
            result << topic.id;
        }
    }
    return result;
}
