#include "compiler/CompilerRegistry.h"
#include "compiler/GccCompiler.h"
#include "compiler/ClangCompiler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>

CompilerRegistry& CompilerRegistry::instance() {
    static CompilerRegistry instance;
    return instance;
}

void CompilerRegistry::registerCompiler(QSharedPointer<ICompiler> compiler) {
    if (compiler) {
        m_compilers[compiler->id()] = compiler;
        emit compilersChanged();
    }
}

void CompilerRegistry::unregisterCompiler(const QString& compilerId) {
    if (m_compilers.remove(compilerId) > 0) {
        emit compilersChanged();
    }
}

QSharedPointer<ICompiler> CompilerRegistry::getCompiler(const QString& compilerId) const {
    return m_compilers.value(compilerId);
}

QList<QSharedPointer<ICompiler>> CompilerRegistry::getCompilers() const {
    return m_compilers.values();
}

QList<QSharedPointer<ICompiler>> CompilerRegistry::getAvailableCompilers() const {
    QList<QSharedPointer<ICompiler>> available;
    for (const auto& compiler : m_compilers) {
        if (compiler->isAvailable()) {
            available.append(compiler);
        }
    }
    return available;
}

QString CompilerRegistry::findInPath(const QString& name) const {
    QProcess process;
    
#ifdef Q_OS_WIN
    process.start("where", QStringList() << name);
#else
    process.start("which", QStringList() << name);
#endif
    
    process.waitForFinished(3000);
    if (process.exitCode() == 0) {
        QString output = process.readAllStandardOutput().trimmed();
        return output.split('\n').first();  // Return first match
    }
    return QString();
}

void CompilerRegistry::autoScanCompilers() {
    // Try to find GCC/G++
    QString gccPath = findInPath("g++");
    if (!gccPath.isEmpty()) {
        auto gcc = QSharedPointer<GccCompiler>::create(gccPath, "gcc-system");
        if (gcc->isAvailable()) {
            registerCompiler(gcc);
            if (m_defaultCompilerId.isEmpty()) {
                m_defaultCompilerId = gcc->id();
            }
        }
    }
    
    // Try to find Clang++
    QString clangPath = findInPath("clang++");
    if (!clangPath.isEmpty()) {
        auto clang = QSharedPointer<ClangCompiler>::create(clangPath, "clang-system");
        if (clang->isAvailable()) {
            registerCompiler(clang);
            if (m_defaultCompilerId.isEmpty()) {
                m_defaultCompilerId = clang->id();
            }
        }
    }
    
    // Platform-specific paths
#ifdef Q_OS_LINUX
    // Check common Linux paths
    QStringList linuxPaths = {
        "/usr/bin/g++", "/usr/bin/clang++",
        "/usr/local/bin/g++", "/usr/local/bin/clang++"
    };
    for (const QString& path : linuxPaths) {
        if (QFile::exists(path) && !m_compilers.contains(path)) {
            if (path.contains("g++")) {
                auto gcc = QSharedPointer<GccCompiler>::create(path, "gcc-" + path);
                if (gcc->isAvailable()) {
                    registerCompiler(gcc);
                }
            } else if (path.contains("clang++")) {
                auto clang = QSharedPointer<ClangCompiler>::create(path, "clang-" + path);
                if (clang->isAvailable()) {
                    registerCompiler(clang);
                }
            }
        }
    }
#endif
    
#ifdef Q_OS_WIN
    // Check common Windows MinGW/LLVM paths
    QStringList winPaths = {
        "C:/MinGW/bin/g++.exe",
        "C:/Program Files/LLVM/bin/clang++.exe",
        "C:/msys64/mingw64/bin/g++.exe"
    };
    for (const QString& path : winPaths) {
        if (QFile::exists(path)) {
            if (path.contains("g++")) {
                auto gcc = QSharedPointer<GccCompiler>::create(path, "gcc-" + path);
                if (gcc->isAvailable()) {
                    registerCompiler(gcc);
                }
            } else if (path.contains("clang++")) {
                auto clang = QSharedPointer<ClangCompiler>::create(path, "clang-" + path);
                if (clang->isAvailable()) {
                    registerCompiler(clang);
                }
            }
        }
    }
#endif
    
#ifdef Q_OS_MAC
    // Check Xcode and Homebrew paths
    QStringList macPaths = {
        "/usr/bin/clang++",
        "/usr/local/bin/g++",
        "/opt/homebrew/bin/g++",
        "/opt/homebrew/bin/clang++"
    };
    for (const QString& path : macPaths) {
        if (QFile::exists(path) && !m_compilers.contains(path)) {
            if (path.contains("g++")) {
                auto gcc = QSharedPointer<GccCompiler>::create(path, "gcc-" + path);
                if (gcc->isAvailable()) {
                    registerCompiler(gcc);
                }
            } else if (path.contains("clang++")) {
                auto clang = QSharedPointer<ClangCompiler>::create(path, "clang-" + path);
                if (clang->isAvailable()) {
                    registerCompiler(clang);
                }
            }
        }
    }
#endif
}

bool CompilerRegistry::loadConfiguration(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // Load default compiler and standard
    m_defaultCompilerId = root["defaultCompiler"].toString();
    m_defaultStandard = root["defaultStandard"].toString("c++17");
    
    // Load compilers array
    QJsonArray compilersArray = root["compilers"].toArray();
    for (const QJsonValue& value : compilersArray) {
        QJsonObject obj = value.toObject();
        QString id = obj["id"].toString();
        QString type = obj["type"].toString();
        QString name = obj["name"].toString();
        QString path = obj["path"].toString();
        
        if (type == "gcc") {
            auto gcc = QSharedPointer<GccCompiler>::create(path, id);
            if (!obj["autoDetected"].toBool() || gcc->isAvailable()) {
                registerCompiler(gcc);
            }
        } else if (type == "clang") {
            auto clang = QSharedPointer<ClangCompiler>::create(path, id);
            if (!obj["autoDetected"].toBool() || clang->isAvailable()) {
                registerCompiler(clang);
            }
        }
    }
    
    return true;
}

bool CompilerRegistry::saveConfiguration(const QString& filePath) {
    QJsonObject root;
    root["defaultCompiler"] = m_defaultCompilerId;
    root["defaultStandard"] = m_defaultStandard;
    
    QJsonArray compilersArray;
    for (const auto& compiler : m_compilers) {
        QJsonObject obj;
        obj["id"] = compiler->id();
        obj["name"] = compiler->name();
        obj["path"] = compiler->executablePath();
        obj["autoDetected"] = true;
        
        // Determine type
        if (qobject_cast<GccCompiler*>(compiler.data())) {
            obj["type"] = "gcc";
        } else if (qobject_cast<ClangCompiler*>(compiler.data())) {
            obj["type"] = "clang";
        }
        
        compilersArray.append(obj);
    }
    root["compilers"] = compilersArray;
    
    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

QString CompilerRegistry::defaultCompilerId() const {
    return m_defaultCompilerId;
}

void CompilerRegistry::setDefaultCompilerId(const QString& compilerId) {
    m_defaultCompilerId = compilerId;
}

QString CompilerRegistry::defaultStandard() const {
    return m_defaultStandard;
}

void CompilerRegistry::setDefaultStandard(const QString& standard) {
    m_defaultStandard = standard;
}
