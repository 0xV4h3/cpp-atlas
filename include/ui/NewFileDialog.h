#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

/**
 * @brief Dialog for creating new files with template selection
 */
class NewFileDialog : public QDialog {
    Q_OBJECT

public:
    enum FileTemplate {
        CppSource,
        CppHeader,
        CHeader,
        CppClass,
        CSource,
        EmptyFile
    };

    explicit NewFileDialog(const QString& defaultDirectory, QWidget *parent = nullptr);
    ~NewFileDialog() override = default;

    /**
     * @brief Get the list of created file paths
     * @return List of created file paths
     */
    QStringList createdFiles() const;

private slots:
    void onTemplateSelectionChanged();
    void onBrowseClicked();
    void onCreateClicked();

private:
    void setupUI();
    void populateTemplates();
    QString loadTemplate(const QString& resourcePath) const;
    QString generateHeaderGuard(const QString& fileName) const;
    bool createFileFromTemplate(const QString& filePath, const QString& templateContent,
                                const QString& className = QString()) const;

    QListWidget* m_templateList;
    QLabel* m_descriptionLabel;
    QLineEdit* m_nameEdit;
    QLineEdit* m_locationEdit;
    QPushButton* m_browseBtn;
    QPushButton* m_createBtn;
    QPushButton* m_cancelBtn;

    QString m_defaultDirectory;
    QStringList m_createdFiles;
};

#endif // NEWFILEDIALOG_H
