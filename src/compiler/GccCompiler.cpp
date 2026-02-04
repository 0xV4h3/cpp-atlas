#include "compiler/GccCompiler.h"
#include <QProcess>
#include <QFileInfo>
#include <QRegularExpression>
#include <QElapsedTimer>

GccCompiler::GccCompiler(const QString& execPath, const QString& compilerId)
    : m_id(compilerId), m_execPath(execPath)
{
}

QString GccCompiler::id() const {
    return m_id;
}

QString GccCompiler::name() const {
    return QString("GCC %1").arg(version());
}

QString GccCompiler::executablePath() const {
    return m_execPath;
}

QStringList GccCompiler::supportedStandards() const {
    return {"c++11", "c++14", "c++17", "c++20", "c++23"};
}

QString GccCompiler::defaultStandard() const {
    return "c++17";
}

bool GccCompiler::isAvailable() const {
    QProcess process;
    process.start(m_execPath, QStringList() << "--version");
    process.waitForFinished(3000);
    return process.exitCode() == 0;
}

QString GccCompiler::version() const {
    if (!m_versionQueried) {
        QProcess process;
        process.start(m_execPath, QStringList() << "--version");
        process.waitForFinished(3000);
        QString output = process.readAllStandardOutput();
        
        // Parse version from first line: g++ (GCC) 13.2.0
        QRegularExpression versionRe(R"((\d+\.\d+\.\d+))");
        QRegularExpressionMatch match = versionRe.match(output);
        if (match.hasMatch()) {
            m_version = match.captured(1);
        } else {
            m_version = "Unknown";
        }
        m_versionQueried = true;
    }
    return m_version;
}

QString GccCompiler::optimizationFlag(OptimizationLevel level) const {
    switch (level) {
        case OptimizationLevel::O0: return "-O0";
        case OptimizationLevel::O1: return "-O1";
        case OptimizationLevel::O2: return "-O2";
        case OptimizationLevel::O3: return "-O3";
        case OptimizationLevel::Os: return "-Os";
        case OptimizationLevel::Oz: return "-Oz";  // GCC doesn't have -Oz, use -Os
    }
    return "-O0";
}

CompileResult GccCompiler::compile(const CompileRequest& request) {
    CompileResult result;
    QElapsedTimer timer;
    timer.start();
    
    QStringList args;
    args << request.sourceFile;
    args << "-o" << request.outputFile;
    args << "-std=" + request.standard;
    
    if (request.optimizationEnabled) {
        args << optimizationFlag(request.optLevel);
    }
    
    args << request.additionalFlags;
    
    QProcess process;
    process.start(m_execPath, args);
    process.waitForFinished(60000);  // 60 second timeout
    
    result.compilationTimeMs = timer.elapsed();
    result.exitCode = process.exitCode();
    result.success = (process.exitCode() == 0);
    result.outputFile = request.outputFile;
    result.rawOutput = process.readAllStandardOutput();
    result.rawError = process.readAllStandardError();
    
    // Parse diagnostics from stderr
    result.diagnostics = parseDiagnostics(result.rawError);
    
    return result;
}

QList<DiagnosticMessage> GccCompiler::parseDiagnostics(const QString& output) {
    QList<DiagnosticMessage> diagnostics;
    
    // GCC format: file:line:col: severity: message
    QRegularExpression re(R"(([^:]+):(\d+):(\d+):\s+(error|warning|note):\s+(.+))");
    QRegularExpressionMatchIterator it = re.globalMatch(output);
    
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        DiagnosticMessage diag;
        diag.file = match.captured(1);
        diag.line = match.captured(2).toInt();
        diag.column = match.captured(3).toInt();
        
        QString severity = match.captured(4);
        if (severity == "error") {
            diag.severity = DiagnosticMessage::Error;
        } else if (severity == "warning") {
            diag.severity = DiagnosticMessage::Warning;
        } else {
            diag.severity = DiagnosticMessage::Note;
        }
        
        diag.message = match.captured(5);
        diagnostics.append(diag);
    }
    
    return diagnostics;
}

QProcess* GccCompiler::runExecutable(const QString& exePath, const QStringList& args) {
    QProcess* process = new QProcess();
    process->setProgram(exePath);
    process->setArguments(args);
    return process;
}
