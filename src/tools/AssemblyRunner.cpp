#include "tools/AssemblyRunner.h"
#include "compiler/CompilerRegistry.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>
#include <QUuid>

AssemblyRunner::AssemblyRunner(QObject* parent)
    : IToolRunner(parent)
{
    // Pick the registry default as the initial compiler
    m_compilerId = CompilerRegistry::instance().defaultCompilerId();
}

AssemblyRunner::~AssemblyRunner() {
    cancel();
}

bool AssemblyRunner::isAvailable() const {
    auto compiler = CompilerRegistry::instance().getCompiler(m_compilerId);
    return compiler && compiler->isAvailable();
}

void AssemblyRunner::setCompilerId(const QString& id) {
    m_compilerId = id;
}

QString AssemblyRunner::compilerId() const {
    return m_compilerId;
}

void AssemblyRunner::setIntelSyntax(bool intel) {
    m_intelSyntax = intel;
}

bool AssemblyRunner::intelSyntax() const {
    return m_intelSyntax;
}

void AssemblyRunner::run(const QString& sourceFile, const QStringList& flags) {
    auto compiler = CompilerRegistry::instance().getCompiler(m_compilerId);
    if (!compiler || !compiler->isAvailable()) {
        emit finished(false, QString(),
            QStringLiteral("Compiler '%1' is not available. "
                           "Please select a valid compiler.").arg(m_compilerId));
        return;
    }

    cancel(); // Kill any running process

    // Temp file for the generated assembly
    QString uuid = QUuid::createUuid().toString().remove('{').remove('}').remove('-');
    m_tmpAsmFile = QDir::tempPath()
        + QStringLiteral("/cppatlas_asm_")
        + uuid
        + QStringLiteral(".s");

    // Build compiler arguments:
    //   <compiler> -S -g [-masm=intel] [flags (includes -std=, -O<n>)] <sourceFile> -o <tmp.s>
    QStringList args;
    args << QStringLiteral("-S");
    args << QStringLiteral("-g");

    if (m_intelSyntax) {
        args << QStringLiteral("-masm=intel");
    }

    args << flags;            // caller supplies -std=<x> and any -O<n>
    args << sourceFile;
    args << QStringLiteral("-o") << m_tmpAsmFile;

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_process, &QProcess::started,
            this, &AssemblyRunner::onProcessStarted);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &AssemblyRunner::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred,
            this, &AssemblyRunner::onProcessError);

    emit progressMessage(
        QStringLiteral("Generating assembly for %1...").arg(
            QFileInfo(sourceFile).fileName()));

    m_process->start(compiler->executablePath(), args);
}

void AssemblyRunner::cancel() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->waitForFinished(1000);
    }
    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
    if (!m_tmpAsmFile.isEmpty()) {
        QFile::remove(m_tmpAsmFile);
        m_tmpAsmFile.clear();
    }
}

void AssemblyRunner::onProcessStarted() {
    emit started();
}

void AssemblyRunner::onProcessFinished(int exitCode, QProcess::ExitStatus status) {
    if (!m_process) return;

    QString errText = QString::fromUtf8(m_process->readAllStandardError());
    bool success = (status == QProcess::NormalExit && exitCode == 0);

    QString asmText;
    if (success) {
        QFile asmFile(m_tmpAsmFile);
        if (asmFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream ts(&asmFile);
            asmText = ts.readAll();
            asmFile.close();
        }
        QFile::remove(m_tmpAsmFile);
        m_tmpAsmFile.clear();

        // Build and emit the source ↔ asm line map
        emit lineMapReady(parseLocDirectives(asmText));
    } else {
        QFile::remove(m_tmpAsmFile);
        m_tmpAsmFile.clear();
    }

    emit finished(success, asmText, errText);

    m_process->deleteLater();
    m_process = nullptr;
}

void AssemblyRunner::onProcessError(QProcess::ProcessError error) {
    static const QMap<QProcess::ProcessError, QString> errors = {
        { QProcess::FailedToStart, QStringLiteral("Failed to start compiler — check path/permissions.") },
        { QProcess::Crashed,       QStringLiteral("Compiler process crashed.") },
        { QProcess::Timedout,      QStringLiteral("Compiler process timed out.") },
        { QProcess::WriteError,    QStringLiteral("Write error to compiler process.") },
        { QProcess::ReadError,     QStringLiteral("Read error from compiler process.") },
    };

    emit finished(false, QString(), errors.value(error, QStringLiteral("Unknown error.")));

    if (!m_tmpAsmFile.isEmpty()) {
        QFile::remove(m_tmpAsmFile);
        m_tmpAsmFile.clear();
    }
    if (m_process) {
        m_process->deleteLater();
        m_process = nullptr;
    }
}

// static
QMap<int, int> AssemblyRunner::parseLocDirectives(const QString& asmText) {
    // .loc directive format (GAS):  .loc <file_num> <line_num> [<col>] [<options>]
    // We only care about the line number (captured group 1).
    static const QRegularExpression locRe(
        QStringLiteral(R"(^\s*\.loc\s+\d+\s+(\d+))"));

    QMap<int, int> asmToSrc;
    int currentSrcLine = -1;
    int asmLine = 1;

    for (const QString& line : asmText.split(QLatin1Char('\n'))) {
        const QRegularExpressionMatch m = locRe.match(line);
        if (m.hasMatch()) {
            currentSrcLine = m.captured(1).toInt();
        }
        if (currentSrcLine > 0) {
            asmToSrc[asmLine] = currentSrcLine;
        }
        ++asmLine;
    }

    return asmToSrc;
}
