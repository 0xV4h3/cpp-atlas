#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QString>
#include <QMap>

/**
 * @brief Theme configuration structure
 */
struct Theme {
    QString name;
    QString displayName;
    
    // Editor colors
    QColor editorBackground;
    QColor editorForeground;
    QColor editorCurrentLine;
    QColor syntaxKeyword;
    QColor syntaxType;
    QColor syntaxString;
    QColor syntaxComment;
    QColor syntaxPreprocessor;
    QColor syntaxNumber;
    QColor syntaxFunction;
    QColor cursorColor;
    
    // UI colors
    QColor windowBackground;
    QColor panelBackground;
    QColor toolbarBackground;
    QColor statusBarBackground;
    QColor sidebarBackground;
    QColor tabActive;
    QColor tabInactive;
    QColor border;
    QColor textPrimary;
    QColor textSecondary;
    QColor accent;
    QColor error;
    QColor warning;
    QColor success;
    
    // Fonts
    QString editorFontFamily;
    int editorFontSize;
};

/**
 * @brief Theme manager singleton
 */
class ThemeManager : public QObject {
    Q_OBJECT
    
public:
    static ThemeManager* instance();
    
    /**
     * @brief Get list of available theme names
     * @return List of theme names
     */
    QStringList availableThemes() const;
    
    /**
     * @brief Set current theme
     * @param themeName Theme name
     */
    void setTheme(const QString& themeName);
    
    /**
     * @brief Get current theme
     * @return Current theme
     */
    Theme currentTheme() const;
    
    /**
     * @brief Get current theme name
     * @return Theme name
     */
    QString currentThemeName() const;
    
    /**
     * @brief Generate QSS stylesheet from current theme
     * @return Stylesheet string
     */
    QString generateStylesheet() const;
    
    /**
     * @brief Get Dark theme
     * @return Dark theme
     */
    static Theme darkTheme();
    
    /**
     * @brief Get Light theme
     * @return Light theme
     */
    static Theme lightTheme();
    
    /**
     * @brief Get Dracula theme
     * @return Dracula theme
     */
    static Theme draculaTheme();
    
    /**
     * @brief Get Monokai theme
     * @return Monokai theme
     */
    static Theme monokaiTheme();
    
signals:
    void themeChanged(const QString& themeName);
    
private:
    ThemeManager();
    ~ThemeManager() override = default;
    
    static ThemeManager* s_instance;
    QString m_currentThemeName;
    QMap<QString, Theme> m_themes;
    
    void loadThemes();
};

#endif // THEMEMANAGER_H
