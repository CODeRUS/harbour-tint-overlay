import QtQuick 2.1
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: appWindow

    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    property bool overlayRunning: false

    function switchOverlay() {
        if (overlayRunning) {
            overlayRunning = false
            helper.closeOverlay()
        }
        else {
            helper.startOverlay()
        }
    }

    Connections {
        target: helper
        onOverlayRunning: {
            console.log("Received overlay pong")
            overlayRunning = true
        }
    }

    Connections {
        target: _coverObject
        ignoreUnknownSignals: true
        onActionTriggered: {
            console.log("### hello")
            //switchOverlay();
        }
    }

    onApplicationActiveChanged: helper.checkOverlay()
}


