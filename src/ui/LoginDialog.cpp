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
#include <QPainter>
#include <QPainterPath>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setWindowTitle("CppAtlas — Sign In");
    setModal(true);
    setMinimumSize(420, 520);
    setMaximumSize(520, 680);

    m_isFirstUser = !UserManager::instance().hasAnyUsers();

    setupUi();
    applyTheme();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &LoginDialog::applyTheme);

    if (m_isFirstUser)
        m_stack->setCurrentIndex(1);
    else
        populateUserList();
}

UserRecord LoginDialog::loggedInUser() const { return m_loggedInUser; }

// ── Avatar helper ─────────────────────────────────────────────────────────────
static QPixmap makeAvatarPixmap(const UserRecord& u, int size = 40)
{
    QPixmap circle(size, size);
    circle.fill(Qt::transparent);
    QPainter painter(&circle);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!u.avatarPath.isEmpty()) {
        QPixmap pix(u.avatarPath);
        if (!pix.isNull()) {
            QPainterPath path;
            path.addEllipse(0, 0, size, size);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, size, size,
                               pix.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            return circle;
        }
    }

    const QColor color(u.avatarColor.isEmpty() ? "#007ACC" : u.avatarColor);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);
    painter.setPen(Qt::white);
    QFont f = painter.font();
    f.setPointSize(qMax(8, size / 3));
    f.setBold(true);
    painter.setFont(f);
    const QString initial = u.displayName.isEmpty()
                                ? u.username.left(1).toUpper()
                                : u.displayName.left(1).toUpper();
    painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, initial);
    return circle;
}

// ── UI Setup ──────────────────────────────────────────────────────────────────
void LoginDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

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
    m_userList->setMaximumHeight(160);
    m_userList->setIconSize(QSize(36, 36));
    connect(m_userList, &QListWidget::itemClicked, this, &LoginDialog::onUserSelected);
    layout->addWidget(m_userList);

    QLabel* pwdLabel = new QLabel("Password:", m_loginPage);
    pwdLabel->setObjectName("fieldLabel");
    layout->addWidget(pwdLabel);

    m_passwordEdit = new QLineEdit(m_loginPage);
    m_passwordEdit->setObjectName("authInput");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter your password");
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
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
    connect(m_toRegisterBtn, &QPushButton::clicked, this, &LoginDialog::onSwitchToRegister);
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
    layout->setSpacing(12);

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
    connect(m_confirmPassEdit, &QLineEdit::returnPressed, this, &LoginDialog::onRegisterClicked);
    form->addRow("Confirm:", m_confirmPassEdit);

    layout->addLayout(form);

    // Avatar selection row
    QHBoxLayout* avatarRow = new QHBoxLayout();
    avatarRow->setSpacing(8);

    m_avatarPreviewLabel = new QLabel(m_registerPage);
    m_avatarPreviewLabel->setFixedSize(48, 48);
    m_avatarPreviewLabel->setAlignment(Qt::AlignCenter);
    m_avatarPreviewLabel->setObjectName("regAvatarPreview");
    updateRegisterAvatarPreview();

    QPushButton* chooseAvatarBtn = new QPushButton("Choose Avatar...", m_registerPage);
    chooseAvatarBtn->setObjectName("linkButton");
    connect(chooseAvatarBtn, &QPushButton::clicked, this, [this]() {
        const QString filePath = QFileDialog::getOpenFileName(
            this, "Select Avatar Image", QString(),
            "Images (*.png *.jpg *.jpeg *.bmp)");
        if (filePath.isEmpty()) return;
        m_pendingAvatarPath = filePath;
        updateRegisterAvatarPreview();
    });

    avatarRow->addWidget(new QLabel("Avatar:", m_registerPage));
    avatarRow->addWidget(m_avatarPreviewLabel);
    avatarRow->addWidget(chooseAvatarBtn);
    avatarRow->addStretch();
    layout->addLayout(avatarRow);

    m_registerError = new QLabel(m_registerPage);
    m_registerError->setObjectName("errorLabel");
    m_registerError->setWordWrap(true);
    m_registerError->setVisible(false);
    layout->addWidget(m_registerError);

    layout->addStretch();

    m_registerBtn = new QPushButton(
        m_isFirstUser ? "Create Admin Account" : "Register", m_registerPage);
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
        connect(m_toLoginBtn, &QPushButton::clicked, this, &LoginDialog::onSwitchToLogin);
        switchRow->addWidget(switchLabel);
        switchRow->addWidget(m_toLoginBtn);
        switchRow->addStretch();
        layout->addLayout(switchRow);
    }
}

