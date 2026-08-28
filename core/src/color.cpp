#include "core/color.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <optional>
#include <sstream>

namespace predator {

namespace {

std::string_view trim(std::string_view view) {
    while (!view.empty() && std::isspace(static_cast<unsigned char>(view.front()))) {
        view.remove_prefix(1);
    }
    while (!view.empty() && std::isspace(static_cast<unsigned char>(view.back()))) {
        view.remove_suffix(1);
    }
    return view;
}

std::optional<int> hex_digit_value(char character) {
    if (character >= '0' && character <= '9') return character - '0';
    if (character >= 'a' && character <= 'f') return character - 'a' + 10;
    if (character >= 'A' && character <= 'F') return character - 'A' + 10;
    return std::nullopt;
}

std::optional<uint8_t> parse_hex_byte(std::string_view view) {
    if (view.size() != 2) return std::nullopt;
    auto high = hex_digit_value(view[0]);
    auto low = hex_digit_value(view[1]);
    if (!high || !low) return std::nullopt;
    return static_cast<uint8_t>((*high << 4) | *low);
}

} // namespace

uint8_t Color::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<uint8_t>(value);
}

std::expected<Color, ColorError> Color::from_hex(std::string_view hex) {
    auto trimmed = trim(hex);
    if (trimmed.empty() || trimmed.front() != '#') {
        return std::unexpected(ColorError::InvalidFormat);
    }
    std::string_view body = trimmed.substr(1);
    if (body.size() != 6) {
        return std::unexpected(ColorError::InvalidLength);
    }
    auto red = parse_hex_byte(body.substr(0, 2));
    auto green = parse_hex_byte(body.substr(2, 2));
    auto blue = parse_hex_byte(body.substr(4, 2));
    if (!red || !green || !blue) {
        return std::unexpected(ColorError::InvalidHex);
    }
    return Color{*red, *green, *blue};
}

std::string Color::to_hex() const {
    std::ostringstream stream;
    stream << '#' << std::uppercase << std::hex << std::setfill('0')
           << std::setw(2) << static_cast<int>(red)
           << std::setw(2) << static_cast<int>(green)
           << std::setw(2) << static_cast<int>(blue);
    return stream.str();
}

Color hsv_to_rgb(Hsv hsv) {
    const float hue = hsv.hue;
    const float saturation = hsv.saturation;
    const float value = hsv.value;

    if (saturation == 0.0f) {
        uint8_t gray = static_cast<uint8_t>(std::round(value * 255.0f));
        return {gray, gray, gray};
    }

    int sector = static_cast<int>(std::floor(hue / 60.0f));
    float fraction = (hue / 60.0f) - static_cast<float>(sector);
    float p = value * (1.0f - saturation);
    float q = value * (1.0f - fraction * saturation);
    float t = value * (1.0f - (1.0f - fraction) * saturation);

    float red, green, blue;
    switch (sector % 6) {
        case 0: red = value; green = t;     blue = p; break;
        case 1: red = q;     green = value; blue = p; break;
        case 2: red = p;     green = value; blue = t; break;
        case 3: red = p;     green = q;     blue = value; break;
        case 4: red = t;     green = p;     blue = value; break;
        default: red = value; green = p;    blue = q; break;
    }

    return {
        static_cast<uint8_t>(std::round(red * 255.0f)),
        static_cast<uint8_t>(std::round(green * 255.0f)),
        static_cast<uint8_t>(std::round(blue * 255.0f)),
    };
}

Hsv rgb_to_hsv(Color color) {
    float red = color.red / 255.0f;
    float green = color.green / 255.0f;
    float blue = color.blue / 255.0f;

    float max = std::max({red, green, blue});
    float min = std::min({red, green, blue});
    float value = max;
    float delta = max - min;
    float saturation = (max == 0.0f) ? 0.0f : delta / max;

    float hue;
    if (delta == 0.0f) {
        hue = 0.0f;
    } else if (max == red) {
        float quotient = (green - blue) / delta;
        float euclidean = quotient - 6.0f * std::floor(quotient / 6.0f);
        hue = 60.0f * euclidean;
    } else if (max == green) {
        hue = 60.0f * (((blue - red) / delta) + 2.0f);
    } else {
        hue = 60.0f * (((red - green) / delta) + 4.0f);
    }

    return {hue, saturation, value};
}

} // namespace predator
