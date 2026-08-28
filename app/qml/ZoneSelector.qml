import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import predator_rgb

Panel {
    id: root

    property int selectedZone: 0
    property bool zonesLinked: true
    property color zone1Color: "#FFFFFF"
    property color zone2Color: "#FFFFFF"
    property color zone3Color: "#FFFFFF"
    property color zone4Color: "#FFFFFF"

    signal zoneSelected(int zoneIndex)
    signal linkToggled(bool linked)

    title: "Zones"

    RowLayout {
        spacing: 16
        Layout.fillWidth: true

        Text {
            text: "LINK ZONES"
            color: Theme.foreground
            font.pixelSize: 13
            font.weight: Font.Bold
        }

        Rectangle {
            width: 52
            height: 28
            color: root.zonesLinked ? Theme.accent : Theme.controlBackground
            border.color: root.zonesLinked ? Theme.accent : Theme.border
            border.width: 1

            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            Behavior on border.color {
                ColorAnimation { duration: 200 }
            }

            Rectangle {
                x: root.zonesLinked ? parent.width - width - 2 : 2
                y: 2
                width: 24
                height: 24
                color: Theme.brightForeground

                Behavior on x {
                    NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.zonesLinked = !root.zonesLinked
                    root.linkToggled(root.zonesLinked)
                }
            }
        }
    }

    RowLayout {
        spacing: 16
        Layout.fillWidth: true

        Repeater {
            model: 4

            Rectangle {
                id: zoneItem

                property int zoneIndex: index + 1
                property bool isSelected: root.selectedZone === zoneItem.zoneIndex
                property color zoneColor: zoneItem.zoneIndex === 1 ? root.zone1Color
                                       : zoneItem.zoneIndex === 2 ? root.zone2Color
                                       : zoneItem.zoneIndex === 3 ? root.zone3Color
                                       : root.zone4Color

                Layout.fillWidth: true
                Layout.preferredHeight: 48
                color: Theme.controlBackground
                border.color: zoneItem.isSelected ? Theme.accent : Theme.border
                border.width: zoneItem.isSelected ? 2 : 1
                opacity: root.zonesLinked ? 0.3 : 1.0

                Behavior on border.color {
                    ColorAnimation { duration: 200 }
                }

                Behavior on border.width {
                    NumberAnimation { duration: 200 }
                }

                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Rectangle {
                        width: 24
                        height: 24
                        color: zoneItem.zoneColor
                        border.color: Qt.rgba(1, 1, 1, 0.2)
                        border.width: 1
                    }

                    Text {
                        text: "ZONE " + zoneItem.zoneIndex
                        color: Theme.foreground
                        font.pixelSize: 13
                        font.weight: Font.Bold
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: !root.zonesLinked
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onEntered: {
                        if (!root.zonesLinked) {
                            zoneItem.border.color = Theme.brightMagenta
                        }
                    }

                    onExited: {
                        if (!root.zonesLinked) {
                            zoneItem.border.color = zoneItem.isSelected ? Theme.accent : Theme.border
                        }
                    }

                    onClicked: root.zoneSelected(zoneItem.zoneIndex)
                }
            }
        }
    }
}
