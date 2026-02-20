#include "editor/EditorTabWidget.h"
#include "editor/CodeEditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

EditorTabWidget::EditorTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    
    connect(this, &QTabWidget::currentChanged, this, &EditorTabWidget::onTabChanged);
    connect(this, &QTabWidget::tabCloseRequested, this, &EditorTabWidget::onTabCloseRequested);
}

CodeEditor* EditorTabWidget::newFile() {
    CodeEditor* editor = new CodeEditor(this);
    QString title = QString("Untitled-%1.cpp").arg(m_newFileCounter++);
    
    connect(editor, &CodeEditor::modificationChanged, 
            this, &EditorTabWidget::onEditorModificationChanged);
    
    int index = addTab(editor, title);
    setCurrentIndex(index);
    
    return editor;
}

CodeEditor* EditorTabWidget::openFile(const QString& filePath) {
    // Check if file is already open
    for (int i = 0; i < count(); ++i) {
        CodeEditor* editor = editorAt(i);
        if (editor && editor->filePath() == filePath) {
            setCurrentIndex(i);
            return editor;
        }
    }
    
    // Open new file
    CodeEditor* editor = new CodeEditor(this);
    if (!editor->loadFile(filePath)) {
        delete editor;
        return nullptr;
    }
    
    connect(editor, &CodeEditor::modificationChanged, 
            this, &EditorTabWidget::onEditorModificationChanged);
    
    QString fileName = shortFileName(filePath);
    int index = addTab(editor, fileName);
    setCurrentIndex(index);
    
    emit fileOpened(filePath);
    
    return editor;
}

bool EditorTabWidget::closeTab(int index) {
    CodeEditor* editor = editorAt(index);
    if (!editor) {
        return false;
    }
    
    if (editor->isModified()) {
        QString fileName = tabText(index).remove('*').trimmed();
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Save Changes?",
            QString("Do you want to save changes to %1?").arg(fileName),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );
        
        if (reply == QMessageBox::Save) {
            // Save file first
            if (editor->filePath().isEmpty()) {
                QString filePath = QFileDialog::getSaveFileName(
                    this,
                    "Save File",
                    QString(),
                    "C++ Files (*.cpp *.h *.hpp *.cc *.cxx);;All Files (*)"
                );
                
                if (filePath.isEmpty()) {
                    return false;  // User cancelled save
                }
                
                if (!editor->saveFile(filePath)) {
                    QMessageBox::warning(this, "Error", "Failed to save file.");
                    return false;
                }
            } else {
                if (!editor->saveFile(editor->filePath())) {
                    QMessageBox::warning(this, "Error", "Failed to save file.");
                    return false;
                }
            }
        } else if (reply == QMessageBox::Cancel) {
            return false;  // User cancelled close
        }
    }
    
    removeTab(index);
    editor->deleteLater();
    
    return true;
}

CodeEditor* EditorTabWidget::currentEditor() const {
    return qobject_cast<CodeEditor*>(currentWidget());
}

CodeEditor* EditorTabWidget::editorAt(int index) const {
    return qobject_cast<CodeEditor*>(widget(index));
}

bool EditorTabWidget::saveCurrentFile() {
    CodeEditor* editor = currentEditor();
    if (!editor) {
        return false;
    }
    
    if (editor->filePath().isEmpty()) {
        return saveCurrentFileAs();
    }
    
    if (editor->saveFile(editor->filePath())) {
        updateTabTitle(editor);
        emit fileSaved(editor->filePath());
        return true;
    }
    
    return false;
}

bool EditorTabWidget::saveCurrentFileAs() {
    CodeEditor* editor = currentEditor();
    if (!editor) {
        return false;
    }
    
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save File As",
        editor->filePath(),
        "C++ Files (*.cpp *.h *.hpp *.cc *.cxx);;All Files (*)"
    );
    
    if (filePath.isEmpty()) {
        return false;
    }
    
    if (editor->saveFile(filePath)) {
        updateTabTitle(editor);
        emit fileSaved(filePath);
        return true;
    }
    
    return false;
}

bool EditorTabWidget::closeAll() {
    while (count() > 0) {
        if (!closeTab(0)) {
            return false;  // User cancelled
        }
    }
    return true;
}

void EditorTabWidget::closeFileTab(const QString& filePath) {
    for (int i = 0; i < count(); ++i) {
        CodeEditor* editor = editorAt(i);
        if (editor && editor->filePath() == filePath) {
            removeTab(i);
            editor->deleteLater();
            return;
        }
    }
}

void EditorTabWidget::updateFilePath(const QString& oldPath, const QString& newPath) {
    for (int i = 0; i < count(); ++i) {
        CodeEditor* editor = editorAt(i);
        if (editor && editor->filePath() == oldPath) {
            editor->setFilePath(newPath);
            setTabText(i, shortFileName(newPath));
            return;
        }
    }
}

void EditorTabWidget::onTabChanged(int index) {
    CodeEditor* editor = editorAt(index);
    emit editorChanged(editor);
}

void EditorTabWidget::onTabCloseRequested(int index) {
    closeTab(index);
}

void EditorTabWidget::onEditorModificationChanged(bool modified) {
    CodeEditor* editor = qobject_cast<CodeEditor*>(sender());
    if (editor) {
        updateTabTitle(editor);
    }
}

void EditorTabWidget::updateTabTitle(CodeEditor* editor) {
    int index = indexOf(editor);
    if (index < 0) {
        return;
    }
    
    QString title;
    if (editor->filePath().isEmpty()) {
        title = QString("Untitled-%1.cpp").arg(index + 1);
    } else {
        title = shortFileName(editor->filePath());
    }
    
    if (editor->isModified()) {
        title += " *";
    }
    
    setTabText(index, title);
}

QString EditorTabWidget::shortFileName(const QString& filePath) const {
    return QFileInfo(filePath).fileName();
}
