import QtQuick 2.1
import Sailfish.Silica 1.0

;import org.nemomobile.configuration 1.0

Page {
    id: page
    objectName: "mainPage"

    SilicaFlickable {
        id: flick
        anchors.fill: page
        contentHeight: content.height

        PullDownMenu {
            MenuItem {
                text: overlayRunning ? "Close overlay" : "Start overlay"
                onClicked: {
                    switchOverlay();
                }
            }

            MenuItem {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        Column {
            id: content
            width: parent.width

            PageHeader {
                title: "Tint Overlay"
            }

            Slider {
                id: redSlider
                width: parent.width
                label: "Red"
                minimumValue: 0
                maximumValue: 255
                stepSize: 1
                value: colorHelper.red(configuration.color)
                valueText: value
                onReleased: writeColor()
            }

            Slider {
                id: greenSlider
                width: parent.width
                label: "Green"
                minimumValue: 0
                maximumValue: 255
                stepSize: 1
                value: colorHelper.green(configuration.color)
                valueText: value
                onReleased: writeColor()
            }

            Slider {
                id: blueSlider
                width: parent.width
                label: "Blue"
                minimumValue: 0
                maximumValue: 255
                stepSize: 1
                value: colorHelper.blue(configuration.color)
                valueText: value
                onReleased: writeColor()
            }

            Slider {
                id: alphaSlider
                width: parent.width
                label: "Alpha"
                minimumValue: 1
                maximumValue: 128
                stepSize: 1
                value: colorHelper.alpha(configuration.color)
                valueText: value
                onReleased: writeColor()
            }
        }
    }

    function writeColor() {
        var newColor = colorHelper.colorString(Qt.rgba(redSlider.value / 255.0,
                                                       greenSlider.value / 255.0,
                                                       blueSlider.value / 255.0,
                                                       alphaSlider.value / 255.0))
        configuration.color = newColor
    }

    ConfigurationGroup {
        id: configuration
        path: "/apps/harbour-tint-overlay"
        property string color: "#05001000"
    }

    Component.onCompleted: {
        helper.checkOverlay();
    }
}
