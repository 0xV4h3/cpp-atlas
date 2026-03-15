#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include "quiz/UserManager.h"
#include "ui/AtlasDialog.h"

/**
 * @brief Login and registration dialog for CppAtlas multi-user system.
 *
 * Shows two pages:
 *  - Page 0 (Select/Login): list of existing users + password field
 *  - Page 1 (Register):     new username + display name + password fields
 *
 * Fully themed via ThemeManager. Connects to UserManager.
 *
 * On first launch (no users exist), navigates directly to Register page
 * and creates the first user as admin.
 */
class LoginDialog : public AtlasDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() = default;

    /**
     * @brief Returns the user record for the successfully logged-in user.
     * Only valid when exec() returns QDialog::Accepted.
     */
    UserRecord loggedInUser() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onSwitchToRegister();
    void onSwitchToLogin();
    void onUserSelected(QListWidgetItem* item);

private:
    void setupUi();
    void setupLoginPage();
    void setupRegisterPage();
    void applyTheme();
    void populateUserList();
    void showError(const QString& message);
    void clearError();

    // ── Stacked pages ─────────────────────────────────────────────────────
    QStackedWidget* m_stack        = nullptr;

    // Login page
    QWidget*        m_loginPage    = nullptr;
    QListWidget*    m_userList     = nullptr;
    QLineEdit*      m_passwordEdit = nullptr;
    QPushButton*    m_loginBtn     = nullptr;
    QPushButton*    m_toRegisterBtn= nullptr;
    QLabel*         m_loginError   = nullptr;

    // Register page
    QWidget*        m_registerPage     = nullptr;
    QLineEdit*      m_usernameEdit     = nullptr;
    QLineEdit*      m_displayNameEdit  = nullptr;
    QLineEdit*      m_regPasswordEdit  = nullptr;
    QLineEdit*      m_confirmPassEdit  = nullptr;
    QPushButton*    m_registerBtn      = nullptr;
    QPushButton*    m_toLoginBtn       = nullptr;
    QLabel*         m_registerError    = nullptr;

    UserRecord      m_loggedInUser;
    bool            m_isFirstUser = false;  // first user becomes admin
};

#endif // LOGINDIALOG_H