void LoginDialog::updateRegisterAvatarPreview()
{
    if (!m_avatarPreviewLabel) return;

    if (!m_pendingAvatarPath.isEmpty()) {
        QPixmap pix(m_pendingAvatarPath);
        if (!pix.isNull()) {
            QPixmap circle(48, 48);
            circle.fill(Qt::transparent);
            QPainter painter(&circle);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, 48, 48);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, 48, 48,
                               pix.scaled(48, 48, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            m_avatarPreviewLabel->setPixmap(circle);
            return;
        }
    }

    // Default placeholder
    QPixmap circle(48, 48);
    circle.fill(Qt::transparent);
    QPainter painter(&circle);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#007ACC"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 48, 48);
    painter.setPen(Qt::white);
    QFont f = painter.font();
    f.setPointSize(16);
    f.setBold(true);
    painter.setFont(f);
    painter.drawText(QRect(0, 0, 48, 48), Qt::AlignCenter, "?");
    m_avatarPreviewLabel->setPixmap(circle);
}

// ── Slots ─────────────────────────────────────────────────────────────────────
void LoginDialog::onLoginClicked()
{
    clearError();
    const QString selectedUsername = m_userList->currentItem()
                                         ? m_userList->currentItem()->data(Qt::UserRole).toString()
                                         : QString();

    if (selectedUsername.isEmpty()) { showError("Please select a user from the list."); return; }
    const QString password = m_passwordEdit->text();
    if (password.isEmpty()) { showError("Please enter your password."); return; }

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

    if (username.isEmpty())                     { showError("Username cannot be empty."); return; }
    if (username.length() < 2 || username.length() > 30) { showError("Username must be 2–30 characters."); return; }
    if (password.length() < 4)                  { showError("Password must be at least 4 characters."); return; }
    if (password != confirm)                    { showError("Passwords do not match."); m_confirmPassEdit->clear(); return; }

    const bool isAdmin = m_isFirstUser;
    if (!UserManager::instance().registerUser(username, displayName, password, isAdmin)) {
        showError("Username already taken. Choose a different username.");
        return;
    }

    UserManager::instance().login(username, password);
    m_loggedInUser = UserManager::instance().currentUser();
    AppSettings::setLastLoggedInUser(username);

    // Copy pending avatar if selected
    if (!m_pendingAvatarPath.isEmpty()) {
        const QString destDir = QStandardPaths::writableLocation(
                                    QStandardPaths::AppDataLocation) + "/avatars/";
        QDir().mkpath(destDir);
        const QString ext = QFileInfo(m_pendingAvatarPath).suffix();
        const QString destPath = destDir + username + "." + ext;
        QFile::remove(destPath);
        if (QFile::copy(m_pendingAvatarPath, destPath))
            UserManager::instance().updateAvatarPath(username, destPath);
    }

    accept();
}

void LoginDialog::onSwitchToRegister() { clearError(); m_stack->setCurrentIndex(1); }

void LoginDialog::onSwitchToLogin()
{
    clearError();
    populateUserList();
    m_stack->setCurrentIndex(0);
}

void LoginDialog::onUserSelected(QListWidgetItem*) { m_passwordEdit->setFocus(); }

