import QtQuick
import predator_rgb

Item {
    id: root

    width: 24
    height: 200

    property int hue: 0

    signal selected(int hue)

    Rectangle {
        id: bar
        anchors.fill: parent
        radius: 0
        clip: true

        gradient: Gradient {
            orientation: Gradient.Vertical
            GradientStop { position: 0.0; color: "#FF0000" }
            GradientStop { position: 0.1667; color: "#FFFF00" }
            GradientStop { position: 0.3333; color: "#00FF00" }
            GradientStop { position: 0.5; color: "#00FFFF" }
            GradientStop { position: 0.6667; color: "#0000FF" }
            GradientStop { position: 0.8333; color: "#FF00FF" }
            GradientStop { position: 1.0; color: "#FF0000" }
        }

        Rectangle {
            id: cursor

            x: -4
            y: bar.height * (root.hue / 360.0) - 8
            width: 32
            height: 16
            radius: 0
            color: "transparent"
            border.color: Theme.brightForeground
            border.width: 2
            scale: barMouseArea.pressed ? 1.2 : barMouseArea.containsMouse ? 1.1 : 1.0

            Behavior on y {
                NumberAnimation { duration: 50; easing.type: Easing.OutCubic }
            }

            Behavior on scale {
                NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
            }
        }

        MouseArea {
            id: barMouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onPressed: (mouse) => emit(mouse)
            onPositionChanged: (mouse) => { if (pressed) emit(mouse) }

            function emit(mouse) {
                root.selected(Math.round(Math.max(0, Math.min(0.999, mouse.y / height)) * 360))
            }
        }
    }
}
