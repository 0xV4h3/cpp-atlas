#include "quiz/AdminAccessController.h"
#include "quiz/UserManager.h"

#include <QCryptographicHash>
#include <QInputDialog>
#include <QLineEdit>
#include <QRegularExpression>
#include <QWidget>
#include <QDebug>

AdminAccessController& AdminAccessController::instance()
{
    static AdminAccessController s_instance;
    return s_instance;
}

AdminAccessController::AdminAccessController(QObject* parent)
    : QObject(parent)
{
}

bool AdminAccessController::canAttemptAdminEntry() const
{
    return UserManager::instance().isLoggedIn();
}

bool AdminAccessController::isCurrentUserAdmin() const
{
    return UserManager::instance().isCurrentUserAdmin();
}

AdminAccessResult AdminAccessController::verifyAccess(QWidget* parent, bool requireReleasePassword) const
{
    if (!UserManager::instance().isLoggedIn())
        return AdminAccessResult::NotLoggedIn;

    if (!UserManager::instance().isCurrentUserAdmin())
        return AdminAccessResult::NotAdmin;

    if (requireReleasePassword) {
        if (m_releaseAdminPasswordHash.isEmpty())
            return AdminAccessResult::Error;

        bool ok = false;
        const QString password = QInputDialog::getText(
            parent,
            QObject::tr("Admin Authentication"),
            QObject::tr("Enter admin password:"),
            QLineEdit::Password,
            QString(),
            &ok
        );

        if (!ok)
            return AdminAccessResult::PasswordRejected;

        const QString hexHash = QString::fromLatin1(
            QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()
        );

        if (hexHash != m_releaseAdminPasswordHash)
            return AdminAccessResult::PasswordRejected;
    }

    return AdminAccessResult::Granted;
}

void AdminAccessController::setReleaseAdminPasswordHash(const QString& hashHex)
{
    // Accept an empty string (clears the hash) or exactly 64 lowercase hex digits
    // (SHA-256 output).  Anything else is silently rejected with a warning so that
    // a malformed CPPATLAS_ADMIN_HASH env value does not look like success.
    static const QRegularExpression kSha256HexPattern(QStringLiteral("^[0-9a-f]{64}$"));
    if (!hashHex.isEmpty() && !kSha256HexPattern.match(hashHex).hasMatch()) {
        qWarning() << "[AdminAccessController] Ignoring invalid admin password hash"
                      " (expected 64 lowercase hex chars or empty to clear):"
                   << hashHex.left(16) + "...";
        return;
    }
    m_releaseAdminPasswordHash = hashHex;
}
