#include "QuizAdminCli.h"
#include "core/DevBuildGuard.h"

#include <QCoreApplication>
#include <QTextStream>

int main(int argc, char* argv[])
{
    // ── Compile-time guard ───────────────────────────────────────────────────
    // This binary is only built when CPPATLAS_DEV_BUILD is ON (see root
    // CMakeLists.txt).  The static_assert below makes it a hard compile error
    // if someone accidentally enables this target in a Release configuration.
#ifndef CPPATLAS_DEV_BUILD
    static_assert(false,
                  "quiz_admin must not be built in non-dev configurations. "
                  "Set CPPATLAS_DEV_BUILD=ON or use a Debug build type.");
#endif

    // ── Runtime guard ────────────────────────────────────────────────────────
    // Belt-and-suspenders: even if the binary somehow ends up on a production
    // machine, it exits immediately unless DevBuild::enabled() is true.
    // Because DevBuild::enabled() is constexpr in Release it is dead-code-
    // eliminated; here in a Dev build it is always true so the check costs
    // nothing at runtime.
    if (!DevBuild::enabled()) {
        QTextStream err(stderr);
        err << "quiz_admin: this tool is for developer use only and is not "
               "available in production builds.\n";
        err.flush();
        return 3;
    }

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("quiz_admin");
    QCoreApplication::setOrganizationName("CppAtlas");
    QCoreApplication::setApplicationVersion("0.1");

    QuizAdminCli cli;
    return cli.run(app.arguments());
}
