#ifndef AVATARUTILS_H
#define AVATARUTILS_H

#include <QPixmap>
#include <QPainter>
#include <QString>

#ifdef CPPATLAS_SVG_AVAILABLE
#  include <QSvgRenderer>
#endif

/**
 * @brief Loads an avatar pixmap from a file path or Qt resource path.
 *
 * Supports:
 *  - SVG resource paths (e.g. ":/avatars/avatar_blue.svg") when Qt Svg is available
 *  - Raster image paths (PNG, JPG, etc.)
 *  - Absolute file-system paths
 *
 * Returns a null QPixmap if loading fails.
 */
inline QPixmap loadAvatarPixmap(const QString& path, int size = 64)
{
    if (path.isEmpty()) return QPixmap();

    QPixmap px;
    if (path.startsWith(QLatin1String(":/"))) {
#ifdef CPPATLAS_SVG_AVAILABLE
        QSvgRenderer renderer(path);
        if (renderer.isValid()) {
            px = QPixmap(size, size);
            px.fill(Qt::transparent);
            QPainter painter(&px);
            renderer.render(&painter);
            return px;
        }
#endif
        // Fall through to normal load (may work for non-SVG resources)
        px.load(path);
    } else {
        px.load(path);
    }

    if (!px.isNull() && (px.width() != size || px.height() != size)) {
        px = px.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return px;
}

#endif // AVATARUTILS_H
