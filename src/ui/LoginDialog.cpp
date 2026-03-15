#include "ui/LoginDialog.h"
#include "core/AppSettings.h"
#include "quiz/UserManager.h"
#include "ui/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QFrame>
#include <QApplication>

LoginDialog::LoginDialog(QWidget* parent)
    : AtlasDialog(parent, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setWindowTitle("CppAtlas — Sign In");
    setModal(true);
    setMinimumSize(420, 520);
    setMaximumSize(520, 640);

    m_isFirstUser = !UserManager::instance().hasAnyUsers();

    setupUi();
    applyTheme();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &LoginDialog::applyTheme);

    // If no users exist, go directly to register page (first-time setup)
    if (m_isFirstUser) {
        m_stack->setCurrentIndex(1);
    } else {
        populateUserList();
    }
}

UserRecord LoginDialog::loggedInUser() const
{
    return m_loggedInUser;
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────────────────

void LoginDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header
    QWidget* header = new QWidget(this);
    header->setObjectName("loginHeader");
    header->setFixedHeight(80);
    QVBoxLayout* headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(30, 15, 30, 15);

    QLabel* logoLabel = new QLabel("C++", header);
    logoLabel->setObjectName("loginLogo");

    QLabel* titleLabel = new QLabel("CppAtlas", header);
    titleLabel->setObjectName("loginTitle");

    QHBoxLayout* headRow = new QHBoxLayout();
    headRow->addWidget(logoLabel);
    headRow->addSpacing(10);
    headRow->addWidget(titleLabel);
    headRow->addStretch();
    headerLayout->addLayout(headRow);

    mainLayout->addWidget(header);

    // Stacked pages
    m_stack = new QStackedWidget(this);
    setupLoginPage();
    setupRegisterPage();
    m_stack->addWidget(m_loginPage);
    m_stack->addWidget(m_registerPage);

    mainLayout->addWidget(m_stack, 1);
}

void LoginDialog::setupLoginPage()
{
    m_loginPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_loginPage);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(16);

    QLabel* title = new QLabel("Sign In", m_loginPage);
    title->setObjectName("pageTitle");
    layout->addWidget(title);

    QLabel* userListLabel = new QLabel("Select user:", m_loginPage);
    userListLabel->setObjectName("fieldLabel");
    layout->addWidget(userListLabel);

    m_userList = new QListWidget(m_loginPage);
    m_userList->setObjectName("userList");
    m_userList->setMaximumHeight(150);
    connect(m_userList, &QListWidget::itemClicked,
            this, &LoginDialog::onUserSelected);
    layout->addWidget(m_userList);

    QLabel* pwdLabel = new QLabel("Password:", m_loginPage);
    pwdLabel->setObjectName("fieldLabel");
    layout->addWidget(pwdLabel);

    m_passwordEdit = new QLineEdit(m_loginPage);
    m_passwordEdit->setObjectName("authInput");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter your password");
    connect(m_passwordEdit, &QLineEdit::returnPressed,
            this, &LoginDialog::onLoginClicked);
    layout->addWidget(m_passwordEdit);

    m_loginError = new QLabel(m_loginPage);
    m_loginError->setObjectName("errorLabel");
    m_loginError->setWordWrap(true);
    m_loginError->setVisible(false);
    layout->addWidget(m_loginError);

    layout->addStretch();

    m_loginBtn = new QPushButton("Sign In", m_loginPage);
    m_loginBtn->setObjectName("primaryButton");
    m_loginBtn->setMinimumHeight(40);
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    layout->addWidget(m_loginBtn);

    QHBoxLayout* switchRow = new QHBoxLayout();
    QLabel* switchLabel = new QLabel("New to CppAtlas?", m_loginPage);
    switchLabel->setObjectName("switchLabel");
    m_toRegisterBtn = new QPushButton("Create Account", m_loginPage);
    m_toRegisterBtn->setObjectName("linkButton");
    connect(m_toRegisterBtn, &QPushButton::clicked,
            this, &LoginDialog::onSwitchToRegister);
    switchRow->addWidget(switchLabel);
    switchRow->addWidget(m_toRegisterBtn);
    switchRow->addStretch();
    layout->addLayout(switchRow);
}

