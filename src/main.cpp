#include "mainwindow.h"

#include "quiz/QuizDatabase.h"
#include "quiz/UserManager.h"
#include "quiz/AdminAccessController.h"
#include "ui/LoginDialog.h"
#include "ui/ThemeManager.h"
#include "core/AppSettings.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QTimer>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);

    QApplication::setOrganizationName("CppAtlas");
    QApplication::setApplicationName("CppAtlas");
    QApplication::setApplicationVersion("0.1");

    // ── Parse command-line options ───────────────────────────────────────────
    QCommandLineParser parser;
    parser.setApplicationDescription("CppAtlas — C++ Learning IDE");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption adminOption(
        QStringLiteral("admin"),
        QStringLiteral("Open admin panel on startup (requires admin account).")
    );
    parser.addOption(adminOption);
    parser.process(a);

    const bool startupAdminRequested = parser.isSet(adminOption);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages) {
        const QString baseName = "cpp-atlas_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // ── Apply last-used theme before any UI shows ────────────────────────────
    // We use a temporary AppSettings at root level to get the last user,
    // then load that user's theme. Falls back to "dark" if no user.
    {
        const QString lastUser = AppSettings::lastLoggedInUser();
        AppSettings tempSettings(lastUser);
        const QString savedTheme = tempSettings.theme();
        ThemeManager::instance()->setTheme(savedTheme);
    }

    // ── Initialize SQLite database ───────────────────────────────────────────
    if (!QuizDatabase::instance().initialize()) {
        QMessageBox::critical(nullptr, "CppAtlas — Database Error",
            "Failed to initialize the quiz database.\n\n"
            "Path: " + QuizDatabase::instance().databasePath() + "\n"
            "Error: " + QuizDatabase::instance().lastError().text() + "\n\n"
            "The application will start without quiz features.");
    }

    // ── Load optional release-admin password hash from environment ───────────
    // Set CPPATLAS_ADMIN_HASH to a SHA-256 hex digest of the admin password
    // before launching in release mode.  See docs/admin_workflow.md.
    {
        const QString adminHash =
            QString::fromUtf8(qgetenv("CPPATLAS_ADMIN_HASH")).trimmed();
        if (!adminHash.isEmpty()) {
            AdminAccessController::instance().setReleaseAdminPasswordHash(adminHash);
        }
    }

    // ── Show Login Dialog ────────────────────────────────────────────────────
    if (QuizDatabase::instance().isOpen()) {
        LoginDialog loginDlg;
        if (loginDlg.exec() != QDialog::Accepted) {
            return 0;
        }
    }

    MainWindow w;
    w.setStartupAdminRequested(startupAdminRequested);
    w.showMaximized();

    // Fire the admin panel after the window is shown and the flag is set.
    // Using QTimer::singleShot here (not in the constructor) ensures the flag
    // has been set before tryOpenAdminPanelFromStartupRequest() runs.
    if (startupAdminRequested) {
        QTimer::singleShot(0, &w, &MainWindow::tryOpenAdminPanelFromStartupRequest);
    }

    return a.exec();
}
