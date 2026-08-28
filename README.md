# Predator RGB

RGB keyboard controller for the **Acer Predator Helios Neo 16**, written in C++23 + Qt6/QML.

> For end-user documentation, see [MANUAL.md](MANUAL.md).

## Architecture

```
predator-rgb/
├── core/            # Domain logic (color, effects, zones, payload encoding)
│   ├── include/core/
│   └── src/
├── backend/         # Device I/O (writes to acer-gkbbl kernel devices)
│   ├── include/backend/
│   └── src/
├── app/             # Qt6/QML GUI application
│   ├── qml/         # UI components
│   └── controller.cpp
└── tests/           # GTest unit tests
    ├── core/
    └── backend/
```

### Modules

| Module | Description |
|--------|-------------|
| `core` | Pure domain logic — HSV/RGB conversion, effect modes (Static, Breath, Neon, Wave, Shifting, Zoom), zone management, binary payload encoding. Zero UI dependencies. |
| `backend` | `DeviceKeyboardBackend` writes raw payloads to `/dev/acer-gkbbl-0` (dynamic) and `/dev/acer-gkbbl-static-0` (static). `FakeKeyboardBackend` for testing. |
| `app` | Qt6 Quick application with QML UI. `Controller` bridges QML ↔ core/backend. Persists config to `~/.config/predator-rgb/config.json`. |

### Device Protocol

The application communicates with the `acer-gkbbl` kernel module via two device nodes:

- **Dynamic device** (`/dev/acer-gkbbl-0`): 16-byte payloads for mode, speed, brightness, direction, color
- **Static device** (`/dev/acer-gkbbl-static-0`): 4-byte payloads for per-zone static color (zone bitmask + RGB)

## Prerequisites

- **CMake** >= 3.21
- **Qt6** >= 6.5 (Core, Gui, Quick, QuickControls2)
- **GTest** (for tests)
- **acer-gkbbl** kernel module loaded (the kernel device nodes must exist)

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

Debug build (default):

```sh
cmake -B build
cmake --build build
```

## Tests

```sh
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests cover:
- Color HSV ↔ RGB conversion and hex serialization
- Effect mode and direction serialization/deserialization
- Zone ID bitmask encoding
- Dynamic and static payload building
- Keyboard apply logic with linked/independent zones
- Device backend file I/O and error handling

## Run

```sh
./build/app/predator-rgb
```

### Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| `PREDATOR_RGB_DYNAMIC_DEVICE` | Path to the dynamic keyboard device | `/dev/acer-gkbbl-0` |
| `PREDATOR_RGB_STATIC_DEVICE` | Path to the static keyboard device | `/dev/acer-gkbbl-static-0` |

## Configuration

The application persists all settings to:

```
~/.config/predator-rgb/config.json
```

Stored fields: `hue`, `saturation`, `value`, `mode`, `speed`, `brightness`, `direction`, `zonesLinked`, `zone1Color`–`zone4Color`.

## UI Components

| QML File | Purpose |
|----------|---------|
| `Main.qml` | Application window, layout, apply button with status feedback |
| `ColorPicker.qml` | HSV color picker with hue slider and saturation/value box |
| `HueSlider.qml` | Vertical hue selector (0–360) |
| `SvBox.qml` | Saturation/value 2D picker |
| `EffectControls.qml` | Mode, speed, brightness, direction controls (native Qt Quick Controls) |
| `ZoneSelector.qml` | 4-zone selector with link toggle (Static mode) |
| `Panel.qml` | Reusable panel container with border |
| `Theme.qml` | Singleton dark theme color definitions |

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).
