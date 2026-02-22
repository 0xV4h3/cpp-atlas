#include "tools/CppInsightsRunner.h"
#include "tools/ToolsConfig.h"

#include <QFileInfo>
#include <QMap>
#include <QStringList>

CppInsightsRunner::CppInsightsRunner(QObject* parent)
    : IToolRunner(parent)
{
    // Default path from ToolsConfig (populated from tools.json or CMake detection)
    m_execPath = ToolsConfig::instance().cppInsightsPath();
}

CppInsightsRunner::~CppInsightsRunner() {
    cancel();
}

bool CppInsightsRunner::isAvailable() const {
    // Delegate to ToolsConfig which checks the binary exists/runs
    if (!m_execPath.isEmpty() &&
        m_execPath != ToolsConfig::instance().cppInsightsPath()) {
        // Custom path set — probe it directly
        QFileInfo fi(m_execPath);
        return fi.exists() && fi.isExecutable();
    }
    return ToolsConfig::instance().isCppInsightsAvailable();
}

void CppInsightsRunner::setExecutablePath(const QString& path) {
    m_execPath = path;
}

QString CppInsightsRunner::executablePath() const {
    return m_execPath.isEmpty()
        ? ToolsConfig::instance().cppInsightsPath()
        : m_execPath;
}

void CppInsightsRunner::run(const QString& sourceFile, const QStringList& flags) {
    if (!isAvailable()) {
        emit finished(false, QString(),
            QStringLiteral("C++ Insights binary not found. "
                           "Please configure the path in Tools > Settings."));
        return;
    }

    cancel(); // Kill any running process

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_process, &QProcess::started,
            this, &CppInsightsRunner::onProcessStarted);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CppInsightsRunner::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred,
            this, &CppInsightsRunner::onProcessError);

    // Build argument list:
    // insights <source_file> -- [compiler flags e.g. -std=c++17 -O2]
    QStringList args;
    args << sourceFile;
    args << QStringLiteral("--");
    args << flags;

    emit progressMessage(QStringLiteral("Running C++ Insights on %1...").arg(sourceFile));
    m_process->start(executablePath(), args);
}

void CppInsightsRunner::cancel() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->waitForFinished(1000);
    }
    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}

void CppInsightsRunner::onProcessStarted() {
    emit started();
}

void CppInsightsRunner::onProcessFinished(int exitCode, QProcess::ExitStatus status) {
    if (!m_process) return;

    QString output  = QString::fromUtf8(m_process->readAllStandardOutput());
    QString errText = QString::fromUtf8(m_process->readAllStandardError());

    bool success = (status == QProcess::NormalExit && exitCode == 0);
    emit finished(success, output, errText);

    m_process->deleteLater();
    m_process = nullptr;
}

void CppInsightsRunner::onProcessError(QProcess::ProcessError error) {
    static const QMap<QProcess::ProcessError, QString> errors = {
        { QProcess::FailedToStart, QStringLiteral("Failed to start insights — check path/permissions.") },
        { QProcess::Crashed,       QStringLiteral("insights process crashed.") },
        { QProcess::Timedout,      QStringLiteral("insights process timed out.") },
        { QProcess::WriteError,    QStringLiteral("Write error to insights process.") },
        { QProcess::ReadError,     QStringLiteral("Read error from insights process.") },
    };

    emit finished(false, QString(), errors.value(error, QStringLiteral("Unknown error.")));

    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}
