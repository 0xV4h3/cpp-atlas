#include "output/RunOutputWidget.h"
#include "ui/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

RunOutputWidget::RunOutputWidget(QWidget *parent)
    : QWidget(parent), m_process(nullptr)
{
    setupUi();

    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &RunOutputWidget::onThemeChanged);
}

void RunOutputWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Toolbar
    QWidget* toolbar = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    
    QLabel* argsLabel = new QLabel("Arguments:", toolbar);
    m_argsEdit = new QLineEdit(toolbar);
    m_argsEdit->setPlaceholderText("Command-line arguments...");
    
    m_stopButton = new QPushButton("Stop", toolbar);
    m_stopButton->setEnabled(false);
    
    m_clearButton = new QPushButton("Clear", toolbar);
    
    toolbarLayout->addWidget(argsLabel);
    toolbarLayout->addWidget(m_argsEdit, 1);
    toolbarLayout->addWidget(m_stopButton);
    toolbarLayout->addWidget(m_clearButton);
    
    // Output area
    m_outputEdit = new QPlainTextEdit(this);
    m_outputEdit->setReadOnly(true);
    m_outputEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(9);
    m_outputEdit->setFont(font);
    
    // Set theme-aware colors
    Theme theme = ThemeManager::instance()->currentTheme();
    QPalette p = m_outputEdit->palette();
    p.setColor(QPalette::Base, theme.panelBackground);
    p.setColor(QPalette::Text, theme.textPrimary);
    m_outputEdit->setPalette(p);
    
    // Input area
    QGroupBox* inputGroup = new QGroupBox("Standard Input", this);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputGroup);
    
    m_inputEdit = new QLineEdit(inputGroup);
    m_inputEdit->setPlaceholderText("Type input and press Enter...");
    m_inputEdit->setEnabled(false);
    
    inputLayout->addWidget(m_inputEdit);
    
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(m_outputEdit, 1);
    mainLayout->addWidget(inputGroup);
    
    // Connect signals
    connect(m_stopButton, &QPushButton::clicked, this, &RunOutputWidget::onStopClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &RunOutputWidget::onClearClicked);
    connect(m_inputEdit, &QLineEdit::returnPressed, this, &RunOutputWidget::onInputReturnPressed);
}

void RunOutputWidget::runProgram(const QString& program, const QStringList& arguments) {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        appendStderr("ERROR: A program is already running.\n");
        return;
    }
    
    clear();
    
    if (m_process) {
        delete m_process;
    }
    
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, 
            this, &RunOutputWidget::onProcessReadyReadStdout);
    connect(m_process, &QProcess::readyReadStandardError, 
            this, &RunOutputWidget::onProcessReadyReadStderr);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &RunOutputWidget::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, 
            this, &RunOutputWidget::onProcessError);
    
    m_timer.start();
    
    QString argsText = m_argsEdit->text().trimmed();
    QStringList allArgs = arguments;
    if (!argsText.isEmpty()) {
        allArgs.append(argsText.split(' ', Qt::SkipEmptyParts));
    }
    
    appendStdout(QString("Running: %1 %2\n").arg(program).arg(allArgs.join(' ')));
    appendStdout(QString("─").repeated(60) + "\n");
    
    m_process->start(program, allArgs);
    
    if (m_process->waitForStarted(3000)) {
        m_stopButton->setEnabled(true);
        m_inputEdit->setEnabled(true);
        emit programStarted();
    } else {
        appendStderr("ERROR: Failed to start program.\n");
        emit programError("Failed to start program");
    }
}

void RunOutputWidget::stopProgram() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        appendStderr("\nProgram terminated by user.\n");
        m_process->kill();
        m_process->waitForFinished(1000);
    }
}

bool RunOutputWidget::isRunning() const {
    return m_process && m_process->state() != QProcess::NotRunning;
}

void RunOutputWidget::clear() {
    m_outputEdit->clear();
}

void RunOutputWidget::setArguments(const QString& args) {
    m_argsEdit->setText(args);
}

QString RunOutputWidget::arguments() const {
    return m_argsEdit->text();
}

void RunOutputWidget::onStopClicked() {
    stopProgram();
}

void RunOutputWidget::onClearClicked() {
    clear();
}

void RunOutputWidget::onInputReturnPressed() {
    if (m_process && m_process->state() == QProcess::Running) {
        QString input = m_inputEdit->text() + "\n";
        m_process->write(input.toUtf8());
        m_inputEdit->clear();
        
        // Echo input to output
        appendStdout(input);
    }
}

void RunOutputWidget::onProcessReadyReadStdout() {
    if (m_process) {
        QString output = QString::fromUtf8(m_process->readAllStandardOutput());
        appendStdout(output);
    }
}

void RunOutputWidget::onProcessReadyReadStderr() {
    if (m_process) {
        QString error = QString::fromUtf8(m_process->readAllStandardError());
        appendStderr(error);
    }
}

void RunOutputWidget::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    m_stopButton->setEnabled(false);
    m_inputEdit->setEnabled(false);
    
    qint64 elapsed = m_timer.elapsed();
    
    appendStdout(QString("\n") + QString("─").repeated(60) + "\n");
    
    if (exitStatus == QProcess::NormalExit) {
        if (exitCode == 0) {
            appendStdout(QString("Program finished successfully (exit code: %1)\n").arg(exitCode));
        } else {
            appendStderr(QString("Program finished with exit code: %1\n").arg(exitCode));
        }
    } else {
        appendStderr("Program crashed.\n");
    }
    
    appendStdout(QString("Elapsed time: %1 ms\n").arg(elapsed));
    
    emit programFinished(exitCode);
}

void RunOutputWidget::onProcessError(QProcess::ProcessError error) {
    QString errorMsg;
    
    switch (error) {
        case QProcess::FailedToStart:
            errorMsg = "Failed to start program. Check if the file exists and is executable.";
            break;
        case QProcess::Crashed:
            errorMsg = "Program crashed.";
            break;
        case QProcess::Timedout:
            errorMsg = "Program timed out.";
            break;
        case QProcess::WriteError:
            errorMsg = "Error writing to program.";
            break;
        case QProcess::ReadError:
            errorMsg = "Error reading from program.";
            break;
        default:
            errorMsg = "Unknown error occurred.";
            break;
    }
    
    appendStderr(QString("ERROR: %1\n").arg(errorMsg));
    emit programError(errorMsg);
}

void RunOutputWidget::appendStdout(const QString& text) {
    QTextCursor cursor = m_outputEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    Theme theme = ThemeManager::instance()->currentTheme();
    format.setForeground(theme.textPrimary);
    
    cursor.setCharFormat(format);
    cursor.insertText(text);
    
    m_outputEdit->setTextCursor(cursor);
    m_outputEdit->ensureCursorVisible();
}

void RunOutputWidget::appendStderr(const QString& text) {
    QTextCursor cursor = m_outputEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor("#F48771"));  // Light red
    
    cursor.setCharFormat(format);
    cursor.insertText(text);
    
    m_outputEdit->setTextCursor(cursor);
    m_outputEdit->ensureCursorVisible();
}

void RunOutputWidget::onThemeChanged() {
    Theme theme = ThemeManager::instance()->currentTheme();
    QPalette p = m_outputEdit->palette();
    p.setColor(QPalette::Base, theme.panelBackground);
    p.setColor(QPalette::Text, theme.textPrimary);
    m_outputEdit->setPalette(p);

    QTextCursor cursor(m_outputEdit->document());
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    format.setForeground(theme.textPrimary);
    cursor.setCharFormat(format);
}
