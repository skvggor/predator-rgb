#pragma once

#include <cstdint>
#include <expected>
#include <string_view>

#include "core/errors.h"

namespace predator {

struct Color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    static uint8_t clamp(int value);
    static std::expected<Color, ColorError> from_hex(std::string_view hex);
    std::string to_hex() const;

    bool operator==(const Color&) const = default;
};

struct Hsv {
    float hue = 0.0f;
    float saturation = 0.0f;
    float value = 0.0f;
};

Color hsv_to_rgb(Hsv hsv);
Hsv rgb_to_hsv(Color color);

} // namespace predator
