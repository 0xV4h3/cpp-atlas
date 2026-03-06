#ifndef QUIZMODEWINDOW_H
#define QUIZMODEWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "quiz/UserManager.h"

class QuizSelectionWidget;

/**
 * @brief Main container widget for Quiz Mode.
 *
 * Displayed by MainWindow as the central widget (via QStackedWidget),
 * replacing the IDE editor view when the user enters Quiz Mode.
 *
 * Internal page stack:
 *   0 — QuizSelectionWidget  (browse quizzes, launch)
 *   1 — QuizSessionWidget    (active quiz — added in PR #4)
 *   2 — QuizResultsWidget    (results + review — added in PR #4)
 *
 * The back button always returns to QuizSelectionWidget (page 0).
 * The "Exit Quiz Mode" button emits exitRequested() → MainWindow shows WelcomeScreen.
 */
class QuizModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit QuizModeWindow(QWidget* parent = nullptr);
    ~QuizModeWindow() = default;

    /** Called by MainWindow after login, to refresh user display. */
    void setCurrentUser(const UserRecord& user);

    /** Navigate back to the selection screen (e.g. after a session ends). */
    void showSelectionScreen();

signals:
    /** Emitted when the user wants to leave Quiz Mode → back to WelcomeScreen. */
    void exitRequested();

    /** Emitted when a quiz is selected and should launch. */
    void quizLaunchRequested(int quizId, const QString& mode);

private slots:
    void applyTheme();
    void onExitClicked();

private:
    void setupUi();
    void setupHeader();

    QStackedWidget*      m_stack          = nullptr;
    QuizSelectionWidget* m_selectionWidget= nullptr;

    // Header bar
    QWidget*    m_header      = nullptr;
    QLabel*     m_titleLabel  = nullptr;
    QLabel*     m_userLabel   = nullptr;
    QPushButton* m_exitBtn    = nullptr;
    QPushButton* m_backBtn    = nullptr;
};

#endif // QUIZMODEWINDOW_H