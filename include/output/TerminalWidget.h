#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QElapsedTimer>
#include "ui/ThemeManager.h"

class TerminalWidget : public QWidget {
    Q_OBJECT

public:
    explicit TerminalWidget(QWidget *parent = nullptr);

    void runCommand(const QString& command, const QStringList& args = {},
                    const QString& workingDir = {});
    void stopProcess();
    bool isRunning() const;
    void clear();

    QString lastStderr() const;
    void appendText(const QString& text, const QColor& color);

signals:
    void processFinished(int exitCode);
    void processStarted();

private slots:
    void onReadyReadStdout();
    void onReadyReadStderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onProcessError(QProcess::ProcessError error);
    void onInputReturnPressed();
    void onStopClicked();
    void onClearClicked();
    void onThemeChanged();

private:
    void setupUi();
    void applyTheme(const Theme& theme);
    void appendOutput(const QString& text, const QColor& color);
    static QString systemShell();
    static QStringList shellArgs();

    QPlainTextEdit* m_output   = nullptr;
    QLineEdit*      m_input    = nullptr;
    QPushButton*    m_stopBtn  = nullptr;
    QPushButton*    m_clearBtn = nullptr;
    QProcess*       m_process  = nullptr;
    QElapsedTimer   m_timer;
    QString         m_lastStderr;
};
