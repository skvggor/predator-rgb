import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import predator_rgb

ApplicationWindow {
    id: window
    width: 960
    height: 640
    minimumWidth: 760
    minimumHeight: 520
    visible: true
    title: "Predator RGB Keyboard"
    color: Theme.background

    Material.theme: Material.Dark
    Material.accent: Theme.accent
    Material.primary: Theme.accent
    Material.foreground: Theme.foreground
    Material.background: Theme.background

    ColumnLayout {
        id: body
        anchors.centerIn: parent
        width: Math.min(parent.width - 64, 880)
        spacing: 24

        RowLayout {
            spacing: 16
            Layout.fillWidth: true

            Rectangle {
                width: 48
                height: 48
                Layout.alignment: Qt.AlignVCenter

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: Theme.accent }
                    GradientStop { position: 1.0; color: Theme.magenta }
                }
            }

            ColumnLayout {
                spacing: 4
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true

                Text {
                    text: "PREDATOR RGB"
                    color: Theme.foreground
                    font.pixelSize: 24
                    font.weight: Font.Bold
                    font.letterSpacing: 1.0
                }

                Text {
                    text: "Keyboard Controller"
                    color: Theme.darkForeground
                    font.pixelSize: 14
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
        spacing: 16

            ColorPicker {
                Layout.fillWidth: true
                Layout.preferredWidth: 320
                Layout.alignment: Qt.AlignTop
                visible: controller.mode !== "Neon" && controller.mode !== "Breath" && controller.mode !== "Wave"
                hue: controller.hue
                saturation: controller.saturation
                value: controller.value
                currentColor: controller.currentColor
                hexText: controller.hexText
                selectedZone: controller.selectedZone
                onColorChanged: (hue, saturation, value) => controller.colorChanged(hue, saturation, value)
            }

            EffectControls {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                selectedMode: controller.mode
                speed: controller.speed
                brightness: controller.brightness
                direction: controller.direction
                onModeEdited: (mode) => controller.mode = mode
                onSpeedEdited: (speed) => controller.speed = speed
                onBrightnessEdited: (brightness) => controller.brightness = brightness
                onDirectionEdited: (direction) => controller.direction = direction
            }
        }

        ZoneSelector {
            Layout.fillWidth: true
            visible: controller.mode === "Static"
            selectedZone: controller.selectedZone
            zonesLinked: controller.zonesLinked
            zone1Color: controller.zone1Color
            zone2Color: controller.zone2Color
            zone3Color: controller.zone3Color
            zone4Color: controller.zone4Color
            onZoneSelected: (zoneIndex) => controller.pickZone(zoneIndex)
            onLinkToggled: (linked) => {
                controller.zonesLinked = linked
                if (linked) controller.selectedZone = 0
            }
        }

        Rectangle {
            id: applyButton
            Layout.fillWidth: true
            height: 56

            property color defaultColor1: Theme.accent
            property color defaultColor2: Theme.magenta
            property color currentColor1: defaultColor1
            property color currentColor2: defaultColor2

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: applyButton.currentColor1 }
                GradientStop { position: 1.0; color: applyButton.currentColor2 }
            }

            Behavior on currentColor1 { ColorAnimation { duration: 200 } }
            Behavior on currentColor2 { ColorAnimation { duration: 200 } }

            Text {
                anchors.centerIn: parent
                text: "APPLY"
                color: "#FFFFFF"
                font.pixelSize: 18
                font.weight: Font.Bold
                font.letterSpacing: 2.0
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: controller.applyKeyboard()
            }

            Timer {
                id: resetTimer
                interval: 1500
                onTriggered: {
                    applyButton.currentColor1 = applyButton.defaultColor1
                    applyButton.currentColor2 = applyButton.defaultColor2
                    controller.status = "Ready"
                }
            }

            Connections {
                target: controller
                function onStatusChanged() {
                    if (controller.status === "success") {
                        applyButton.currentColor1 = "#2E7D32"
                        applyButton.currentColor2 = "#388E3C"
                        resetTimer.restart()
                    } else if (controller.status === "error") {
                        applyButton.currentColor1 = "#C62828"
                        applyButton.currentColor2 = "#D32F2F"
                        resetTimer.restart()
                    }
                }
            }
        }
    }
}
