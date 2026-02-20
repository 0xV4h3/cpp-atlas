#include "output/TerminalWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFont>

static constexpr int PROCESS_START_TIMEOUT_MS = 3000;
static constexpr int PROCESS_KILL_TIMEOUT_MS  = 1000;

TerminalWidget::TerminalWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &TerminalWidget::onThemeChanged);
}

void TerminalWidget::setupUi() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* toolbar = new QWidget(this);
    auto* tbLayout = new QHBoxLayout(toolbar);
    tbLayout->setContentsMargins(6, 4, 6, 4);

    m_stopBtn  = new QPushButton("■ Stop",  toolbar);
    m_clearBtn = new QPushButton("Clear", toolbar);
    m_stopBtn->setEnabled(false);

    tbLayout->addWidget(m_stopBtn);
    tbLayout->addWidget(m_clearBtn);
    tbLayout->addStretch();

    m_output = new QPlainTextEdit(this);
    m_output->setReadOnly(true);
    m_output->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    m_output->setMaximumBlockCount(5000);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(9);
    m_output->setFont(font);

    auto* inputRow = new QWidget(this);
    auto* inLayout = new QHBoxLayout(inputRow);
    inLayout->setContentsMargins(6, 2, 6, 4);

    auto* promptLabel = new QLabel("\xe2\x9d\xaf", inputRow);
    m_input = new QLineEdit(inputRow);
    m_input->setPlaceholderText("Enter input or shell command...");
    m_input->setEnabled(false);

    inLayout->addWidget(promptLabel);
    inLayout->addWidget(m_input, 1);

    layout->addWidget(toolbar);
    layout->addWidget(m_output, 1);
    layout->addWidget(inputRow);

    applyTheme(ThemeManager::instance()->currentTheme());

    connect(m_stopBtn,  &QPushButton::clicked, this, &TerminalWidget::onStopClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &TerminalWidget::onClearClicked);
    connect(m_input, &QLineEdit::returnPressed, this, &TerminalWidget::onInputReturnPressed);
}

QString TerminalWidget::systemShell() {
#if defined(Q_OS_WIN)
    return "cmd.exe";
#elif defined(Q_OS_MACOS)
    return qEnvironmentVariable("SHELL", "/bin/zsh");
#else
    return qEnvironmentVariable("SHELL", "/bin/bash");
#endif
}

QStringList TerminalWidget::shellArgs() {
#if defined(Q_OS_WIN)
    return {"/C"};
#else
    return {"-c"};
#endif
}

void TerminalWidget::runCommand(const QString& command,
                                 const QStringList& args,
                                 const QString& workingDir)
{
    if (m_process && m_process->state() != QProcess::NotRunning) {
        appendOutput("[Terminal] A process is already running.\n", QColor("#F44747"));
        return;
    }

    if (!m_process) {
        m_process = new QProcess(this);
    }

    disconnect(m_process, nullptr, this, nullptr);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &TerminalWidget::onReadyReadStdout);
    connect(m_process, &QProcess::readyReadStandardError,  this, &TerminalWidget::onReadyReadStderr);
    connect(m_process, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalWidget::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &TerminalWidget::onProcessError);

    if (!workingDir.isEmpty())
        m_process->setWorkingDirectory(workingDir);

    QString fullCommand = command;
    if (!args.isEmpty())
        fullCommand += " " + args.join(' ');

    m_lastStderr.clear();

    Theme theme = ThemeManager::instance()->currentTheme();
    appendOutput("\xe2\x9d\xaf " + fullCommand + "\n", theme.textSecondary);
    appendOutput(QString("\xe2\x94\x80").repeated(60) + "\n", theme.textSecondary);

    m_timer.start();
    m_stopBtn->setEnabled(true);
    m_input->setEnabled(true);

    m_process->start(systemShell(), shellArgs() << fullCommand);

    if (!m_process->waitForStarted(PROCESS_START_TIMEOUT_MS)) {
        appendOutput("ERROR: Failed to start process.\n", QColor("#F44747"));
        m_stopBtn->setEnabled(false);
        m_input->setEnabled(false);
    } else {
        emit processStarted();
    }
}

