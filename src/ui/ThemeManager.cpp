#include "ui/ThemeManager.h"
#include <QApplication>

ThemeManager* ThemeManager::s_instance = nullptr;

ThemeManager::ThemeManager()
    : m_currentThemeName("dark")
{
    loadThemes();
}

ThemeManager* ThemeManager::instance() {
    if (!s_instance) {
        s_instance = new ThemeManager();
    }
    return s_instance;
}

void ThemeManager::loadThemes() {
    m_themes["dark"] = darkTheme();
    m_themes["light"] = lightTheme();
    m_themes["dracula"] = draculaTheme();
    m_themes["monokai"] = monokaiTheme();
}

QStringList ThemeManager::availableThemes() const {
    return m_themes.keys();
}

void ThemeManager::setTheme(const QString& themeName) {
    if (m_themes.contains(themeName)) {
        m_currentThemeName = themeName;
        QString stylesheet = generateStylesheet();
        qApp->setStyleSheet(stylesheet);
        emit themeChanged(themeName);
    }
}

Theme ThemeManager::currentTheme() const {
    return m_themes.value(m_currentThemeName, darkTheme());
}

QString ThemeManager::currentThemeName() const {
    return m_currentThemeName;
}

QString ThemeManager::generateStylesheet() const {
    Theme theme = currentTheme();
    
    QString qss;
    
    // QMainWindow
    qss += QString("QMainWindow { background-color: %1; color: %2; }\n")
        .arg(theme.windowBackground.name())
        .arg(theme.textPrimary.name());
    
    // QMenuBar
    qss += QString("QMenuBar { background-color: %1; color: %2; border-bottom: 1px solid %3; }\n")
        .arg(theme.toolbarBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QMenuBar::item { padding: 4px 8px; }\n");
    qss += QString("QMenuBar::item:selected { background-color: %1; }\n")
        .arg(theme.accent.name());
    
    // QMenu
    qss += QString("QMenu { background-color: %1; color: %2; border: 1px solid %3; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QMenu::item { padding: 5px 25px; }\n");
    qss += QString("QMenu::item:selected { background-color: %1; }\n")
        .arg(theme.accent.name());
    qss += QString("QMenu::separator { height: 1px; background: %1; margin: 5px 0px; }\n")
        .arg(theme.border.name());
    
    // QToolBar
    qss += QString("QToolBar { background-color: %1; border: none; spacing: 3px; }\n")
        .arg(theme.toolbarBackground.name());
    qss += QString("QToolButton { background-color: transparent; color: %1; border: none; padding: 5px; }\n")
        .arg(theme.textPrimary.name());
    qss += QString("QToolButton:hover { background-color: %1; }\n")
        .arg(theme.accent.name());
    qss += QString("QToolButton:pressed { background-color: %1; }\n")
        .arg(theme.accent.darker(120).name());
    
    // QTabWidget and QTabBar
    qss += QString("QTabWidget::pane { border: 1px solid %1; background-color: %2; }\n")
        .arg(theme.border.name())
        .arg(theme.windowBackground.name());
    qss += QString("QTabBar::tab { background-color: %1; color: %2; padding: 8px 16px; border: 1px solid %3; border-bottom: none; }\n")
        .arg(theme.tabInactive.name())
        .arg(theme.textSecondary.name())
        .arg(theme.border.name());
    qss += QString("QTabBar::tab:selected { background-color: %1; color: %2; }\n")
        .arg(theme.tabActive.name())
        .arg(theme.textPrimary.name());
    qss += QString("QTabBar::tab:hover:!selected { background-color: %1; }\n")
        .arg(theme.tabInactive.lighter(110).name());
    
    // QTabBar close button
    qss += QString("QTabBar::close-button { image: url(:/icons/close.svg); subcontrol-position: right; }\n");
    qss += QString("QTabBar::close-button:hover { background-color: %1; }\n")
        .arg(theme.error.name());
    
    // QDockWidget
    qss += QString("QDockWidget { titlebar-close-icon: url(:/icons/close.svg); titlebar-normal-icon: url(:/icons/maximize.svg); }\n");
    qss += QString("QDockWidget::title { background-color: %1; color: %2; padding: 5px; }\n")
        .arg(theme.sidebarBackground.name())
        .arg(theme.textPrimary.name());
    
    // QTreeView (FileTree)
    qss += QString("QTreeView { background-color: %1; color: %2; border: none; }\n")
        .arg(theme.sidebarBackground.name())
        .arg(theme.textPrimary.name());
    qss += QString("QTreeView::item { padding: 2px; }\n");
    qss += QString("QTreeView::item:hover { background-color: %1; }\n")
        .arg(theme.accent.lighter(150).name());
    qss += QString("QTreeView::item:selected { background-color: %1; color: %2; }\n")
        .arg(theme.accent.name())
        .arg(theme.textPrimary.name());
    qss += QString("QTreeView::branch { background-color: %1; }\n")
        .arg(theme.sidebarBackground.name());
    
    // QScrollBar
    qss += QString("QScrollBar:vertical { background-color: %1; width: 12px; }\n")
        .arg(theme.panelBackground.name());
    qss += QString("QScrollBar::handle:vertical { background-color: %1; min-height: 20px; border-radius: 6px; }\n")
        .arg(theme.textSecondary.name());
    qss += QString("QScrollBar::handle:vertical:hover { background-color: %1; }\n")
        .arg(theme.textPrimary.name());
    qss += QString("QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n");
    qss += QString("QScrollBar:horizontal { background-color: %1; height: 12px; }\n")
        .arg(theme.panelBackground.name());
    qss += QString("QScrollBar::handle:horizontal { background-color: %1; min-width: 20px; border-radius: 6px; }\n")
        .arg(theme.textSecondary.name());
    qss += QString("QScrollBar::handle:horizontal:hover { background-color: %1; }\n")
        .arg(theme.textPrimary.name());
    qss += QString("QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }\n");
    
    // QPushButton
    qss += QString("QPushButton { background-color: %1; color: %2; border: 1px solid %3; padding: 5px 15px; border-radius: 3px; }\n")
        .arg(theme.accent.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QPushButton:hover { background-color: %1; }\n")
        .arg(theme.accent.lighter(110).name());
    qss += QString("QPushButton:pressed { background-color: %1; }\n")
        .arg(theme.accent.darker(110).name());
    qss += QString("QPushButton:disabled { background-color: %1; color: %2; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textSecondary.name());
    
    // QLineEdit
    qss += QString("QLineEdit { background-color: %1; color: %2; border: 1px solid %3; padding: 4px; border-radius: 3px; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QLineEdit:focus { border: 1px solid %1; }\n")
        .arg(theme.accent.name());
    
    // QComboBox
    qss += QString("QComboBox { background-color: %1; color: %2; border: 1px solid %3; padding: 4px; border-radius: 3px; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QComboBox:hover { border: 1px solid %1; }\n")
        .arg(theme.accent.name());
    qss += QString("QComboBox::drop-down { border: none; }\n");
    qss += QString("QComboBox QAbstractItemView { background-color: %1; color: %2; selection-background-color: %3; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.accent.name());
    
    // QSplitter
    qss += QString("QSplitter::handle { background-color: %1; }\n")
        .arg(theme.border.name());
    qss += QString("QSplitter::handle:hover { background-color: %1; }\n")
        .arg(theme.accent.name());
    
    // QPlainTextEdit (output panels)
    qss += QString("QPlainTextEdit { background-color: %1; color: %2; border: none; font-family: monospace; }\n")
        .arg(theme.editorBackground.name())
        .arg(theme.editorForeground.name());
    
    // QTableView (problems)
    qss += QString("QTableView { background-color: %1; color: %2; border: none; gridline-color: %3; }\n")
        .arg(theme.panelBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    qss += QString("QTableView::item:selected { background-color: %1; }\n")
        .arg(theme.accent.name());
    qss += QString("QHeaderView::section { background-color: %1; color: %2; border: none; padding: 4px; }\n")
        .arg(theme.sidebarBackground.name())
        .arg(theme.textPrimary.name());
    
    // QStatusBar
    qss += QString("QStatusBar { background-color: %1; color: %2; border-top: 1px solid %3; }\n")
        .arg(theme.statusBarBackground.name())
        .arg(theme.textPrimary.name())
        .arg(theme.border.name());
    
    return qss;
}

Theme ThemeManager::darkTheme() {
    Theme theme;
    theme.name = "dark";
    theme.displayName = "Dark (VS Code Dark+)";
    
    // Editor colors (VS Code Dark+)
    theme.editorBackground = QColor("#1E1E1E");
    theme.editorForeground = QColor("#D4D4D4");
    theme.editorCurrentLine = QColor("#2A2A2A");
    theme.syntaxKeyword = QColor("#569CD6");       // Blue
    theme.syntaxType = QColor("#4EC9B0");          // Cyan
    theme.syntaxString = QColor("#CE9178");        // Orange
    theme.syntaxComment = QColor("#6A9955");       // Green
    theme.syntaxPreprocessor = QColor("#C586C0");  // Purple
    theme.syntaxNumber = QColor("#B5CEA8");        // Light green
    theme.syntaxFunction = QColor("#DCDCAA");      // Yellow
    
    // UI colors
    theme.windowBackground = QColor("#1E1E1E");
    theme.panelBackground = QColor("#252526");
    theme.toolbarBackground = QColor("#333333");
    theme.statusBarBackground = QColor("#007ACC");
    theme.sidebarBackground = QColor("#252526");
    theme.tabActive = QColor("#1E1E1E");
    theme.tabInactive = QColor("#2D2D2D");
    theme.border = QColor("#454545");
    theme.textPrimary = QColor("#D4D4D4");
    theme.textSecondary = QColor("#858585");
    theme.accent = QColor("#007ACC");
    theme.error = QColor("#F14C4C");
    theme.warning = QColor("#CCA700");
    theme.success = QColor("#89D185");
    
    // Fonts
    theme.editorFontFamily = "Monospace";
    theme.editorFontSize = 10;
    
    return theme;
}

Theme ThemeManager::lightTheme() {
    Theme theme;
    theme.name = "light";
    theme.displayName = "Light (VS Code Light+)";
    
    // Editor colors (VS Code Light+)
    theme.editorBackground = QColor("#FFFFFF");
    theme.editorForeground = QColor("#000000");
    theme.editorCurrentLine = QColor("#F3F3F3");
    theme.syntaxKeyword = QColor("#0000FF");       // Blue
    theme.syntaxType = QColor("#267F99");          // Teal
    theme.syntaxString = QColor("#A31515");        // Red
    theme.syntaxComment = QColor("#008000");       // Green
    theme.syntaxPreprocessor = QColor("#0000FF");  // Blue
    theme.syntaxNumber = QColor("#098658");        // Dark green
    theme.syntaxFunction = QColor("#795E26");      // Brown
    
    // UI colors
    theme.windowBackground = QColor("#FFFFFF");
    theme.panelBackground = QColor("#F3F3F3");
    theme.toolbarBackground = QColor("#F3F3F3");
    theme.statusBarBackground = QColor("#007ACC");
    theme.sidebarBackground = QColor("#F3F3F3");
    theme.tabActive = QColor("#FFFFFF");
    theme.tabInactive = QColor("#ECECEC");
    theme.border = QColor("#CCCCCC");
    theme.textPrimary = QColor("#000000");
    theme.textSecondary = QColor("#6A6A6A");
    theme.accent = QColor("#007ACC");
    theme.error = QColor("#E51400");
    theme.warning = QColor("#BF8803");
    theme.success = QColor("#388A34");
    
    // Fonts
    theme.editorFontFamily = "Monospace";
    theme.editorFontSize = 10;
    
    return theme;
}

Theme ThemeManager::draculaTheme() {
    Theme theme;
    theme.name = "dracula";
    theme.displayName = "Dracula";
    
    // Editor colors (Dracula)
    theme.editorBackground = QColor("#282A36");
    theme.editorForeground = QColor("#F8F8F2");
    theme.editorCurrentLine = QColor("#44475A");
    theme.syntaxKeyword = QColor("#FF79C6");       // Pink
    theme.syntaxType = QColor("#8BE9FD");          // Cyan
    theme.syntaxString = QColor("#F1FA8C");        // Yellow
    theme.syntaxComment = QColor("#6272A4");       // Blue gray
    theme.syntaxPreprocessor = QColor("#FF79C6");  // Pink
    theme.syntaxNumber = QColor("#BD93F9");        // Purple
    theme.syntaxFunction = QColor("#50FA7B");      // Green
    
    // UI colors
    theme.windowBackground = QColor("#282A36");
    theme.panelBackground = QColor("#21222C");
    theme.toolbarBackground = QColor("#191A21");
    theme.statusBarBackground = QColor("#BD93F9");
    theme.sidebarBackground = QColor("#21222C");
    theme.tabActive = QColor("#282A36");
    theme.tabInactive = QColor("#21222C");
    theme.border = QColor("#44475A");
    theme.textPrimary = QColor("#F8F8F2");
    theme.textSecondary = QColor("#6272A4");
    theme.accent = QColor("#BD93F9");
    theme.error = QColor("#FF5555");
    theme.warning = QColor("#FFB86C");
    theme.success = QColor("#50FA7B");
    
    // Fonts
    theme.editorFontFamily = "Monospace";
    theme.editorFontSize = 10;
    
    return theme;
}

Theme ThemeManager::monokaiTheme() {
    Theme theme;
    theme.name = "monokai";
    theme.displayName = "Monokai";
    
    // Editor colors (Monokai)
    theme.editorBackground = QColor("#272822");
    theme.editorForeground = QColor("#F8F8F2");
    theme.editorCurrentLine = QColor("#3E3D32");
    theme.syntaxKeyword = QColor("#F92672");       // Pink
    theme.syntaxType = QColor("#66D9EF");          // Cyan
    theme.syntaxString = QColor("#E6DB74");        // Yellow
    theme.syntaxComment = QColor("#75715E");       // Gray
    theme.syntaxPreprocessor = QColor("#F92672");  // Pink
    theme.syntaxNumber = QColor("#AE81FF");        // Purple
    theme.syntaxFunction = QColor("#A6E22E");      // Green
    
    // UI colors
    theme.windowBackground = QColor("#272822");
    theme.panelBackground = QColor("#1E1F1C");
    theme.toolbarBackground = QColor("#1E1F1C");
    theme.statusBarBackground = QColor("#75715E");
    theme.sidebarBackground = QColor("#1E1F1C");
    theme.tabActive = QColor("#272822");
    theme.tabInactive = QColor("#1E1F1C");
    theme.border = QColor("#3E3D32");
    theme.textPrimary = QColor("#F8F8F2");
    theme.textSecondary = QColor("#75715E");
    theme.accent = QColor("#66D9EF");
    theme.error = QColor("#F92672");
    theme.warning = QColor("#FD971F");
    theme.success = QColor("#A6E22E");
    
    // Fonts
    theme.editorFontFamily = "Monospace";
    theme.editorFontSize = 10;
    
    return theme;
}
