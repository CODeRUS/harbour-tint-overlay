#include "viewhelper.h"
#include "colorhelper.h"
#include "configview.h"

#include <qpa/qplatformnativeinterface.h>
#include <QtQml>
#include <QTimer>
#include <QDebug>
#include <QDBusReply>
#include <QDir>
#include <QFile>
#include <QTextStream>

ViewHelper::ViewHelper(QObject *parent) :
    QObject(parent),
    overlayView(NULL),
    settingsView(NULL)
{
    QDBusConnection::sessionBus().connect("", "", "com.jolla.jollastore", "packageStatusChanged", this, SLOT(onPackageStatusChanged(QString, int)));
}

void ViewHelper::closeOverlay()
{
    if (overlayView) {
        QDBusConnection::sessionBus().unregisterObject("/harbour/tintoverlay/overlay");
        QDBusConnection::sessionBus().unregisterService("harbour.tintoverlay.overlay");
        overlayView->close();
        delete overlayView;
        overlayView = NULL;

        if (!settingsView) {
            qGuiApp->quit();
        }
    }
    else {
        QDBusInterface iface("harbour.tintoverlay.overlay", "/harbour/tintoverlay/overlay", "harbour.tintoverlay");
        iface.call(QDBus::NoBlock, "exit");
    }
}

void ViewHelper::checkOverlay()
{
    QDBusInterface iface("harbour.tintoverlay.overlay", "/harbour/tintoverlay/overlay", "harbour.tintoverlay");
    iface.call(QDBus::NoBlock, "pingOverlay");
}

void ViewHelper::startOverlay()
{
    checkActiveOverlay();
}

void ViewHelper::openStore()
{
    QDBusInterface iface("com.jolla.jollastore", "/StoreClient", "com.jolla.jollastore");
    iface.call(QDBus::NoBlock, "showApp", "harbour-tint-overlay");
}

void ViewHelper::checkActiveSettings()
{
    bool newSettings = QDBusConnection::sessionBus().registerService("harbour.tintoverlay.settings");
    if (newSettings) {
        showSettings();
    }
    else {
        QDBusInterface iface("harbour.tintoverlay.settings", "/harbour/tintoverlay/settings", "harbour.tintoverlay");
        iface.call(QDBus::NoBlock, "show");
        qGuiApp->exit(0);
        return;
    }
}

void ViewHelper::checkActiveOverlay()
{
    bool inactive = QDBusConnection::sessionBus().registerService("harbour.tintoverlay.overlay");
    if (inactive) {
        showOverlay();
    }
}

void ViewHelper::show()
{
    if (settingsView) {
        settingsView->raise();
        checkActiveOverlay();
    }
}

void ViewHelper::exit()
{
    QTimer::singleShot(100, qGuiApp, SLOT(quit()));
}

void ViewHelper::pingOverlay()
{
    if (overlayView) {
        Q_EMIT overlayRunning();
    }
}

void ViewHelper::showOverlay()
{
    qDebug() << "show overlay";
    QDBusConnection::sessionBus().registerObject("/harbour/tintoverlay/overlay", this, QDBusConnection::ExportScriptableSlots | QDBusConnection::ExportScriptableSignals);

    qGuiApp->setApplicationName("Tint Overlay");
    qGuiApp->setApplicationDisplayName("Tint Overlay");

    overlayView = SailfishApp::createView();
    QObject::connect(overlayView->engine(), SIGNAL(quit()), qGuiApp, SLOT(quit()));
    overlayView->setTitle("Tint Overlay");

    overlayView->setSource(SailfishApp::pathTo("qml/overlay.qml"));
    ConfigView *conf = new ConfigView(overlayView);

    QDBusConnection::sessionBus().disconnect("", "/harbour/tintoverlay/overlay", "harbour.tintoverlay",
                                          "overlayRunning", this, SIGNAL(overlayRunning()));

    Q_EMIT overlayRunning();
}

void ViewHelper::showSettings()
{
    qDebug() << "show settings";
    QDBusConnection::sessionBus().registerObject("/harbour/tintoverlay/settings", this, QDBusConnection::ExportScriptableSlots | QDBusConnection::ExportScriptableSignals);

    qGuiApp->setApplicationName("Tint Overlay Settings");
    qGuiApp->setApplicationDisplayName("Tint Overlay Settings");

    settingsView = SailfishApp::createView();
    settingsView->setTitle("Tint Overlay Settings");
    settingsView->rootContext()->setContextProperty("helper", this);
    settingsView->rootContext()->setContextProperty("colorHelper", new ColorHelper(this));
    settingsView->setSource(SailfishApp::pathTo("qml/settings.qml"));
    settingsView->showFullScreen();

    if (!overlayView) {
        QDBusConnection::sessionBus().connect("", "/harbour/tintoverlay/overlay", "harbour.tintoverlay",
                                              "overlayRunning", this, SIGNAL(overlayRunning()));
    }

    QObject::connect(settingsView, SIGNAL(destroyed()), this, SLOT(onSettingsDestroyed()));
    QObject::connect(settingsView, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onSettingsClosing(QQuickCloseEvent*)));
}

void ViewHelper::onPackageStatusChanged(const QString &package, int status)
{
    if (package == "harbour-tint-overlay" && status != 1) {
        if (overlayView) {
            Q_EMIT applicationRemoval();
        }
        else if (settingsView) {
            qGuiApp->quit();
        }
    }
}

void ViewHelper::onSettingsDestroyed()
{
    QObject::disconnect(settingsView, 0, 0, 0);
    settingsView = NULL;
}

void ViewHelper::onSettingsClosing(QQuickCloseEvent *)
{
    settingsView->destroy();
    settingsView->deleteLater();

    QDBusConnection::sessionBus().unregisterObject("/harbour/tintoverlay/settings");
    QDBusConnection::sessionBus().unregisterService("harbour.tintoverlay.settings");

    if (!overlayView) {
        qGuiApp->quit();
    }

    //QDBusConnection::sessionBus().disconnect("", "/harbour/tintoverlay/overlay", "harbour.tintoverlay",
    //                                      "overlayRunning", 0, 0);
}
