pragma Singleton
import QtQuick
import QtQuick.Controls.Material

QtObject {
    readonly property string mode: "dark"

    readonly property color accent: "#7B68EE"
    readonly property color selection: "#1E1E2E"
    readonly property color muted: "#3A3A4A"

    readonly property color background: "#0A0A0F"
    readonly property color darkBackground: "#06060C"
    readonly property color darkerBackground: "#040408"
    readonly property color lighterBackground: "#141420"

    readonly property color foreground: "#F0F0F5"
    readonly property color darkForeground: "#6A6A80"
    readonly property color lightForeground: "#B0B0C0"
    readonly property color brightForeground: "#FFFFFF"

    readonly property color red: "#F87171"
    readonly property color yellow: "#FBBF24"
    readonly property color orange: "#FB923C"
    readonly property color green: "#4ADE80"
    readonly property color cyan: "#22D3EE"
    readonly property color blue: "#60A5FA"
    readonly property color magenta: "#C084FC"
    readonly property color brown: "#A16207"

    readonly property color brightRed: "#FCA5A5"
    readonly property color brightYellow: "#FDE68A"
    readonly property color brightGreen: "#86EFAC"
    readonly property color brightCyan: "#67E8F9"
    readonly property color brightBlue: "#93C5FD"
    readonly property color brightMagenta: "#D8B4FE"

    readonly property color controlBackground: "#1A1A28"
    readonly property color controlThumb: "#FFFFFF"
    readonly property color controlTrack: "#2A2A3A"

    readonly property color border: "#2A2A3A"
    readonly property color borderSubtle: "#1E1E2E"
    readonly property color borderFocus: accent

    readonly property color success: green
    readonly property color error: red
    readonly property color warning: yellow
    readonly property color info: blue

    readonly property int materialTheme: Material.Dark
    readonly property color materialAccent: accent
}
