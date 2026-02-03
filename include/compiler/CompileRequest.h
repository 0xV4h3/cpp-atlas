#ifndef COMPILEREQUEST_H
#define COMPILEREQUEST_H

#include <QString>
#include <QStringList>

enum class OptimizationLevel {
    O0, // No optimization
    O1, // Basic optimization
    O2, // Moderate optimization
    O3, // Aggressive optimization
    Os, // Optimize for size
    Oz  // Aggressive size optimization (Clang)
};

struct CompileRequest {
    QString sourceFile;
    QString outputFile;
    QString standard;           // "c++17", "c++20", etc.
    QStringList additionalFlags;
    bool optimizationEnabled = false;
    OptimizationLevel optLevel = OptimizationLevel::O0;
};

#endif // COMPILEREQUEST_H
