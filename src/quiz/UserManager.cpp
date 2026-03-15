#include "quiz/UserManager.h"
#include "quiz/QuizDatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>

UserManager& UserManager::instance()
{
    static UserManager s_instance;
    return s_instance;
}

UserManager::UserManager(QObject* parent)
    : QObject(parent)
{
}

// ─────────────────────────────────────────────────────────────────────────────
// Authentication
// ─────────────────────────────────────────────────────────────────────────────

bool UserManager::registerUser(const QString& username,
                                const QString& displayName,
                                const QString& password,
                                bool isAdmin)
{
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        qWarning() << "[UserManager] registerUser: empty username or password";
        return false;
    }

    // Check if username already exists (case-insensitive, handled by COLLATE NOCASE)
    if (userByUsername(username).id != -1) {
        qWarning() << "[UserManager] Username already taken:" << username;
        return false;
    }

    const QString salt = generateSalt();
    const QString hash = hashPassword(salt, password);
    const QString now  = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO users (username, display_name, password_hash, salt, "
        "                   avatar_color, created_at, is_admin) "
        "VALUES (:username, :display_name, :hash, :salt, :color, :created, :admin)"
    );
    q.bindValue(":username",     username.trimmed());
    q.bindValue(":display_name", displayName.trimmed().isEmpty() ? username : displayName.trimmed());
    q.bindValue(":hash",         hash);
    q.bindValue(":salt",         salt);
    q.bindValue(":color",        "#007ACC");
    q.bindValue(":created",      now);
    q.bindValue(":admin",        isAdmin ? 1 : 0);

    if (!q.exec()) {
        qWarning() << "[UserManager] registerUser failed:" << q.lastError().text();
        return false;
    }

    qDebug() << "[UserManager] Registered user:" << username
             << "(admin=" << isAdmin << ")";
    return true;
}

bool UserManager::login(const QString& username, const QString& password)
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare(
        "SELECT id, username, display_name, password_hash, salt, "
        "       avatar_color, avatar_path, is_admin, created_at "
        "FROM users WHERE username = :username COLLATE NOCASE"
    );
    q.bindValue(":username", username.trimmed());

    if (!q.exec() || !q.next()) {
        qWarning() << "[UserManager] User not found:" << username;
        return false;
    }

    const QString storedHash = q.value("password_hash").toString();
    const QString salt       = q.value("salt").toString();
    const QString inputHash  = hashPassword(salt, password);

    if (storedHash != inputHash) {
        qWarning() << "[UserManager] Incorrect password for:" << username;
        return false;
    }

    // Populate current user
    m_currentUser.id          = q.value("id").toInt();
    m_currentUser.username    = q.value("username").toString();
    m_currentUser.displayName = q.value("display_name").toString();
    m_currentUser.avatarColor = q.value("avatar_color").toString();
    m_currentUser.avatarPath  = q.value("avatar_path").toString();
    m_currentUser.isAdmin     = q.value("is_admin").toInt() == 1;
    m_currentUser.createdAt   = q.value("created_at").toString();
    m_loggedIn = true;

    // Update last_login timestamp
    QSqlQuery upd(db);
    upd.prepare("UPDATE users SET last_login = :now WHERE id = :id");
    upd.bindValue(":now", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    upd.bindValue(":id",  m_currentUser.id);
    upd.exec();

    qDebug() << "[UserManager] Logged in:" << m_currentUser.username;
    emit userLoggedIn(m_currentUser);
    return true;
}

void UserManager::logout()
{
    if (m_loggedIn) {
        qDebug() << "[UserManager] Logged out:" << m_currentUser.username;
        m_loggedIn    = false;
        m_currentUser = UserRecord{};
        emit userLoggedOut();
    }
}

bool UserManager::isLoggedIn() const
{
    return m_loggedIn;
}

UserRecord UserManager::currentUser() const
{
    return m_currentUser;
}

// ─────────────────────────────────────────────────────────────────────────────
// Admin checks
// ─────────────────────────────────────────────────────────────────────────────

bool UserManager::isCurrentUserAdmin() const
{
    return m_loggedIn && m_currentUser.isAdmin;
}

bool UserManager::isAdmin(const QString& username) const
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT is_admin FROM users WHERE username = :u COLLATE NOCASE");
    q.bindValue(":u", username);
    if (!q.exec() || !q.next()) return false;
    return q.value("is_admin").toInt() == 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// User Management
// ─────────────────────────────────────────────────────────────────────────────

QList<UserRecord> UserManager::allUsers() const
{
    QList<UserRecord> users;
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.exec("SELECT id, username, display_name, avatar_color, is_admin, "
           "       created_at, last_login FROM users ORDER BY id");
    while (q.next()) {
        UserRecord r;
        r.id          = q.value("id").toInt();
        r.username    = q.value("username").toString();
        r.displayName = q.value("display_name").toString();
        r.avatarColor = q.value("avatar_color").toString();
        r.isAdmin     = q.value("is_admin").toInt() == 1;
        r.createdAt   = q.value("created_at").toString();
        r.lastLogin   = q.value("last_login").toString();
        users << r;
    }
    return users;
}

