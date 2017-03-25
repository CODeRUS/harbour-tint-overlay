import QtQuick 2.1
import Sailfish.Silica 1.0

CoverBackground {
    signal actionTriggered

    CoverPlaceholder {
        text: "Tint\nOverlay\nSettings"
    }

    CoverActionList {
        CoverAction {
            iconSource: appWindow.overlayRunning ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: {
                actionTriggered();
                appWindow.switchOverlay()
            }
        }
    }
}
