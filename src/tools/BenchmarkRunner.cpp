#include "tools/BenchmarkRunner.h"
#include "tools/ToolsConfig.h"
#include "compiler/CompilerRegistry.h"
#include "compiler/ICompiler.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

// ── Construction ─────────────────────────────────────────────────────────────

BenchmarkRunner::BenchmarkRunner(QObject* parent)
    : IToolRunner(parent)
{
    // Default to first available compiler — overridden by setCompilerId()
    const auto compilers = CompilerRegistry::instance().getAvailableCompilers();
    if (!compilers.isEmpty())
        m_compilerId = compilers.first()->id();
}

BenchmarkRunner::~BenchmarkRunner() { cancel(); }

// ── IToolRunner ───────────────────────────────────────────────────────────────

bool BenchmarkRunner::isAvailable() const {
    return ToolsConfig::instance().isBenchmarkAvailable() &&
           !CompilerRegistry::instance().getAvailableCompilers().isEmpty();
}

void BenchmarkRunner::setCompilerId(const QString& id) { m_compilerId = id; }
QString BenchmarkRunner::compilerId()              const { return m_compilerId; }
BenchmarkResult BenchmarkRunner::lastResult()      const { return m_lastResult; }

void BenchmarkRunner::setResultMetadata(const QString& compilerId,
                                         const QString& standard,
                                         const QString& optimizationLevel) {
    m_lastResult.compilerId        = compilerId;
    m_lastResult.standard          = standard;
    m_lastResult.optimizationLevel = optimizationLevel;
}

QString BenchmarkRunner::extractStandardFromFlags(const QStringList& flags) {
    for (const QString& f : flags) {
        if (f.startsWith(QStringLiteral("-std=")))
            return f.mid(5);
    }
    return QString();
}

QString BenchmarkRunner::extractOptFromFlags(const QStringList& flags) {
    for (const QString& f : flags) {
        if (f.startsWith(QLatin1Char('-')) && f.length() == 3 &&
            (f[1] == QLatin1Char('O') || f[1] == QLatin1Char('o')))
            return f.mid(1);
    }
    return QString();
}

// ── run() — Phase 1: compile ─────────────────────────────────────────────────

void BenchmarkRunner::run(const QString& sourceFile, const QStringList& flags) {
    if (!isAvailable()) {
        m_lastResult = {};
        m_lastResult.errorMessage =
            QStringLiteral("Google Benchmark not available.\n"
                           "Check benchmark include path in Tools > Settings.");
        emit finished(false, {}, m_lastResult.errorMessage);
        return;
    }

    cancel();
    m_sourceFilePath = sourceFile;
    m_compileFlags   = flags;
    m_lastResult     = {};

    // Resolve compiler
    const QString cid = m_compilerId.isEmpty()
        ? CompilerRegistry::instance().getAvailableCompilers().first()->id()
        : m_compilerId;

    auto compiler = CompilerRegistry::instance().getCompiler(cid);
    if (!compiler) {
        const QString msg = QStringLiteral("Compiler not found: %1").arg(cid);
        emit finished(false, {}, msg);
        return;
    }

    // Fresh temp directory for this run
    m_tempDir.reset(new QTemporaryDir());
    if (!m_tempDir->isValid()) {
        emit finished(false, {}, QStringLiteral("Failed to create temporary directory."));
        return;
    }

#ifdef Q_OS_WIN
    m_tempBinaryPath = m_tempDir->filePath(QStringLiteral("bench_out.exe"));
#else
    m_tempBinaryPath = m_tempDir->filePath(QStringLiteral("bench_out"));
#endif

    const ToolsConfig& cfg = ToolsConfig::instance();

    QStringList args;
    args << sourceFile
         << QStringLiteral("-o") << m_tempBinaryPath;
    args << flags;
    args << (QStringLiteral("-I") + cfg.benchmarkIncludeDir());

    // Link benchmark library — absolute path (libbenchmark.a) or flag
    const QString lib = cfg.benchmarkLibrary();
    if (!lib.isEmpty() && QFileInfo::exists(lib)) {
        args << lib;                    // e.g. /build/_deps/googlebenchmark-build/src/libbenchmark.a
    } else {
        args << QStringLiteral("-lbenchmark")
             << QStringLiteral("-lbenchmark_main");
    }
#ifndef Q_OS_WIN
    args << QStringLiteral("-lpthread");
#endif

    m_compileProcess = new QProcess(this);
    m_compileProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_compileProcess,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &BenchmarkRunner::onCompileFinished);
    connect(m_compileProcess, &QProcess::errorOccurred,
            this, &BenchmarkRunner::onCompileError);

    emit progressMessage(
        QStringLiteral("Compiling benchmark with %1...").arg(compiler->name()));
    emit started();
    m_compileProcess->start(compiler->executablePath(), args);
}

