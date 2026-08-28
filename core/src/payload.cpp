#include "core/payload.h"

namespace predator {

std::array<uint8_t, 16> build_dynamic_payload(const DynamicSpec& spec) {
    std::array<uint8_t, 16> payload{};
    payload[0] = static_cast<uint8_t>(spec.mode);
    payload[1] = spec.speed;
    payload[2] = spec.brightness;
    payload[3] = (spec.mode == Mode::Wave) ? 8 : 0;
    payload[4] = static_cast<uint8_t>(spec.direction);
    payload[5] = spec.color.red;
    payload[6] = spec.color.green;
    payload[7] = spec.color.blue;
    payload[9] = 1; // commit flag
    return payload;
}

std::array<uint8_t, 4> build_static_payload(const StaticSpec& spec) {
    return {spec.zone.bitmask(), spec.color.red, spec.color.green, spec.color.blue};
}

std::array<uint8_t, 16> build_static_brightness_payload(uint8_t brightness) {
    std::array<uint8_t, 16> payload{};
    payload[2] = brightness;
    payload[9] = 1; // commit flag
    return payload;
}

} // namespace predator
