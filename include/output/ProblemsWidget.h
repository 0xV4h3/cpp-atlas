#ifndef PROBLEMSWIDGET_H
#define PROBLEMSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include "compiler/CompileResult.h"

/**
 * @brief Widget for displaying compiler diagnostics in a table
 */
class ProblemsWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit ProblemsWidget(QWidget *parent = nullptr);
    ~ProblemsWidget() override = default;
    
    /**
     * @brief Set diagnostics to display
     * @param diagnostics List of diagnostic messages
     */
    void setDiagnostics(const QList<DiagnosticMessage>& diagnostics);
    
    /**
     * @brief Clear all diagnostics
     */
    void clear();
    
    /**
     * @brief Get count of errors
     * @return Number of errors
     */
    int errorCount() const;
    
    /**
     * @brief Get count of warnings
     * @return Number of warnings
     */
    int warningCount() const;
    
signals:
    void diagnosticClicked(const QString& file, int line, int column);
    
private slots:
    void onFilterChanged(int index);
    void onCellClicked(int row, int column);
    
private:
    QTableWidget* m_tableWidget;
    QComboBox* m_filterCombo;
    QPushButton* m_clearButton;
    QList<DiagnosticMessage> m_diagnostics;
    
    enum FilterMode {
        All,
        ErrorsOnly,
        WarningsOnly
    };
    
    FilterMode m_filterMode = All;
    
    void setupUi();
    void updateTable();
    QString severityIcon(DiagnosticMessage::Severity severity) const;
    QString severityText(DiagnosticMessage::Severity severity) const;
    QColor severityColor(DiagnosticMessage::Severity severity) const;
};

#endif // PROBLEMSWIDGET_H
