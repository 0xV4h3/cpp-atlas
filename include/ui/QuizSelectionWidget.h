#ifndef QUIZSELECTIONWIDGET_H
#define QUIZSELECTIONWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCompleter>
#include "quiz/QuizRepository.h"

/**
 * @brief Browse and select a quiz to start.
 *
 * Layout (horizontal split):
 *   LEFT  — Topic tree (QTreeWidget): chapters → leaf topics
 *   RIGHT — Quiz cards (QListWidget): quizzes for selected topic
 *            + filter bar: difficulty combo, tag search
 *            + quiz detail panel at bottom: title, description, stats, Start button
 *
 * Selecting a topic filters the quiz list.
 * Selecting "All" at tree root shows all active quizzes.
 * Clicking Start emits quizSelected(quizId).
 */
class QuizSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuizSelectionWidget(QWidget* parent = nullptr);
    ~QuizSelectionWidget() = default;

    /** Reload from DB (call after new content is seeded). */
    void refresh();

signals:
    void quizSelected(int quizId);

private slots:
    void onTopicSelected();
    void onQuizSelected();
    void onStartClicked();
    void onDifficultyFilterChanged(int index);
    void onSearchChanged();      // unified: called by title or tag textChanged
    void onTagSearchChanged(const QString& text);  // kept for tag-only use
    void applyTheme();

private:
    void setupUi();
    void populateTopicTree();
    void populateQuizList(int topicId = -1);   // -1 = all
    void showQuizDetail(const QuizDTO& quiz);
    void clearQuizDetail();
    void applyCompleterTheme();   ///< theme m_tagCompleter popup
    QString difficultyLabel(int d) const;
    QString difficultyStars(int d) const;

    QuizRepository   m_repo;

    // Left: topic tree
    QTreeWidget*     m_topicTree     = nullptr;

    // Right: quiz list + filters
    QComboBox*       m_difficultyCombo = nullptr;
    QLineEdit*       m_titleSearch     = nullptr;  // filter by quiz title
    QLineEdit*       m_tagSearch       = nullptr;
    QCompleter*      m_tagCompleter    = nullptr;
    QListWidget*     m_quizList        = nullptr;

    // Detail panel (bottom-right)
    QWidget*         m_detailPanel     = nullptr;
    QLabel*          m_detailTitle     = nullptr;
    QLabel*          m_detailDesc      = nullptr;
    QLabel*          m_detailMeta      = nullptr;  // difficulty, questions, tags
    QPushButton*     m_startBtn        = nullptr;

    int              m_selectedQuizId  = -1;
    QList<QuizDTO>   m_currentQuizzes; // filtered list currently shown
};

#endif // QUIZSELECTIONWIDGET_H