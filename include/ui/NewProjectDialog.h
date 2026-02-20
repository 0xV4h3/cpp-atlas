#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class NewProjectDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);
    ~NewProjectDialog() override = default;
    
    QString projectName() const;
    QString projectLocation() const;
    QString projectFilePath() const;
    bool createProjectFolder() const;
    bool createMainCpp() const;
    bool createCMakeLists() const;
    QString selectedCompilerId() const;
    QString selectedStandard() const;

private slots:
    void onBrowseLocation();
    void onCreateProject();
    void validateInput();

private:
    void setupUI();
    void loadCompilers();
    
    QLineEdit* m_nameEdit;
    QLineEdit* m_locationEdit;
    QPushButton* m_browseBtn;
    QCheckBox* m_createFolderCheck;
    QComboBox* m_compilerCombo;
    QComboBox* m_standardCombo;
    QCheckBox* m_createMainCheck;
    QCheckBox* m_createCMakeCheck;
    QPushButton* m_createBtn;
    QPushButton* m_cancelBtn;
    QLabel* m_folderPreview;
    
    QString m_projectFilePath;
};

#endif // NEWPROJECTDIALOG_H
