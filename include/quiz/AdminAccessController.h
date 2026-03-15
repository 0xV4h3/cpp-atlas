#pragma once

#include <QObject>
#include <QString>

class QWidget;

enum class AdminAccessResult {
    Granted,
    NotLoggedIn,
    NotAdmin,
    PasswordRejected,
    Error
};

class AdminAccessController : public QObject
{
    Q_OBJECT
public:
    static AdminAccessController& instance();

    bool canAttemptAdminEntry() const;
    bool isCurrentUserAdmin() const;

    // requireReleasePassword should be true in release flow, false in debug flow.
    AdminAccessResult verifyAccess(QWidget* parent, bool requireReleasePassword) const;

    // Hash format: SHA-256 hex (same style as current project hashing utilities).
    void setReleaseAdminPasswordHash(const QString& hashHex);

private:
    explicit AdminAccessController(QObject* parent = nullptr);
    QString m_releaseAdminPasswordHash;
};
