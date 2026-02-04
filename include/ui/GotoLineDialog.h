#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

/**
 * @brief Dialog for navigating to a specific line number
 */
class GotoLineDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit GotoLineDialog(int maxLine, QWidget *parent = nullptr);
    ~GotoLineDialog() override = default;
    
    /**
     * @brief Get the selected line number
     * @return Line number (1-based)
     */
    int lineNumber() const;
    
private:
    QSpinBox* m_lineSpinBox;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    
    void setupUi();
};

#endif // GOTOLINEDIALOG_H
