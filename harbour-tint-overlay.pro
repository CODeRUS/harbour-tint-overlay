TARGET = harbour-tint-overlay

QT += dbus gui-private
CONFIG += sailfishapp
PKGCONFIG += mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    src/main.cpp \
    src/configview.cpp \
    src/viewhelper.cpp \
    src/colorhelper.cpp

HEADERS += \
    src/configview.h \
    src/viewhelper.h \
    src/colorhelper.h

OTHER_FILES += qml/harbour-tint-overlay.qml \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/SecondPage.qml \
    qml/settings.qml \
    qml/overlay.qml \
    rpm/harbour-tint-overlay.spec \
    harbour-tint-overlay.desktop