void TerminalWidget::stopProcess() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        appendOutput("\n[Terminated by user]\n", QColor("#CCA700"));
        m_process->kill();
        m_process->waitForFinished(PROCESS_KILL_TIMEOUT_MS);
    }
}

bool TerminalWidget::isRunning() const {
    return m_process && m_process->state() != QProcess::NotRunning;
}

void TerminalWidget::clear() {
    m_output->clear();
}

QString TerminalWidget::lastStderr() const {
    return m_lastStderr;
}

void TerminalWidget::appendText(const QString& text, const QColor& color) {
    appendOutput(text, color);
}

void TerminalWidget::onReadyReadStdout() {
    if (!m_process) return;
    QString text = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    Theme theme = ThemeManager::instance()->currentTheme();
    appendOutput(text, theme.textPrimary);
}

void TerminalWidget::onReadyReadStderr() {
    if (!m_process) return;
    QString text = QString::fromLocal8Bit(m_process->readAllStandardError());
    m_lastStderr += text;
    appendOutput(text, QColor("#F48771"));
}

void TerminalWidget::onProcessFinished(int exitCode, QProcess::ExitStatus status) {
    m_stopBtn->setEnabled(false);
    m_input->setEnabled(false);

    Theme theme = ThemeManager::instance()->currentTheme();
    appendOutput(QString("\xe2\x94\x80").repeated(60) + "\n", theme.textSecondary);

    if (status == QProcess::NormalExit) {
        QColor color = (exitCode == 0) ? QColor("#4EC994") : QColor("#F44747");
        appendOutput(QString("Process exited with code %1  (%2 ms)\n")
                         .arg(exitCode).arg(m_timer.elapsed()), color);
    } else {
        appendOutput("Process crashed.\n", QColor("#F44747"));
    }

    emit processFinished(exitCode);
}

void TerminalWidget::onProcessError(QProcess::ProcessError error) {
    static const QMap<QProcess::ProcessError, QString> errors = {
        {QProcess::FailedToStart, "Failed to start \xe2\x80\x94 check path/permissions."},
        {QProcess::Crashed,       "Process crashed."},
        {QProcess::Timedout,      "Timed out."},
        {QProcess::WriteError,    "Write error."},
        {QProcess::ReadError,     "Read error."},
    };
    appendOutput("ERROR: " + errors.value(error, "Unknown error.") + "\n",
                 QColor("#F44747"));
    emit processFinished(-1);
}

void TerminalWidget::onInputReturnPressed() {
    if (m_process && m_process->state() == QProcess::Running) {
        QString input = m_input->text() + "\n";
        m_process->write(input.toLocal8Bit());
        Theme theme = ThemeManager::instance()->currentTheme();
        appendOutput("\xe2\x9d\xaf " + input, theme.textSecondary);
        m_input->clear();
    }
}

void TerminalWidget::onStopClicked()  { stopProcess(); }
void TerminalWidget::onClearClicked() { clear(); }

void TerminalWidget::onThemeChanged() {
    applyTheme(ThemeManager::instance()->currentTheme());
}

void TerminalWidget::applyTheme(const Theme& theme) {
    QPalette p = m_output->palette();
    p.setColor(QPalette::Base, theme.panelBackground);
    p.setColor(QPalette::Text, theme.textPrimary);
    m_output->setPalette(p);
}

void TerminalWidget::appendOutput(const QString& text, const QColor& color) {
    QTextCursor cursor = m_output->textCursor();
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat fmt;
    fmt.setForeground(color);
    cursor.setCharFormat(fmt);
    cursor.insertText(text);
    m_output->setTextCursor(cursor);
    m_output->ensureCursorVisible();
}
