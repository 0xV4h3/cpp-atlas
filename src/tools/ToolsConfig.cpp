#include "tools/ToolsConfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

// CMake-injected compile-time hints (empty strings if not found at configure time)
#ifndef CPPINSIGHTS_DEFAULT_PATH
#define CPPINSIGHTS_DEFAULT_PATH ""
#endif

#ifndef GOOGLE_BENCHMARK_INCLUDE_DIR
#define GOOGLE_BENCHMARK_INCLUDE_DIR ""
#endif

#ifndef GOOGLE_BENCHMARK_LIB_TARGET
#define GOOGLE_BENCHMARK_LIB_TARGET "benchmark"
#endif

// ---------------------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------------------

ToolsConfig& ToolsConfig::instance() {
    static ToolsConfig s_instance;
    return s_instance;
}

ToolsConfig::ToolsConfig()
    : QObject(nullptr)
    , m_cppInsightsPath(QLatin1String(CPPINSIGHTS_DEFAULT_PATH))
    , m_benchmarkIncludeDir(QLatin1String(GOOGLE_BENCHMARK_INCLUDE_DIR))
    , m_benchmarkLibrary(QLatin1String(GOOGLE_BENCHMARK_LIB_TARGET))
{
    // Auto-detect if compile-time path was not injected
    if (m_cppInsightsPath.isEmpty() && m_insightsAutoDetect) {
        m_cppInsightsPath = autoDetectCppInsights();
    }
    if (m_benchmarkIncludeDir.isEmpty() && m_benchmarkAutoDetect) {
        m_benchmarkIncludeDir = autoDetectBenchmarkInclude();
    }
}

// ---------------------------------------------------------------------------
// CppInsights
// ---------------------------------------------------------------------------

QString ToolsConfig::cppInsightsPath() const {
    return m_cppInsightsPath;
}

void ToolsConfig::setCppInsightsPath(const QString& path) {
    if (m_cppInsightsPath != path) {
        m_cppInsightsPath = path;
        emit configChanged();
    }
}

bool ToolsConfig::isCppInsightsAvailable() const {
    if (m_cppInsightsPath.isEmpty()) {
        return false;
    }
    // Check absolute or relative path first
    if (QFileInfo::exists(m_cppInsightsPath)) {
        return true;
    }
    // Probe via PATH (for bare name like "insights")
    QProcess probe;
    probe.start(m_cppInsightsPath, QStringList() << "--help");
    probe.waitForFinished(3000);
    return probe.exitStatus() == QProcess::NormalExit && probe.exitCode() == 0;
}

// ---------------------------------------------------------------------------
// Google Benchmark
// ---------------------------------------------------------------------------

QString ToolsConfig::benchmarkIncludeDir() const {
    return m_benchmarkIncludeDir;
}

void ToolsConfig::setBenchmarkIncludeDir(const QString& dir) {
    if (m_benchmarkIncludeDir != dir) {
        m_benchmarkIncludeDir = dir;
        emit configChanged();
    }
}

QString ToolsConfig::benchmarkLibrary() const {
    return m_benchmarkLibrary;
}

void ToolsConfig::setBenchmarkLibrary(const QString& lib) {
    if (m_benchmarkLibrary != lib) {
        m_benchmarkLibrary = lib;
        emit configChanged();
    }
}

bool ToolsConfig::isBenchmarkAvailable() const {
    if (m_benchmarkIncludeDir.isEmpty()) {
        return false;
    }
    QString headerPath = QDir(m_benchmarkIncludeDir).filePath("benchmark/benchmark.h");
    return QFileInfo::exists(headerPath);
}

// ---------------------------------------------------------------------------
// Benchmark defaults
// ---------------------------------------------------------------------------

QString ToolsConfig::defaultBenchmarkCompilerId() const {
    return m_defaultCompilerId;
}

void ToolsConfig::setDefaultBenchmarkCompilerId(const QString& id) {
    if (m_defaultCompilerId != id) {
        m_defaultCompilerId = id;
        emit configChanged();
    }
}

QString ToolsConfig::defaultBenchmarkStandard() const {
    return m_defaultStandard;
}

void ToolsConfig::setDefaultBenchmarkStandard(const QString& standard) {
    if (m_defaultStandard != standard) {
        m_defaultStandard = standard;
        emit configChanged();
    }
}

QString ToolsConfig::defaultBenchmarkOptimization() const {
    return m_defaultOptimization;
}

void ToolsConfig::setDefaultBenchmarkOptimization(const QString& opt) {
    if (m_defaultOptimization != opt) {
        m_defaultOptimization = opt;
        emit configChanged();
    }
}

// ---------------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------------

