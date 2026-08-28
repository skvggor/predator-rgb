#include "core/effect.h"

namespace predator {

std::string_view mode_to_string(Mode mode) {
    switch (mode) {
        case Mode::Static: return "static";
        case Mode::Breath: return "breath";
        case Mode::Neon: return "neon";
        case Mode::Wave: return "wave";
        case Mode::Shifting: return "shifting";
        case Mode::Zoom: return "zoom";
    }
    return {};
}

std::expected<Mode, std::string> mode_from_string(std::string_view text) {
    if (text == "static") return Mode::Static;
    if (text == "breath") return Mode::Breath;
    if (text == "neon") return Mode::Neon;
    if (text == "wave") return Mode::Wave;
    if (text == "shifting") return Mode::Shifting;
    if (text == "zoom") return Mode::Zoom;
    return std::unexpected(std::string(text));
}

std::string_view direction_to_string(Direction direction) {
    switch (direction) {
        case Direction::RightToLeft: return "right-to-left";
        case Direction::LeftToRight: return "left-to-right";
    }
    return {};
}

std::expected<Direction, std::string> direction_from_string(std::string_view text) {
    if (text == "right-to-left") return Direction::RightToLeft;
    if (text == "left-to-right") return Direction::LeftToRight;
    return std::unexpected(std::string(text));
}

} // namespace predator
