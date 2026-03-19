#pragma once

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "quiz/UserManager.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);
    UserRecord loggedInUser() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onSwitchToRegister();
    void onSwitchToLogin();
    void onUserSelected(QListWidgetItem* item);
    void applyTheme();

private:
    void setupUi();
    void setupLoginPage();
    void setupRegisterPage();
    void populateUserList();
    void showError(const QString& message);
    void clearError();
    void updateRegisterAvatarPreview();

    QStackedWidget* m_stack         = nullptr;

    // Login page
    QWidget*      m_loginPage       = nullptr;
    QListWidget*  m_userList        = nullptr;
    QLineEdit*    m_passwordEdit     = nullptr;
    QPushButton*  m_loginBtn        = nullptr;
    QPushButton*  m_toRegisterBtn   = nullptr;
    QLabel*       m_loginError      = nullptr;

    // Register page
    QWidget*      m_registerPage    = nullptr;
    QLineEdit*    m_usernameEdit    = nullptr;
    QLineEdit*    m_displayNameEdit = nullptr;
    QLineEdit*    m_regPasswordEdit = nullptr;
    QLineEdit*    m_confirmPassEdit = nullptr;
    QPushButton*  m_registerBtn     = nullptr;
    QPushButton*  m_toLoginBtn      = nullptr;
    QLabel*       m_registerError   = nullptr;

    //Avatar selection in register
    QLabel*       m_avatarPreviewLabel = nullptr;
    QString       m_pendingAvatarPath;

    UserRecord    m_loggedInUser;
    bool          m_isFirstUser = false;
};
