#include <QtGui/QGuiApplication>
#include <QtQml>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <qpa/qplatformnativeinterface.h>
#include <QScopedPointer>
#include <QTimer>
#include <sailfishapp.h>
#include "viewhelper.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setApplicationVersion(QString(APP_VERSION));
    application->setQuitOnLastWindowClosed(false);
    QScopedPointer<ViewHelper> helper(new ViewHelper(application.data()));

    QTimer::singleShot(1, helper.data(), SLOT(checkActiveOverlay()));
    if (argc == 1) {
        QTimer::singleShot(2, helper.data(), SLOT(checkActiveSettings()));
    }

    return application->exec();
}
