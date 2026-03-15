#include "quiz/AdminAccessController.h"
#include "quiz/UserManager.h"

#include <QCryptographicHash>
#include <QInputDialog>
#include <QLineEdit>
#include <QWidget>

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
    m_releaseAdminPasswordHash = hashHex;
}