void BenchmarkRunner::cancel() {
    for (QProcess* p : {m_compileProcess, m_runProcess}) {
        if (p && p->state() != QProcess::NotRunning) {
            p->kill();
            p->waitForFinished(1000);
            p->deleteLater();
        }
    }
    m_compileProcess = nullptr;
    m_runProcess     = nullptr;
}

// ── Phase 1 slots ─────────────────────────────────────────────────────────────

void BenchmarkRunner::onCompileFinished(int exitCode, QProcess::ExitStatus status) {
    if (!m_compileProcess) return;
    const QString errText =
        QString::fromLocal8Bit(m_compileProcess->readAllStandardError());
    m_compileProcess->deleteLater();
    m_compileProcess = nullptr;

    const bool ok = (status == QProcess::NormalExit && exitCode == 0);
    emit compilationFinished(ok, errText);

    if (!ok) {
        m_lastResult.errorMessage = errText;
        emit finished(false, {}, errText);
        return;
    }
    startRun(m_tempBinaryPath);
}

void BenchmarkRunner::onCompileError(QProcess::ProcessError) {
    const QString msg =
        QStringLiteral("Compile process error: failed to start compiler.");
    emit compilationFinished(false, msg);
    emit finished(false, {}, msg);
    if (m_compileProcess) {
        m_compileProcess->deleteLater();
        m_compileProcess = nullptr;
    }
}

// ── Phase 2: run ──────────────────────────────────────────────────────────────

void BenchmarkRunner::startRun(const QString& binaryPath) {
    m_runProcess = new QProcess(this);
    m_runProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_runProcess,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &BenchmarkRunner::onRunFinished);
    connect(m_runProcess, &QProcess::errorOccurred,
            this, &BenchmarkRunner::onRunError);

    emit progressMessage(QStringLiteral("Running benchmark..."));
    m_runProcess->start(binaryPath,
                        {QStringLiteral("--benchmark_format=json")});
}

void BenchmarkRunner::onRunFinished(int exitCode, QProcess::ExitStatus status) {
    if (!m_runProcess) return;
    const QString jsonOut =
        QString::fromUtf8(m_runProcess->readAllStandardOutput());
    const QString errText =
        QString::fromLocal8Bit(m_runProcess->readAllStandardError());
    m_runProcess->deleteLater();
    m_runProcess = nullptr;

    const bool ok = (status == QProcess::NormalExit && exitCode == 0);
    if (ok) {
        m_lastResult = parseJsonOutput(jsonOut);
        m_lastResult.rawJson           = jsonOut;
        m_lastResult.compilerId        = m_compilerId;
        m_lastResult.standard          = extractStandardFromFlags(m_compileFlags);
        m_lastResult.optimizationLevel = extractOptFromFlags(m_compileFlags);
        m_lastResult.success = true;
        emit benchmarkResultReady(m_lastResult);
        emit finished(true, jsonOut, errText);
    } else {
        m_lastResult.errorMessage = errText.isEmpty() ? jsonOut : errText;
        emit finished(false, {}, m_lastResult.errorMessage);
    }
}

void BenchmarkRunner::onRunError(QProcess::ProcessError) {
    emit finished(false, {},
                  QStringLiteral("Failed to start benchmark binary."));
    if (m_runProcess) {
        m_runProcess->deleteLater();
        m_runProcess = nullptr;
    }
}

// ── JSON parsing ──────────────────────────────────────────────────────────────