bool ToolsConfig::loadConfiguration(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    QJsonObject root = doc.object();

    // --- tools section ---
    QJsonObject tools = root["tools"].toObject();

    QJsonObject insights = tools["cppinsights"].toObject();
    m_insightsAutoDetect = insights["autoDetect"].toBool(true);
    QString insightsPath = insights["path"].toString();
    if (!insightsPath.isEmpty()) {
        m_cppInsightsPath = insightsPath;
    } else if (m_insightsAutoDetect && m_cppInsightsPath.isEmpty()) {
        m_cppInsightsPath = autoDetectCppInsights();
    }

    QJsonObject benchmark = tools["benchmark"].toObject();
    m_benchmarkAutoDetect = benchmark["autoDetect"].toBool(true);
    QString includeDir = benchmark["includeDir"].toString();
    if (!includeDir.isEmpty()) {
        m_benchmarkIncludeDir = includeDir;
    } else if (m_benchmarkAutoDetect && m_benchmarkIncludeDir.isEmpty()) {
        m_benchmarkIncludeDir = autoDetectBenchmarkInclude();
    }
    QString lib = benchmark["library"].toString();
    if (!lib.isEmpty()) {
        m_benchmarkLibrary = lib;
    }

    // --- benchmarkDefaults section ---
    QJsonObject defaults = root["benchmarkDefaults"].toObject();
    if (defaults.contains("compiler")) {
        m_defaultCompilerId = defaults["compiler"].toString();
    }
    if (defaults.contains("standard")) {
        m_defaultStandard = defaults["standard"].toString();
    }
    if (defaults.contains("optimizationLevel")) {
        m_defaultOptimization = defaults["optimizationLevel"].toString();
    }

    m_configFilePath = filePath;
    emit configChanged();
    return true;
}

bool ToolsConfig::saveConfiguration(const QString& filePath) const {
    QJsonObject root;

    QJsonObject tools;

    QJsonObject insights;
    insights["path"]       = m_cppInsightsPath;
    insights["autoDetect"] = m_insightsAutoDetect;
    tools["cppinsights"]   = insights;

    QJsonObject benchmark;
    benchmark["includeDir"] = m_benchmarkIncludeDir;
    benchmark["library"]    = m_benchmarkLibrary;
    benchmark["autoDetect"] = m_benchmarkAutoDetect;
    tools["benchmark"]      = benchmark;

    root["tools"] = tools;

    QJsonObject defaults;
    defaults["compiler"]          = m_defaultCompilerId;
    defaults["standard"]          = m_defaultStandard;
    defaults["optimizationLevel"] = m_defaultOptimization;
    root["benchmarkDefaults"]     = defaults;

    QJsonDocument doc(root);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

QString ToolsConfig::configFilePath() const {
    return m_configFilePath;
}

// ---------------------------------------------------------------------------
// Auto-detection helpers
// ---------------------------------------------------------------------------

QString ToolsConfig::autoDetectCppInsights() const {
    // 1. Check bundled third_party location
    QString bundledBase = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + "/../third_party/cppinsights/bin/insights");
    if (QFileInfo::exists(bundledBase)) {
        return QFileInfo(bundledBase).absoluteFilePath();
    }
#ifdef Q_OS_WIN
    QString bundledExe = bundledBase + ".exe";
    if (QFileInfo::exists(bundledExe)) {
        return QFileInfo(bundledExe).absoluteFilePath();
    }
#endif

    // 2. Check system PATH
    QProcess probe;
#ifdef Q_OS_WIN
    probe.start("where", QStringList() << "insights.exe");
#else
    probe.start("which", QStringList() << "insights");
#endif
    probe.waitForFinished(3000);
    if (probe.exitCode() == 0) {
        QString found = QString::fromLocal8Bit(probe.readAllStandardOutput()).trimmed();
        if (!found.isEmpty()) {
            return found.split('\n').first().trimmed();
        }
    }

    // 3. Common install locations
    QStringList candidates;
#ifdef Q_OS_WIN
    candidates << "C:/Program Files/CppInsights/insights.exe";
#elif defined(Q_OS_MACOS)
    candidates << "/usr/local/bin/insights"
               << "/opt/homebrew/bin/insights";
#else
    candidates << "/usr/bin/insights"
               << "/usr/local/bin/insights"
               << QDir::homePath() + "/.local/bin/insights";
#endif
    for (const QString& c : candidates) {
        if (QFileInfo::exists(c)) {
            return c;
        }
    }

    return QString(); // Not found — user must configure manually
}

QString ToolsConfig::autoDetectBenchmarkInclude() const {
    // CMake FetchContent populates GOOGLE_BENCHMARK_INCLUDE_DIR at compile time
    QString compiletimePath = QLatin1String(GOOGLE_BENCHMARK_INCLUDE_DIR);
    if (!compiletimePath.isEmpty() && QFileInfo::exists(compiletimePath)) {
        return compiletimePath;
    }

    // Fallback: common system install paths
    QStringList candidates;
#ifdef Q_OS_WIN
    candidates << "C:/Program Files/benchmark/include";
#elif defined(Q_OS_MACOS)
    candidates << "/usr/local/include"
               << "/opt/homebrew/include";
#else
    candidates << "/usr/include"
               << "/usr/local/include";
#endif
    for (const QString& c : candidates) {
        if (QFileInfo::exists(c + "/benchmark/benchmark.h")) {
            return c;
        }
    }

    return QString();
}