void LoginDialog::setupRegisterPage()
{
    m_registerPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_registerPage);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(14);

    QLabel* title = new QLabel(m_isFirstUser ? "Create Admin Account" : "Create Account",
                               m_registerPage);
    title->setObjectName("pageTitle");
    layout->addWidget(title);

    if (m_isFirstUser) {
        QLabel* hint = new QLabel(
            "Welcome! Set up the first admin account.\nYou can add more users later.",
            m_registerPage);
        hint->setObjectName("hintLabel");
        hint->setWordWrap(true);
        layout->addWidget(hint);
    }

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    m_usernameEdit = new QLineEdit(m_registerPage);
    m_usernameEdit->setObjectName("authInput");
    m_usernameEdit->setPlaceholderText("e.g. alice");
    form->addRow("Username:", m_usernameEdit);

    m_displayNameEdit = new QLineEdit(m_registerPage);
    m_displayNameEdit->setObjectName("authInput");
    m_displayNameEdit->setPlaceholderText("e.g. Alice Smith (optional)");
    form->addRow("Display Name:", m_displayNameEdit);

    m_regPasswordEdit = new QLineEdit(m_registerPage);
    m_regPasswordEdit->setObjectName("authInput");
    m_regPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regPasswordEdit->setPlaceholderText("Minimum 4 characters");
    form->addRow("Password:", m_regPasswordEdit);

    m_confirmPassEdit = new QLineEdit(m_registerPage);
    m_confirmPassEdit->setObjectName("authInput");
    m_confirmPassEdit->setEchoMode(QLineEdit::Password);
    m_confirmPassEdit->setPlaceholderText("Repeat password");
    connect(m_confirmPassEdit, &QLineEdit::returnPressed,
            this, &LoginDialog::onRegisterClicked);
    form->addRow("Confirm:", m_confirmPassEdit);

    layout->addLayout(form);

    m_registerError = new QLabel(m_registerPage);
    m_registerError->setObjectName("errorLabel");
    m_registerError->setWordWrap(true);
    m_registerError->setVisible(false);
    layout->addWidget(m_registerError);

    layout->addStretch();

    m_registerBtn = new QPushButton(m_isFirstUser ? "Create Admin Account" : "Register",
                                    m_registerPage);
    m_registerBtn->setObjectName("primaryButton");
    m_registerBtn->setMinimumHeight(40);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    layout->addWidget(m_registerBtn);

    if (!m_isFirstUser) {
        QHBoxLayout* switchRow = new QHBoxLayout();
        QLabel* switchLabel = new QLabel("Already have an account?", m_registerPage);
        switchLabel->setObjectName("switchLabel");
        m_toLoginBtn = new QPushButton("Sign In", m_registerPage);
        m_toLoginBtn->setObjectName("linkButton");
        connect(m_toLoginBtn, &QPushButton::clicked,
                this, &LoginDialog::onSwitchToLogin);
        switchRow->addWidget(switchLabel);
        switchRow->addWidget(m_toLoginBtn);
        switchRow->addStretch();
        layout->addLayout(switchRow);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void LoginDialog::onLoginClicked()
{
    clearError();
    const QString selectedUsername = m_userList->currentItem()
        ? m_userList->currentItem()->data(Qt::UserRole).toString()
        : QString();

    if (selectedUsername.isEmpty()) {
        showError("Please select a user from the list.");
        return;
    }

    const QString password = m_passwordEdit->text();
    if (password.isEmpty()) {
        showError("Please enter your password.");
        return;
    }

    if (UserManager::instance().login(selectedUsername, password)) {
        m_loggedInUser = UserManager::instance().currentUser();
        AppSettings::setLastLoggedInUser(selectedUsername);
        accept();
    } else {
        showError("Incorrect password. Please try again.");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}

void LoginDialog::onRegisterClicked()
{
    clearError();
    const QString username    = m_usernameEdit->text().trimmed();
    const QString displayName = m_displayNameEdit->text().trimmed();
    const QString password    = m_regPasswordEdit->text();
    const QString confirm     = m_confirmPassEdit->text();

    if (username.isEmpty()) {
        showError("Username cannot be empty.");
        return;
    }
    if (username.length() < 2 || username.length() > 30) {
        showError("Username must be 2–30 characters.");
        return;
    }
    if (password.length() < 4) {
        showError("Password must be at least 4 characters.");
        return;
    }
    if (password != confirm) {
        showError("Passwords do not match.");
        m_confirmPassEdit->clear();
        return;
    }

    const bool isAdmin = m_isFirstUser;  // first user is always admin
    if (!UserManager::instance().registerUser(username, displayName, password, isAdmin)) {
        showError("Username already taken. Choose a different username.");
        return;
    }

    // Auto-login after registration
    UserManager::instance().login(username, password);
    m_loggedInUser = UserManager::instance().currentUser();
    AppSettings::setLastLoggedInUser(username);
    accept();
}

void LoginDialog::onSwitchToRegister()
{
    clearError();
    m_stack->setCurrentIndex(1);
}

void LoginDialog::onSwitchToLogin()
{
    clearError();
    populateUserList();
    m_stack->setCurrentIndex(0);
}

void LoginDialog::onUserSelected(QListWidgetItem* item)
{
    Q_UNUSED(item);
    m_passwordEdit->setFocus();
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

void LoginDialog::populateUserList()
{
    m_userList->clear();
    const QList<UserRecord> users = UserManager::instance().allUsers();
    for (const UserRecord& u : users) {
        QListWidgetItem* item = new QListWidgetItem(m_userList);

        // Show avatar circle color + display name + username
        const QString label = QString("%1  (%2)%3")
            .arg(u.displayName)
            .arg(u.username)
            .arg(u.isAdmin ? "  👑" : "");
        item->setText(label);
        item->setData(Qt::UserRole, u.username);
        item->setData(Qt::DecorationRole, QColor(u.avatarColor));
        m_userList->addItem(item);
    }

    // Auto-select last logged-in user
    const QString lastUser = AppSettings::lastLoggedInUser();
    if (!lastUser.isEmpty()) {
        for (int i = 0; i < m_userList->count(); ++i) {
            if (m_userList->item(i)->data(Qt::UserRole).toString() == lastUser) {
                m_userList->setCurrentRow(i);
                m_passwordEdit->setFocus();
                break;
            }
        }
    }
}

void LoginDialog::showError(const QString& message)
{
    QLabel* errLabel = (m_stack->currentIndex() == 0)
                       ? m_loginError : m_registerError;
    errLabel->setText("⚠ " + message);
    errLabel->setVisible(true);
}

void LoginDialog::clearError()
{
    if (m_loginError)    m_loginError->setVisible(false);
    if (m_registerError) m_registerError->setVisible(false);
}

void LoginDialog::applyTheme()
{
    Theme t = ThemeManager::instance()->currentTheme();

    setStyleSheet(QString(R"(
        LoginDialog {
            background-color: %1;
        }
        #loginHeader {
            background-color: %2;
            border-bottom: 1px solid %3;
        }
        #loginLogo {
            font-size: 28px;
            font-weight: bold;
            color: %4;
            font-family: "Consolas", monospace;
        }
        #loginTitle {
            font-size: 22px;
            font-weight: bold;
            color: %5;
        }
        #pageTitle {
            font-size: 18px;
            font-weight: bold;
            color: %5;
            padding-bottom: 4px;
        }
        #fieldLabel, #switchLabel {
            color: %6;
            font-size: 13px;
        }
        #hintLabel {
            color: %6;
            font-size: 12px;
            background-color: %7;
            border-radius: 4px;
            padding: 8px;
        }
        #authInput {
            background-color: %7;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 8px 10px;
            font-size: 13px;
        }
        #authInput:focus {
            border: 1px solid %4;
        }
        #userList {
            background-color: %7;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            font-size: 13px;
        }
        #userList::item {
            padding: 8px 10px;
            border-bottom: 1px solid %3;
        }
        #userList::item:selected {
            background-color: %4;
            color: white;
        }
        #userList::item:hover {
            background-color: %8;
        }
        #primaryButton {
            background-color: %4;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
        }
        #primaryButton:hover {
            background-color: %9;
        }
        #primaryButton:pressed {
            background-color: %10;
        }
        #linkButton {
            background: transparent;
            color: %4;
            border: none;
            text-decoration: underline;
            font-size: 13px;
        }
        #linkButton:hover {
            color: %9;
        }
        #errorLabel {
            color: %11;
            font-size: 12px;
            background-color: %12;
            border-radius: 4px;
            padding: 6px 8px;
        }
        QLabel {
            color: %5;
        }
        QFormLayout QLabel {
            color: %6;
        }
    )")
    .arg(t.windowBackground.name())         // %1
    .arg(t.toolbarBackground.name())         // %2
    .arg(t.border.name())                    // %3
    .arg(t.accent.name())                    // %4
    .arg(t.textPrimary.name())               // %5
    .arg(t.textSecondary.name())             // %6
    .arg(t.panelBackground.name())           // %7
    .arg(t.sidebarBackground.name())         // %8
    .arg(t.accent.lighter(115).name())       // %9
    .arg(t.accent.darker(115).name())        // %10
    .arg(t.error.name())                     // %11
    .arg(t.error.darker(200).name())         // %12
    );
}