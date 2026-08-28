#pragma once

#include <array>
#include <cstdint>

#include "core/color.h"
#include "core/effect.h"
#include "core/zone.h"

namespace predator {

struct DynamicSpec {
    Mode mode = Mode::Static;
    uint8_t speed = 0;
    uint8_t brightness = 0;
    Direction direction = Direction::RightToLeft;
    Color color;
    bool operator==(const DynamicSpec&) const = default;
};

struct StaticSpec {
    ZoneId zone;
    Color color;
    bool operator==(const StaticSpec&) const = default;
};

std::array<uint8_t, 16> build_dynamic_payload(const DynamicSpec& spec);
std::array<uint8_t, 4> build_static_payload(const StaticSpec& spec);
std::array<uint8_t, 16> build_static_brightness_payload(uint8_t brightness);

} // namespace predator
