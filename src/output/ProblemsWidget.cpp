#include "output/ProblemsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>

ProblemsWidget::ProblemsWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void ProblemsWidget::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Toolbar
    QWidget* toolbar = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    
    QLabel* filterLabel = new QLabel("Filter:", toolbar);
    m_filterCombo = new QComboBox(toolbar);
    m_filterCombo->addItem("All", All);
    m_filterCombo->addItem("Errors Only", ErrorsOnly);
    m_filterCombo->addItem("Warnings Only", WarningsOnly);
    
    m_clearButton = new QPushButton("Clear", toolbar);
    
    toolbarLayout->addWidget(filterLabel);
    toolbarLayout->addWidget(m_filterCombo);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_clearButton);
    
    // Table
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(5);
    m_tableWidget->setHorizontalHeaderLabels({"", "Severity", "Message", "File", "Line:Col"});
    m_tableWidget->horizontalHeader()->setStretchLastSection(false);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_tableWidget->setColumnWidth(0, 30);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setAlternatingRowColors(true);
    
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(m_tableWidget);
    
    // Connect signals
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProblemsWidget::onFilterChanged);
    connect(m_clearButton, &QPushButton::clicked, this, &ProblemsWidget::clear);
    connect(m_tableWidget, &QTableWidget::cellClicked, this, &ProblemsWidget::onCellClicked);
}

void ProblemsWidget::setDiagnostics(const QList<DiagnosticMessage>& diagnostics) {
    m_diagnostics = diagnostics;
    updateTable();
}

void ProblemsWidget::clear() {
    m_diagnostics.clear();
    updateTable();
}

int ProblemsWidget::errorCount() const {
    int count = 0;
    for (const auto& diag : m_diagnostics) {
        if (diag.severity == DiagnosticMessage::Error) {
            count++;
        }
    }
    return count;
}

int ProblemsWidget::warningCount() const {
    int count = 0;
    for (const auto& diag : m_diagnostics) {
        if (diag.severity == DiagnosticMessage::Warning) {
            count++;
        }
    }
    return count;
}

void ProblemsWidget::onFilterChanged(int index) {
    m_filterMode = static_cast<FilterMode>(m_filterCombo->itemData(index).toInt());
    updateTable();
}

void ProblemsWidget::onCellClicked(int row, int column) {
    Q_UNUSED(column);
    
    if (row < 0 || row >= m_tableWidget->rowCount()) {
        return;
    }
    
    // Get file, line, column from the row's data
    QTableWidgetItem* fileItem = m_tableWidget->item(row, 3);
    QTableWidgetItem* posItem = m_tableWidget->item(row, 4);
    
    if (fileItem && posItem) {
        QString file = fileItem->text();
        QString posText = posItem->text();  // Format: "line:col"
        
        QStringList parts = posText.split(':');
        if (parts.size() == 2) {
            int line = parts[0].toInt();
            int column = parts[1].toInt();
            emit diagnosticClicked(file, line, column);
        }
    }
}

void ProblemsWidget::updateTable() {
    m_tableWidget->setRowCount(0);
    
    // Filter diagnostics
    QList<DiagnosticMessage> filtered;
    for (const auto& diag : m_diagnostics) {
        if (m_filterMode == All ||
            (m_filterMode == ErrorsOnly && diag.severity == DiagnosticMessage::Error) ||
            (m_filterMode == WarningsOnly && diag.severity == DiagnosticMessage::Warning)) {
            filtered.append(diag);
        }
    }
    
    // Populate table
    m_tableWidget->setRowCount(filtered.size());
    
    for (int i = 0; i < filtered.size(); ++i) {
        const DiagnosticMessage& diag = filtered[i];
        
        // Icon column
        QTableWidgetItem* iconItem = new QTableWidgetItem(severityIcon(diag.severity));
        iconItem->setForeground(severityColor(diag.severity));
        iconItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 0, iconItem);
        
        // Severity column
        QTableWidgetItem* severityItem = new QTableWidgetItem(severityText(diag.severity));
        severityItem->setForeground(severityColor(diag.severity));
        m_tableWidget->setItem(i, 1, severityItem);
        
        // Message column
        QTableWidgetItem* messageItem = new QTableWidgetItem(diag.message);
        m_tableWidget->setItem(i, 2, messageItem);
        
        // File column
        QString fileName = QFileInfo(diag.file).fileName();
        QTableWidgetItem* fileItem = new QTableWidgetItem(fileName);
        fileItem->setToolTip(diag.file);  // Full path in tooltip
        m_tableWidget->setItem(i, 3, fileItem);
        
        // Line:Column column
        QString posText = QString("%1:%2").arg(diag.line).arg(diag.column);
        QTableWidgetItem* posItem = new QTableWidgetItem(posText);
        m_tableWidget->setItem(i, 4, posItem);
    }
}

QString ProblemsWidget::severityIcon(DiagnosticMessage::Severity severity) const {
    switch (severity) {
        case DiagnosticMessage::Error:   return "✖";
        case DiagnosticMessage::Warning: return "⚠";
        case DiagnosticMessage::Note:    return "ℹ";
    }
    return "";
}

QString ProblemsWidget::severityText(DiagnosticMessage::Severity severity) const {
    switch (severity) {
        case DiagnosticMessage::Error:   return "Error";
        case DiagnosticMessage::Warning: return "Warning";
        case DiagnosticMessage::Note:    return "Note";
    }
    return "";
}

QColor ProblemsWidget::severityColor(DiagnosticMessage::Severity severity) const {
    switch (severity) {
        case DiagnosticMessage::Error:   return QColor("#F44747");  // Red
        case DiagnosticMessage::Warning: return QColor("#CCA700");  // Yellow
        case DiagnosticMessage::Note:    return QColor("#75BEFF");  // Blue
    }
    return QColor("#D4D4D4");  // Default
}