void LoginDialog::populateUserList()
{
    m_userList->clear();
    const QList<UserRecord> users = UserManager::instance().allUsers();
    for (const UserRecord& u : users) {
        QListWidgetItem* item = new QListWidgetItem(m_userList);
        item->setIcon(QIcon(makeAvatarPixmap(u, 36)));
        const QString label = QString("%1  (%2)%3")
                                  .arg(u.displayName)
                                  .arg(u.username)
                                  .arg(u.isAdmin ? "  \xf0\x9f\x91\x91" : "");
        item->setText(label);
        item->setData(Qt::UserRole, u.username);
        m_userList->addItem(item);
    }

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
    QLabel* errLabel = (m_stack->currentIndex() == 0) ? m_loginError : m_registerError;
    errLabel->setText("\xe2\x9a\xa0 " + message);
    errLabel->setVisible(true);
}

void LoginDialog::clearError()
{
    if (m_loginError)    m_loginError->setVisible(false);
    if (m_registerError) m_registerError->setVisible(false);
}

void LoginDialog::applyTheme()
{
    const Theme& t = ThemeManager::instance()->currentTheme();

    // Readable colors: white text on accent for selected; lighter accent bg for hover
    const QString selBg   = t.accent.name();
    const QString selFg   = QStringLiteral("#FFFFFF");
    const QString hoverBg = t.accent.lighter(160).name();
    const QString hoverFg = t.editorBackground.darker(120).name(); // dark text on light hover

    // FIX: Use sequential arg numbers with NO gaps.
    // %1=windowBg  %2=toolbarBg  %3=border   %4=accent   %5=textPrimary
    // %6=textSecondary  %7=panelBg  %8=accentLighter  %9=accentDarker
    // %10=selBg  %11=selFg  %12=hoverBg  %13=hoverFg  %14=error  %15=errorDark
    setStyleSheet(QString(R"(
        LoginDialog { background-color: %1; }

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
        #loginTitle { font-size: 22px; font-weight: bold; color: %5; }

        #pageTitle {
            font-size: 18px;
            font-weight: bold;
            color: %5;
            padding-bottom: 4px;
        }
        #fieldLabel, #switchLabel { color: %6; font-size: 13px; }
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
        #authInput:focus { border: 1px solid %4; }

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
            color: %5;
            background-color: %7;
        }
        #userList::item:selected {
            background-color: %10;
            color: %11;
        }
        #userList::item:hover:!selected {
            background-color: %12;
            color: %13;
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
        #primaryButton:hover   { background-color: %8; }
        #primaryButton:pressed { background-color: %9; }

        #linkButton {
            background: transparent;
            color: %4;
            border: none;
            text-decoration: underline;
            font-size: 13px;
        }
        #linkButton:hover { color: %8; }

        #errorLabel {
            color: %14;
            font-size: 12px;
            background-color: %15;
            border-radius: 4px;
            padding: 6px 8px;
        }

        QLabel { color: %5; }
        QFormLayout QLabel { color: %6; }
        #regAvatarPreview { border-radius: 24px; }
    )")
                      .arg(t.windowBackground.name())       // %1
                      .arg(t.toolbarBackground.name())       // %2
                      .arg(t.border.name())                  // %3
                      .arg(t.accent.name())                  // %4
                      .arg(t.textPrimary.name())             // %5
                      .arg(t.textSecondary.name())           // %6
                      .arg(t.panelBackground.name())         // %7
                      .arg(t.accent.lighter(115).name())     // %8
                      .arg(t.accent.darker(115).name())      // %9
                      .arg(selBg)                            // %10
                      .arg(selFg)                            // %11
                      .arg(hoverBg)                          // %12
                      .arg(hoverFg)                          // %13
                      .arg(t.textPrimary.name())             // %14  error text = readable foreground
                      .arg(t.error.darker(170).name())       // %15  error bg = dark red
                  );
}
