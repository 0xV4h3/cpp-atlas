#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>
#include <QSettings>

/**
 * @brief Per-user QSettings wrapper for CppAtlas.
 *
 * All per-user UI preferences are stored under the group:
 *   "Users/{username}/"
 *
 * Application-level (non-user) settings remain at root level.
 *
 * Usage:
 * @code
 *   AppSettings s("alice");
 *   s.setTheme("dracula");
 *   s.setEditorFontSize(12);
 *   QString theme = s.theme(); // "dracula"
 * @endcode
 */
class AppSettings
{
public:
    /**
     * @brief Construct settings for a specific user.
     * @param username The username. If empty, operates at root level.
     */
    explicit AppSettings(const QString& username = QString());

    // ── UI Preferences ───────────────────────────────────────────────────────
    QString theme() const;
    void    setTheme(const QString& theme);

    int  editorFontSize() const;
    void setEditorFontSize(int size);

    QString editorFontFamily() const;
    void    setEditorFontFamily(const QString& family);

    bool showLineNumbers() const;
    void setShowLineNumbers(bool show);

    bool wordWrap() const;
    void setWordWrap(bool wrap);

    // ── Session (per-user) ───────────────────────────────────────────────────
    QString lastOpenedProject() const;
    void    setLastOpenedProject(const QString& path);

    // ── Generic access (for future extensibility) ────────────────────────────
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void     setValue(const QString& key, const QVariant& value);

    // ── App-level (not per-user) ─────────────────────────────────────────────
    static QString lastLoggedInUser();
    static void    setLastLoggedInUser(const QString& username);

private:
    QSettings m_settings;  ///< Scoped to "Users/{username}/" group when username given
    QString   m_username;

    QString key(const QString& k) const;
};

#endif // APPSETTINGS_H