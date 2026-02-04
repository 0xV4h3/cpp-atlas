#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

/**
 * @brief Dialog for find and replace functionality
 */
class FindReplaceDialog : public QDialog {
    Q_OBJECT
    
public:
    enum Mode {
        Find,
        Replace
    };
    
    explicit FindReplaceDialog(Mode mode, QWidget *parent = nullptr);
    ~FindReplaceDialog() override = default;
    
    /**
     * @brief Get search text
     * @return Search text
     */
    QString findText() const;
    
    /**
     * @brief Get replacement text
     * @return Replacement text
     */
    QString replaceText() const;
    
    /**
     * @brief Check if case sensitive search is enabled
     * @return true if case sensitive
     */
    bool caseSensitive() const;
    
    /**
     * @brief Check if whole word search is enabled
     * @return true if whole word
     */
    bool wholeWord() const;
    
    /**
     * @brief Check if regex search is enabled
     * @return true if regex
     */
    bool useRegex() const;
    
signals:
    void findNext();
    void findPrevious();
    void replaceNext();
    void replaceAll();
    
private:
    Mode m_mode;
    QLineEdit* m_findLineEdit;
    QLineEdit* m_replaceLineEdit;
    QCheckBox* m_caseSensitiveCheckBox;
    QCheckBox* m_wholeWordCheckBox;
    QCheckBox* m_regexCheckBox;
    QPushButton* m_findNextButton;
    QPushButton* m_findPrevButton;
    QPushButton* m_replaceButton;
    QPushButton* m_replaceAllButton;
    QPushButton* m_closeButton;
    
    void setupUi();
};

#endif // FINDREPLACEDIALOG_H
