#include "quiz/ProgressAnalyzer.h"
#include <algorithm>
#include <QDebug>

void ProgressAnalyzer::generateRecommendations(int userId) const
{
    m_repo.clearRecommendationsForUser(userId);

    const QList<UserTopicStatDTO> stats = weakTopics(userId);
    for (const auto& s : stats) {
        if (s.topicId < 0) continue;

        // Fetch topic ref_url for the recommendation link
        const TopicDTO topic = m_repo.topicById(s.topicId);
        const QString refUrl = topic.refUrl.isEmpty() ? topic.refUrl2 : topic.refUrl;

        const QString reason = QString(
            "Your score in \"%1\" is %2% — review this topic to strengthen your skills."
        ).arg(s.topicTitle)
         .arg(static_cast<int>(s.masteryLevel * 100));

        m_repo.insertRecommendation(userId, s.topicId, reason, refUrl);
        qDebug() << "[ProgressAnalyzer] Recommendation for" << userId
                 << "→" << s.topicTitle
                 << QString("(%1%)").arg(static_cast<int>(s.masteryLevel * 100));
    }
}

QList<UserTopicStatDTO> ProgressAnalyzer::weakTopics(int userId) const
{
    QList<UserTopicStatDTO> result;
    for (const auto& s : m_repo.userTopicStats(userId)) {
        if (s.attempts >= MIN_ATTEMPTS_FOR_ANALYSIS
            && s.masteryLevel < WEAK_THRESHOLD) {
            result << s;
        }
    }
    // Sort weakest first
    std::sort(result.begin(), result.end(),
              [](const UserTopicStatDTO& a, const UserTopicStatDTO& b) {
                  return a.masteryLevel < b.masteryLevel;
              });
    return result;
}

QList<UserTopicStatDTO> ProgressAnalyzer::strongTopics(int userId) const
{
    QList<UserTopicStatDTO> result;
    for (const auto& s : m_repo.userTopicStats(userId)) {
        if (s.attempts >= MIN_ATTEMPTS_FOR_ANALYSIS
            && s.masteryLevel >= STRONG_THRESHOLD) {
            result << s;
        }
    }
    std::sort(result.begin(), result.end(),
              [](const UserTopicStatDTO& a, const UserTopicStatDTO& b) {
                  return a.masteryLevel > b.masteryLevel;
              });
    return result;
}

QList<UserTopicStatDTO> ProgressAnalyzer::allTopicStatsSorted(int userId) const
{
    QList<UserTopicStatDTO> stats = m_repo.userTopicStats(userId);
    std::sort(stats.begin(), stats.end(),
              [](const UserTopicStatDTO& a, const UserTopicStatDTO& b) {
                  return a.masteryLevel < b.masteryLevel;
              });
    return stats;
}

int ProgressAnalyzer::overallScore(int userId) const
{
    const auto stats = m_repo.userTopicStats(userId);
    if (stats.isEmpty()) return 0;

    double total = 0.0;
    int    count = 0;
    for (const auto& s : stats) {
        if (s.attempts >= MIN_ATTEMPTS_FOR_ANALYSIS) {
            total += s.masteryLevel;
            ++count;
        }
    }
    return count > 0 ? static_cast<int>((total / count) * 100) : 0;
}