#include "mainwindow.h"
#include "quiz/QuizDatabase.h"
#include "quiz/UserManager.h"
#include "ui/LoginDialog.h"
#include "ui/ThemeManager.h"
#include "core/AppSettings.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    // Wayland: prevent native widget sibling issues that cause xdg_surface buffer mismatches
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication a(argc, argv);

    // App identity (used by QSettings)
    QApplication::setOrganizationName("CppAtlas");
    QApplication::setApplicationName("CppAtlas");
    QApplication::setApplicationVersion("0.2");

    // Translations
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
        // Non-fatal: continue without DB (IDE mode still works)
    }

    // ── Show Login Dialog ────────────────────────────────────────────────────
    if (QuizDatabase::instance().isOpen()) {
        LoginDialog loginDlg;
        if (loginDlg.exec() != QDialog::Accepted) {
            // User closed the dialog without logging in — exit gracefully
            return 0;
        }
        // UserManager now has the logged-in user available globally
    }

    // ── Launch Main Window ───────────────────────────────────────────────────
    MainWindow w;
    w.show();

    // Restore per-user window geometry (if available)
    if (UserManager::instance().isLoggedIn()) {
        AppSettings userSettings(UserManager::instance().currentUser().username);
        const QByteArray geometry = userSettings.windowGeometry();
        const QByteArray state    = userSettings.windowState();
        if (!geometry.isEmpty() && !w.isMaximized() && !w.isFullScreen()) {
            w.restoreGeometry(geometry);
        }
        if (!state.isEmpty()) {
            w.restoreState(state);
        }
    }

    return a.exec();
}
