#ifndef COMPILERESULT_H
#define COMPILERESULT_H

#include <QString>
#include <QList>

struct DiagnosticMessage {
    enum Severity { Error, Warning, Note };
    
    Severity severity;
    QString file;
    int line = 0;
    int column = 0;
    QString message;
    QString code;  // Error code if available
};

struct CompileResult {
    bool success = false;
    QString outputFile;
    QString rawOutput;
    QString rawError;
    QList<DiagnosticMessage> diagnostics;
    int exitCode = 0;
    qint64 compilationTimeMs = 0;
};

#endif // COMPILERESULT_H