bool UserManager::hasAnyUsers() const
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.exec("SELECT COUNT(*) FROM users");
    if (q.next()) {
        return q.value(0).toInt() > 0;
    }
    return false;
}

bool UserManager::updateDisplayName(const QString& newDisplayName)
{
    if (!m_loggedIn || newDisplayName.trimmed().isEmpty()) return false;

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("UPDATE users SET display_name = :name WHERE id = :id");
    q.bindValue(":name", newDisplayName.trimmed());
    q.bindValue(":id",   m_currentUser.id);

    if (!q.exec()) {
        qWarning() << "[UserManager] updateDisplayName failed:" << q.lastError().text();
        return false;
    }
    m_currentUser.displayName = newDisplayName.trimmed();
    emit userDataChanged(m_currentUser);
    return true;
}

bool UserManager::updateAvatarColor(const QString& hexColor)
{
    if (!m_loggedIn) return false;

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("UPDATE users SET avatar_color = :color WHERE id = :id");
    q.bindValue(":color", hexColor);
    q.bindValue(":id",    m_currentUser.id);

    if (!q.exec()) return false;
    m_currentUser.avatarColor = hexColor;
    emit userDataChanged(m_currentUser);
    return true;
}

bool UserManager::updateAvatarPath(const QString& username, const QString& avatarPath)
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("UPDATE users SET avatar_path = :path WHERE username = :user COLLATE NOCASE");
    q.bindValue(":path", avatarPath);
    q.bindValue(":user", username);
    if (!q.exec()) {
        qWarning() << "[UserManager] updateAvatarPath failed:" << q.lastError().text();
        return false;
    }
    if (m_currentUser.username.compare(username, Qt::CaseInsensitive) == 0) {
        m_currentUser.avatarPath = avatarPath;
        emit userDataChanged(m_currentUser);
    }
    return true;
}

bool UserManager::changePassword(const QString& username,
                                  const QString& oldPassword,
                                  const QString& newPassword)
{
    if (newPassword.length() < 4) {
        qWarning() << "[UserManager] New password too short";
        return false;
    }

    // Verify old password first
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT password_hash, salt FROM users WHERE username = :u COLLATE NOCASE");
    q.bindValue(":u", username);
    if (!q.exec() || !q.next()) return false;

    const QString storedHash = q.value("password_hash").toString();
    const QString salt       = q.value("salt").toString();
    if (hashPassword(salt, oldPassword) != storedHash) {
        qWarning() << "[UserManager] changePassword: old password incorrect";
        return false;
    }

    const QString newSalt = generateSalt();
    const QString newHash = hashPassword(newSalt, newPassword);

    QSqlQuery upd(db);
    upd.prepare("UPDATE users SET password_hash = :h, salt = :s WHERE username = :u COLLATE NOCASE");
    upd.bindValue(":h", newHash);
    upd.bindValue(":s", newSalt);
    upd.bindValue(":u", username);
    return upd.exec();
}

bool UserManager::deleteUser(int userId)
{
    if (!m_currentUser.isAdmin) {
        qWarning() << "[UserManager] deleteUser: requires admin";
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("DELETE FROM users WHERE id = :id");
    q.bindValue(":id", userId);
    return q.exec();
}

UserRecord UserManager::userByUsername(const QString& username) const
{
    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);
    QSqlQuery q(db);
    q.prepare("SELECT id, username, display_name, avatar_color, is_admin, "
              "       created_at, last_login FROM users "
              "WHERE username = :u COLLATE NOCASE");
    q.bindValue(":u", username);

    if (!q.exec() || !q.next()) return UserRecord{};

    UserRecord r;
    r.id          = q.value("id").toInt();
    r.username    = q.value("username").toString();
    r.displayName = q.value("display_name").toString();
    r.avatarColor = q.value("avatar_color").toString();
    r.isAdmin     = q.value("is_admin").toInt() == 1;
    r.createdAt   = q.value("created_at").toString();
    r.lastLogin   = q.value("last_login").toString();
    return r;
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

QString UserManager::generateSalt()
{
    // 32 random bytes → 64-char hex string
    QByteArray salt(32, '\0');
    QRandomGenerator* rng = QRandomGenerator::global();
    for (int i = 0; i < 32; ++i) {
        salt[i] = static_cast<char>(rng->bounded(256));
    }
    return QString::fromLatin1(salt.toHex());
}

QString UserManager::hashPassword(const QString& salt, const QString& password)
{
    // SHA-256(salt_hex + password_utf8)
    // ⚠️ Acceptable for offline local use; use Argon2id for production systems
    const QByteArray data = (salt + password).toUtf8();
    return QString::fromLatin1(
        QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex()
    );
}