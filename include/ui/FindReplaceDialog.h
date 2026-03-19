#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

class FindReplaceDialog : public QDialog
{
    Q_OBJECT
public:
    enum Mode { Find, Replace };

    explicit FindReplaceDialog(Mode mode = Find, QWidget* parent = nullptr);

    QString findText()      const;
    QString replaceText()   const;
    bool    caseSensitive() const;
    bool    wholeWord()     const;
    bool    useRegex()      const;

signals:
    void findNext();
    void findPrevious();
    void replaceNext();
    void replaceAll();

protected:
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void toggleMode();
    void applyTheme();

private:
    void setupUi(Mode initialMode);
    void setReplaceVisible(bool visible);

    QLineEdit*   m_findEdit         = nullptr;
    QLineEdit*   m_replaceEdit      = nullptr;
    QWidget*     m_replaceRow       = nullptr;
    QCheckBox*   m_caseSensitive    = nullptr;
    QCheckBox*   m_wholeWord        = nullptr;
    QCheckBox*   m_regexCheck       = nullptr;
    QPushButton* m_toggleModeBtn    = nullptr;
    QPushButton* m_findNextBtn      = nullptr;
    QPushButton* m_findPrevBtn      = nullptr;
    QPushButton* m_replaceBtn       = nullptr;
    QPushButton* m_replaceAllBtn    = nullptr;
    QPushButton* m_closeBtn         = nullptr;
    bool         m_replaceVisible   = false;
};
