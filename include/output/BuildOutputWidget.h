#ifndef BUILDOUTPUTWIDGET_H
#define BUILDOUTPUTWIDGET_H

#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief Widget for displaying compiler build output
 */
class BuildOutputWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit BuildOutputWidget(QWidget *parent = nullptr);
    ~BuildOutputWidget() override = default;
    
    /**
     * @brief Append text to build output
     * @param text Text to append
     */
    void appendText(const QString& text);
    
    /**
     * @brief Append error text (colored red)
     * @param text Error text
     */
    void appendError(const QString& text);
    
    /**
     * @brief Append warning text (colored yellow)
     * @param text Warning text
     */
    void appendWarning(const QString& text);
    
    /**
     * @brief Clear build output
     */
    void clear();
    
    /**
     * @brief Get all output text
     * @return Output text
     */
    QString text() const;
    
signals:
    void errorClicked(const QString& file, int line, int column);
    
private slots:
    void onClearClicked();
    void onCopyClicked();
    void onThemeChanged();
    
private:
    QPlainTextEdit* m_textEdit;
    QPushButton* m_clearButton;
    QPushButton* m_copyButton;
    
    void setupUi();
    void parseAndHighlightLine(const QString& line);
};

#endif // BUILDOUTPUTWIDGET_H
