#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @brief Welcome screen widget shown on startup
 */
class WelcomeWidget : public QWidget {
    Q_OBJECT
public:
    explicit WelcomeWidget(QWidget* parent = nullptr);
    
    /**
     * @brief Update recent projects list
     * @param projects List of recent project paths
     */
    void updateRecentProjects(const QStringList& projects);

signals:
    void newFileRequested();
    void openFolderRequested();
    void openFileRequested();
    void recentProjectClicked(const QString& path);

private:
    void setupUi();
    
    QLabel* m_titleLabel;
    QListWidget* m_recentList;
    QPushButton* m_newFileBtn;
    QPushButton* m_openFolderBtn;
    QPushButton* m_openFileBtn;
};

#endif // WELCOMEWIDGET_H
