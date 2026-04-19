#include "mainwindow.h"

#include "quiz/QuizDatabase.h"
#include "quiz/UserManager.h"
#include "core/DevBuildGuard.h"
#include "ui/LoginDialog.h"
#include "ui/ThemeManager.h"
#include "core/AppSettings.h"

#ifdef CPPATLAS_DEV_BUILD
#  include "quiz/AdminAccessController.h"
#endif

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

#ifdef CPPATLAS_DEV_BUILD
    QCommandLineOption adminOption(
        QStringLiteral("admin"),
        QStringLiteral("(Developer only) Open admin panel on startup.")
        );
    parser.addOption(adminOption);
#endif

    parser.process(a);

    bool startupAdminRequested = false;
#ifdef CPPATLAS_DEV_BUILD
    startupAdminRequested = parser.isSet(adminOption);
#endif

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

#ifdef CPPATLAS_DEV_BUILD
    {
        const QString adminHash =
            QString::fromUtf8(qgetenv("CPPATLAS_ADMIN_HASH")).trimmed();
        if (!adminHash.isEmpty()) {
            AdminAccessController::instance().setReleaseAdminPasswordHash(adminHash);
        }
    }
#endif

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

    if (startupAdminRequested) {
        QTimer::singleShot(0, &w, &MainWindow::tryOpenAdminPanelFromStartupRequest);
    }

    return a.exec();
}
