#ifndef RUNOUTPUTWIDGET_H
#define RUNOUTPUTWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QElapsedTimer>

/**
 * @brief Widget for displaying program execution output
 */
class RunOutputWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit RunOutputWidget(QWidget *parent = nullptr);
    ~RunOutputWidget() override = default;
    
    /**
     * @brief Start running a program
     * @param program Program path
     * @param arguments Command-line arguments
     */
    void runProgram(const QString& program, const QStringList& arguments = QStringList());
    
    /**
     * @brief Stop the currently running program
     */
    void stopProgram();
    
    /**
     * @brief Check if a program is currently running
     * @return true if program is running
     */
    bool isRunning() const;
    
    /**
     * @brief Clear output
     */
    void clear();
    
    /**
     * @brief Set command-line arguments
     * @param args Arguments string
     */
    void setArguments(const QString& args);
    
    /**
     * @brief Get command-line arguments
     * @return Arguments string
     */
    QString arguments() const;
    
signals:
    void programStarted();
    void programFinished(int exitCode);
    void programError(const QString& error);
    
private slots:
    void onStopClicked();
    void onClearClicked();
    void onInputReturnPressed();
    void onProcessReadyReadStdout();
    void onProcessReadyReadStderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    
private:
    QPlainTextEdit* m_outputEdit;
    QLineEdit* m_inputEdit;
    QLineEdit* m_argsEdit;
    QPushButton* m_stopButton;
    QPushButton* m_clearButton;
    QProcess* m_process;
    QElapsedTimer m_timer;
    
    void setupUi();
    void appendStdout(const QString& text);
    void appendStderr(const QString& text);
    void updateElapsedTime();
};

#endif // RUNOUTPUTWIDGET_H
