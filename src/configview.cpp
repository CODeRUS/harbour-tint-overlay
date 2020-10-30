#include "configview.h"
#include <wayland-client.h>
#include <QRegion>

static void setWaylandInputRegion(QPlatformNativeInterface *wliface, QWindow *window, const QRegion &region)
{
    if (wl_compositor *wlcompositor = static_cast<wl_compositor *>(
                wliface->nativeResourceForIntegration("compositor"))) {
        if (wl_surface *wlsurface = static_cast<wl_surface *>(
                    wliface->nativeResourceForWindow("surface", window))) {
            wl_region *wlregion = wl_compositor_create_region(wlcompositor);

            for (const QRect &rect : region.rects()) {
                wl_region_add(wlregion, rect.x(), rect.y(), rect.width(), rect.height());
            }

            wl_surface_set_input_region(wlsurface, wlregion);
            wl_region_destroy(wlregion);

            wl_surface_commit(wlsurface);
        }
    }
}

static void setWaylandOpaqueRegion(QPlatformNativeInterface *wliface, QWindow *window, const QRegion &region)
{
    if (wl_compositor *wlcompositor = static_cast<wl_compositor *>(
                wliface->nativeResourceForIntegration("compositor"))) {
        if (wl_surface *wlsurface = static_cast<wl_surface *>(
                    wliface->nativeResourceForWindow("surface", window))) {
            wl_region *wlregion = wl_compositor_create_region(wlcompositor);

            for (const QRect &rect : region.rects()) {
                wl_region_add(wlregion, rect.x(), rect.y(), rect.width(), rect.height());
            }

            wl_surface_set_opaque_region(wlsurface, wlregion);
            wl_region_destroy(wlregion);

            wl_surface_commit(wlsurface);
        }
    }
}

ConfigView::ConfigView(QQuickView *parent) :
    QObject(parent)
{
    parent->installEventFilter(this);
    dconf = new MGConfItem("/apps/harbour-tint-overlay/color");
    QObject::connect(dconf, SIGNAL(valueChanged()), this, SLOT(updateColor()));
    updateColor();
}

bool ConfigView::eventFilter(QObject *object, QEvent *event)
{
    auto *overlayView = qobject_cast<QQuickView*>(object);
    if (!overlayView) {
        return false;
    } else switch (event->type()) {
    case QEvent::PlatformSurface: {
        QPlatformSurfaceEvent *platformEvent = static_cast<QPlatformSurfaceEvent *>(event);

        if (QPlatformWindow *handle = platformEvent->surfaceEventType() == QPlatformSurfaceEvent::SurfaceCreated
                ? overlayView->handle()
                : nullptr) {
            QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();

            native->setWindowProperty(handle, QStringLiteral("CATEGORY"), QStringLiteral("notification"));
            setWaylandOpaqueRegion(native, overlayView, QRegion(0, 0, 0, 0));
            QRegion region(0, 0, 0, 0);
            overlayView->setMask(region);
            native->setWindowProperty(handle, QStringLiteral("MOUSE_REGION"), region);
            setWaylandInputRegion(native, overlayView, region);
        }
        return false;
    }
    default:
        return false;
    }

}

void ConfigView::setColor(qreal red, qreal green, qreal blue, qreal opacity)
{
    QColor color;
    color.setRedF(red);
    color.setGreenF(green);
    color.setBlueF(blue);
    color.setAlphaF(opacity);
    setColor(color);
}

void ConfigView::setColor(const QColor &color)
{
    QQuickView *view = qobject_cast<QQuickView*>(parent());
    if (view) {
        view->close();
        view->setColor(color);
        view->setClearBeforeRendering(true);

        view->create();

        view->show();
    }
}

void ConfigView::updateColor()
{
    QString value = dconf->value("#05001000").toString();
    setColor(QColor(value));
}