BenchmarkResult BenchmarkRunner::parseJsonOutput(const QString& json) const {
    // Google Benchmark JSON:
    // { "context": { "date": "...", ... },
    //   "benchmarks": [ { "name":"BM_Foo", "real_time":42.0,
    //                      "cpu_time":40.0, "iterations":1000000,
    //                      "time_unit":"ns", ... } ] }
    BenchmarkResult result;
    QJsonParseError err;
    const QJsonDocument doc =
        QJsonDocument::fromJson(json.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        result.errorMessage =
            QStringLiteral("JSON parse error: ") + err.errorString();
        return result;
    }

    const QJsonObject root = doc.object();
    result.date = root[QStringLiteral("context")]
                      .toObject()[QStringLiteral("date")].toString();

    static const QStringList knownKeys = {
        "name", "run_name", "run_type", "repetitions", "repetition_index",
        "threads", "iterations", "real_time", "cpu_time", "time_unit",
        "error_occurred", "error_message"
    };

    for (const QJsonValue& v :
             root[QStringLiteral("benchmarks")].toArray()) {
        const QJsonObject obj = v.toObject();
        BenchmarkEntry entry;
        entry.name       = obj[QStringLiteral("name")].toString();
        entry.realTimeNs = obj[QStringLiteral("real_time")].toDouble();
        entry.cpuTimeNs  = obj[QStringLiteral("cpu_time")].toDouble();
        entry.iterations =
            static_cast<qint64>(obj[QStringLiteral("iterations")].toDouble());
        entry.timeUnit   = obj[QStringLiteral("time_unit")].toString();

        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (!knownKeys.contains(it.key()))
                entry.counters[it.key()] = it.value().toVariant();
        }
        result.benchmarks << entry;
    }
    return result;
}

// ── Export ────────────────────────────────────────────────────────────────────

bool BenchmarkRunner::exportToJson(const QString& filePath) const {
    QJsonArray arr;
    for (const BenchmarkEntry& e : m_lastResult.benchmarks) {
        QJsonObject obj;
        obj[QStringLiteral("name")]       = e.name;
        obj[QStringLiteral("real_time")]  = e.realTimeNs;
        obj[QStringLiteral("cpu_time")]   = e.cpuTimeNs;
        obj[QStringLiteral("iterations")] = e.iterations;
        obj[QStringLiteral("time_unit")]  = e.timeUnit;
        arr.append(obj);
    }
    QJsonObject metadata;
    metadata[QStringLiteral("compilerId")]        = m_lastResult.compilerId;
    metadata[QStringLiteral("standard")]          = m_lastResult.standard;
    metadata[QStringLiteral("optimizationLevel")] = m_lastResult.optimizationLevel;
    metadata[QStringLiteral("source_file")]       = m_sourceFilePath;

    QJsonObject root;
    root[QStringLiteral("date")]       = m_lastResult.date;
    root[QStringLiteral("metadata")]   = metadata;
    root[QStringLiteral("benchmarks")] = arr;

    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool BenchmarkRunner::exportToCsv(const QString& filePath) const {
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << "name,real_time_ns,cpu_time_ns,iterations,time_unit\n";
    for (const BenchmarkEntry& e : m_lastResult.benchmarks) {
        out << e.name       << ","
            << e.realTimeNs << ","
            << e.cpuTimeNs  << ","
            << e.iterations << ","
            << e.timeUnit   << "\n";
    }
    return true;
}

BenchmarkResult BenchmarkRunner::loadFromJson(const QString& filePath) const {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return {};
    const QByteArray data = f.readAll();
    f.close();

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) return {};

    const QJsonObject root = doc.object();
    BenchmarkResult result;
    result.success = true;
    result.date    = root[QStringLiteral("date")].toString();

    const QJsonObject meta = root[QStringLiteral("metadata")].toObject();
    result.compilerId        = meta[QStringLiteral("compilerId")].toString();
    result.standard          = meta[QStringLiteral("standard")].toString();
    result.optimizationLevel = meta[QStringLiteral("optimizationLevel")].toString();
    result.label = result.optimizationLevel.isEmpty()
                   ? QFileInfo(filePath).fileName()
                   : result.optimizationLevel;

    for (const QJsonValue& v : root[QStringLiteral("benchmarks")].toArray()) {
        const QJsonObject obj = v.toObject();
        BenchmarkEntry entry;
        entry.name       = obj[QStringLiteral("name")].toString();
        entry.realTimeNs = obj[QStringLiteral("real_time")].toDouble();
        entry.cpuTimeNs  = obj[QStringLiteral("cpu_time")].toDouble();
        entry.iterations =
            static_cast<qint64>(obj[QStringLiteral("iterations")].toDouble());
        entry.timeUnit   = obj[QStringLiteral("time_unit")].toString();
        result.benchmarks << entry;
    }
    return result;
}
