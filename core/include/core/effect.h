#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

namespace predator {

enum class Mode : uint8_t {
    Static = 0,
    Breath = 1,
    Neon = 2,
    Wave = 3,
    Shifting = 4,
    Zoom = 5,
};

enum class Direction : uint8_t {
    RightToLeft = 1,
    LeftToRight = 2,
};

std::string_view mode_to_string(Mode mode);
std::expected<Mode, std::string> mode_from_string(std::string_view text);

std::string_view direction_to_string(Direction direction);
std::expected<Direction, std::string> direction_from_string(std::string_view text);

} // namespace predator
