#include "core/keyboard.h"

namespace predator {

KeyboardState KeyboardState::default_state() {
    KeyboardState state;
    state.zones = {
        {ZoneId{1}, Color{255, 255, 255}},
        {ZoneId{2}, Color{255, 255, 255}},
        {ZoneId{3}, Color{255, 255, 255}},
        {ZoneId{4}, Color{255, 255, 255}},
    };
    return state;
}

std::expected<void, KeyboardError> apply(const KeyboardState& state, KeyboardBackend& backend) {
    if (state.mode == Mode::Static) {
        for (const auto& zone_state : state.zones) {
            Color color = state.zones_linked ? state.color : zone_state.color;
            StaticSpec spec{zone_state.zone, color};
            if (auto result = backend.write_static(build_static_payload(spec)); !result) {
                return std::unexpected(result.error());
            }
        }
        if (auto result = backend.write_dynamic(build_static_brightness_payload(state.brightness)); !result) {
            return std::unexpected(result.error());
        }
    } else {
        DynamicSpec spec{state.mode, state.speed, state.brightness, state.direction, state.color};
        if (auto result = backend.write_dynamic(build_dynamic_payload(spec)); !result) {
            return std::unexpected(result.error());
        }
    }
    return {};
}

} // namespace predator
