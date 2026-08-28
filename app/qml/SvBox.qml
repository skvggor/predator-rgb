import QtQuick
import predator_rgb

Item {
    id: root

    width: 200
    height: 200

    property color hueColor: "red"
    property real saturation: 0.0
    property real value: 1.0

    signal selected(real saturation, real value)

    Rectangle {
        id: surface
        anchors.fill: parent
        radius: 0
        clip: true

        Rectangle {
            anchors.fill: parent

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#FFFFFF" }
                GradientStop { position: 1.0; color: root.hueColor }
            }
        }

        Rectangle {
            anchors.fill: parent

            gradient: Gradient {
                GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0) }
                GradientStop { position: 1.0; color: "#000000" }
            }
        }

        Rectangle {
            id: cursor

            x: surface.width * root.saturation - 6
            y: surface.height * (1 - root.value) - 6
            width: 12
            height: 12
            radius: 0
            color: "transparent"
            border.color: Theme.brightForeground
            border.width: 2
            scale: svMouseArea.pressed ? 1.3 : svMouseArea.containsMouse ? 1.15 : 1.0

            Behavior on x {
                NumberAnimation { duration: 50; easing.type: Easing.OutCubic }
            }

            Behavior on y {
                NumberAnimation { duration: 50; easing.type: Easing.OutCubic }
            }

            Behavior on scale {
                NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
            }
        }

        MouseArea {
            id: svMouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onPressed: (mouse) => emit(mouse)
            onPositionChanged: (mouse) => { if (pressed) emit(mouse) }

            function emit(mouse) {
                root.selected(
                    Math.max(0, Math.min(1, mouse.x / width)),
                    Math.max(0, Math.min(1, 1 - mouse.y / height))
                )
            }
        }
    }
}
