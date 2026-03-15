#include "QuizAdminCli.h"

#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("quiz_admin");
    QCoreApplication::setOrganizationName("CppAtlas");
    QCoreApplication::setApplicationVersion("0.1");

    QuizAdminCli cli;
    return cli.run(app.arguments());
}
