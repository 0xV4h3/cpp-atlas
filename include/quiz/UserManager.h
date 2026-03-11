#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>
#include <QList>

/**
 * @brief Represents a CppAtlas local user account.
 */
struct UserRecord {
    int     id           = -1;
    QString username;
    QString displayName;
    QString avatarColor  = "#007ACC";
    QString avatarPath;          // empty = use color circle fallback
    bool    isAdmin      = false;
    QString lastLogin;
    QString createdAt;
};

/**
 * @brief Singleton manager for local multi-user authentication.
 *
 * Provides registration, login, logout, and user enumeration.
 * Passwords are stored as SHA-256(salt + password) hex digests.
 *
 * ⚠️ OMISSION NOTE: SHA-256 + random salt is acceptable for an offline
 * educational tool. A production system should use Argon2id or bcrypt.
 *
 * Usage:
 * @code
 *   auto& um = UserManager::instance();
 *   if (um.registerUser("alice", "Alice Smith", "pass123")) {
 *       um.login("alice", "pass123");
 *   }
 *   UserRecord user = um.currentUser();
 * @endcode
 */
class UserManager : public QObject
{
    Q_OBJECT

public:
    static UserManager& instance();

    // ── Authentication ──────────────────────────────────────────────────────

    /**
     * @brief Register a new user.
     * @return true on success; false if username already taken or DB error
     */
    bool registerUser(const QString& username,
                      const QString& displayName,
                      const QString& password,
                      bool isAdmin = false);

    /**
     * @brief Attempt to log in with username + password.
     * @return true on success
     */
    bool login(const QString& username, const QString& password);

    /**
     * @brief Log out the current user.
     */
    void logout();

    /**
     * @brief Returns true if a user is currently logged in.
     */
    bool isLoggedIn() const;

    /**
     * @brief Returns the currently logged-in user record.
     * If no user is logged in, returns a default-constructed UserRecord (id == -1).
     */
    UserRecord currentUser() const;

    // ── User Management ─────────────────────────────────────────────────────

    /**
     * @brief Returns all registered users (admin use).
     */
    QList<UserRecord> allUsers() const;

    /**
     * @brief Returns true if any users exist in the database.
     */
    bool hasAnyUsers() const;

    /**
     * @brief Change the display name of the current user.
     */
    bool updateDisplayName(const QString& newDisplayName);

    /**
     * @brief Change the avatar color of the current user.
     */
    bool updateAvatarColor(const QString& hexColor);

    /**
     * @brief Change the avatar image path of the current user.
     * @param avatarPath Path to avatar image, or empty string to clear.
     */
    bool updateAvatarPath(const QString& username, const QString& avatarPath);

    /**
     * @brief Change password for a user (admin can change any user; users change own only).
     */
    bool changePassword(const QString& username,
                        const QString& oldPassword,
                        const QString& newPassword);

    /**
     * @brief Delete a user account (admin only).
     */
    bool deleteUser(int userId);

    // ── Helpers ─────────────────────────────────────────────────────────────

    /**
     * @brief Get user record by username. Returns empty record if not found.
     */
    UserRecord userByUsername(const QString& username) const;

signals:
    /** Emitted when a user successfully logs in. */
    void userLoggedIn(const UserRecord& user);

    /** Emitted when the current user logs out. */
    void userLoggedOut();

    /** Emitted when user data changes (e.g., display name update). */
    void userDataChanged(const UserRecord& user);

private:
    explicit UserManager(QObject* parent = nullptr);
    ~UserManager() override = default;
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    /**
     * @brief Generate a cryptographically random hex salt (32 bytes = 64 hex chars).
     */
    static QString generateSalt();

    /**
     * @brief Hash a password: SHA-256(salt + password), returned as hex string.
     * ⚠️ For offline educational use only.
     */
    static QString hashPassword(const QString& salt, const QString& password);

    UserRecord m_currentUser;
    bool       m_loggedIn = false;
};

#endif // USERMANAGER_H