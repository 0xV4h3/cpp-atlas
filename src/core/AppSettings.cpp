#include "core/AppSettings.h"

AppSettings::AppSettings(const QString& username)
    : m_settings("CppAtlas", "CppAtlas")
    , m_username(username)
{
}

// ─────────────────────────────────────────────────────────────────────────────
// UI Preferences
// ─────────────────────────────────────────────────────────────────────────────

QString AppSettings::theme() const
{
    return m_settings.value(key("ui/theme"), "dark").toString();
}

void AppSettings::setTheme(const QString& theme)
{
    m_settings.setValue(key("ui/theme"), theme);
}

int AppSettings::editorFontSize() const
{
    return m_settings.value(key("editor/fontSize"), 10).toInt();
}

void AppSettings::setEditorFontSize(int size)
{
    m_settings.setValue(key("editor/fontSize"), size);
}

QString AppSettings::editorFontFamily() const
{
    return m_settings.value(key("editor/fontFamily"), "Monospace").toString();
}

void AppSettings::setEditorFontFamily(const QString& family)
{
    m_settings.setValue(key("editor/fontFamily"), family);
}

bool AppSettings::showLineNumbers() const
{
    return m_settings.value(key("editor/showLineNumbers"), true).toBool();
}

void AppSettings::setShowLineNumbers(bool show)
{
    m_settings.setValue(key("editor/showLineNumbers"), show);
}

bool AppSettings::wordWrap() const
{
    return m_settings.value(key("editor/wordWrap"), false).toBool();
}

void AppSettings::setWordWrap(bool wrap)
{
    m_settings.setValue(key("editor/wordWrap"), wrap);
}

// ─────────────────────────────────────────────────────────────────────────────
// Session
// ─────────────────────────────────────────────────────────────────────────────

QString AppSettings::lastOpenedProject() const
{
    return m_settings.value(key("session/lastProject"), QString()).toString();
}

void AppSettings::setLastOpenedProject(const QString& path)
{
    m_settings.setValue(key("session/lastProject"), path);
}

QByteArray AppSettings::windowGeometry() const
{
    return m_settings.value(key("session/windowGeometry"), QByteArray()).toByteArray();
}

void AppSettings::setWindowGeometry(const QByteArray& geometry)
{
    m_settings.setValue(key("session/windowGeometry"), geometry);
}

QByteArray AppSettings::windowState() const
{
    return m_settings.value(key("session/windowState"), QByteArray()).toByteArray();
}

void AppSettings::setWindowState(const QByteArray& state)
{
    m_settings.setValue(key("session/windowState"), state);
}

// ─────────────────────────────────────────────────────────────────────────────
// Generic access
// ─────────────────────────────────────────────────────────────────────────────

QVariant AppSettings::value(const QString& k, const QVariant& defaultValue) const
{
    return m_settings.value(key(k), defaultValue);
}

void AppSettings::setValue(const QString& k, const QVariant& val)
{
    m_settings.setValue(key(k), val);
}

// ─────────────────────────────────────────────────────────────────────────────
// App-level (static)
// ─────────────────────────────────────────────────────────────────────────────

QString AppSettings::lastLoggedInUser()
{
    QSettings s("CppAtlas", "CppAtlas");
    return s.value("app/lastLoggedInUser", QString()).toString();
}

void AppSettings::setLastLoggedInUser(const QString& username)
{
    QSettings s("CppAtlas", "CppAtlas");
    s.setValue("app/lastLoggedInUser", username);
}

// ─────────────────────────────────────────────────────────────────────────────
// Private helper
// ─────────────────────────────────────────────────────────────────────────────

QString AppSettings::key(const QString& k) const
{
    if (m_username.isEmpty()) {
        return k;
    }
    // Namespace: "Users/alice/ui/theme"
    return QString("Users/%1/%2").arg(m_username, k);
}