#pragma once
#include <QDialog>
#include <QEvent>

/**
 * @brief Base dialog class for CppAtlas dialogs.
 *
 * On Wayland (and optionally X11) a modal dialog that is minimized can lock
 * the application because the window never receives a close/hide event while
 * remaining modal.  This subclass treats a minimize (WindowMinimized) state
 * change as an implicit close request, so the application never gets stuck.
 *
 * Usage — replace QDialog with AtlasDialog:
 *
 * @code
 * class MyDialog : public AtlasDialog { … };
 * @endcode
 *
 * Requires src/ui/AtlasDialog.cpp to be compiled so that CMake's AUTOMOC
 * generates and links the Qt meta-object implementation (moc_AtlasDialog.cpp).
 */
class AtlasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AtlasDialog(QWidget* parent = nullptr,
                         Qt::WindowFlags flags = Qt::Dialog
                                               | Qt::WindowTitleHint
                                               | Qt::WindowCloseButtonHint)
        : QDialog(parent, flags)
    {}

protected:
    void changeEvent(QEvent* event) override
    {
        QDialog::changeEvent(event);
        if (event->type() == QEvent::WindowStateChange) {
            if (windowState() & Qt::WindowMinimized) {
                // Treat minimize as close to prevent app lock on Wayland
                QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
            }
        }
    }
};
