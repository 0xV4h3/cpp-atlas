#include "output/ProblemsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>

ProblemsWidget::ProblemsWidget(QWidget *parent) : QWidget(parent) { setupUi(); }

void ProblemsWidget::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* toolbar = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);

    QLabel* filterLabel = new QLabel("Filter:", toolbar);
    m_filterCombo = new QComboBox(toolbar);
    m_filterCombo->addItem("All",           All);
    m_filterCombo->addItem("Errors Only",   ErrorsOnly);
    m_filterCombo->addItem("Warnings Only", WarningsOnly);

    m_clearButton = new QPushButton("Clear", toolbar);

    toolbarLayout->addWidget(filterLabel);
    toolbarLayout->addWidget(m_filterCombo);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_clearButton);

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

    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProblemsWidget::onFilterChanged);
    connect(m_clearButton, &QPushButton::clicked, this, &ProblemsWidget::clear);
    connect(m_tableWidget, &QTableWidget::cellClicked, this, &ProblemsWidget::onCellClicked);
}

void ProblemsWidget::setDiagnostics(const QList<DiagnosticMessage>& diagnostics)
{
    m_diagnostics = diagnostics;
    updateTable();
}

void ProblemsWidget::clear() { m_diagnostics.clear(); updateTable(); }

int ProblemsWidget::errorCount() const
{
    int count = 0;
    for (const auto& d : m_diagnostics)
        if (d.severity == DiagnosticMessage::Error) ++count;
    return count;
}

int ProblemsWidget::warningCount() const
{
    int count = 0;
    for (const auto& d : m_diagnostics)
        if (d.severity == DiagnosticMessage::Warning) ++count;
    return count;
}

void ProblemsWidget::onFilterChanged(int index)
{
    m_filterMode = static_cast<FilterMode>(m_filterCombo->itemData(index).toInt());
    updateTable();
}

void ProblemsWidget::onCellClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0 || row >= m_tableWidget->rowCount()) return;

    QTableWidgetItem* fileItem = m_tableWidget->item(row, 3);
    QTableWidgetItem* posItem  = m_tableWidget->item(row, 4);
    if (!fileItem || !posItem) return;

    const QString file = fileItem->toolTip().isEmpty()
                             ? fileItem->text()
                             : fileItem->toolTip();

    const QStringList parts = posItem->text().split(':');
    if (parts.size() == 2) {
        const int line = parts[0].toInt();
        const int col  = parts[1].toInt();
        emit diagnosticClicked(file, line, col);
    }
}

void ProblemsWidget::updateTable()
{
    m_tableWidget->setRowCount(0);

    QList<DiagnosticMessage> filtered;
    for (const auto& d : m_diagnostics) {
        if (m_filterMode == All ||
            (m_filterMode == ErrorsOnly   && d.severity == DiagnosticMessage::Error) ||
            (m_filterMode == WarningsOnly && d.severity == DiagnosticMessage::Warning))
            filtered.append(d);
    }

    m_tableWidget->setRowCount(filtered.size());
    for (int i = 0; i < filtered.size(); ++i) {
        const DiagnosticMessage& d = filtered[i];

        QTableWidgetItem* iconItem = new QTableWidgetItem(severityIcon(d.severity));
        iconItem->setForeground(severityColor(d.severity));
        iconItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(i, 0, iconItem);

        QTableWidgetItem* sevItem = new QTableWidgetItem(severityText(d.severity));
        sevItem->setForeground(severityColor(d.severity));
        m_tableWidget->setItem(i, 1, sevItem);

        m_tableWidget->setItem(i, 2, new QTableWidgetItem(d.message));

        QTableWidgetItem* fileItem = new QTableWidgetItem(QFileInfo(d.file).fileName());
        fileItem->setToolTip(d.file);
        m_tableWidget->setItem(i, 3, fileItem);

        m_tableWidget->setItem(i, 4,
                               new QTableWidgetItem(QString("%1:%2").arg(d.line).arg(d.column)));
    }
}

QString ProblemsWidget::severityIcon(DiagnosticMessage::Severity s) const
{
    switch (s) {
    case DiagnosticMessage::Error:   return "\xe2\x9c\x96";
    case DiagnosticMessage::Warning: return "\xe2\x9a\xa0";
    case DiagnosticMessage::Note:    return "\xe2\x84\xb9";
    }
    return "";
}

QString ProblemsWidget::severityText(DiagnosticMessage::Severity s) const
{
    switch (s) {
    case DiagnosticMessage::Error:   return "Error";
    case DiagnosticMessage::Warning: return "Warning";
    case DiagnosticMessage::Note:    return "Note";
    }
    return "";
}

QColor ProblemsWidget::severityColor(DiagnosticMessage::Severity s) const
{
    switch (s) {
    case DiagnosticMessage::Error:   return QColor("#F44747");
    case DiagnosticMessage::Warning: return QColor("#CCA700");
    case DiagnosticMessage::Note:    return QColor("#75BEFF");
    }
    return QColor("#D4D4D4");
}
