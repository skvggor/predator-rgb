#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <vector>

#include "core/color.h"
#include "core/errors.h"
#include "core/effect.h"
#include "core/payload.h"
#include "core/zone.h"

namespace predator {

class KeyboardBackend {
public:
    virtual ~KeyboardBackend() = default;
    virtual std::expected<void, KeyboardError> write_dynamic(std::array<uint8_t, 16> payload) = 0;
    virtual std::expected<void, KeyboardError> write_static(std::array<uint8_t, 4> payload) = 0;
};

struct KeyboardState {
    Mode mode = Mode::Static;
    uint8_t speed = 4;
    uint8_t brightness = 100;
    Direction direction = Direction::RightToLeft;
    Color color{255, 255, 255};
    bool zones_linked = true;
    std::vector<ZoneState> zones;

    static KeyboardState default_state();
    bool operator==(const KeyboardState&) const = default;
};

std::expected<void, KeyboardError> apply(const KeyboardState& state, KeyboardBackend& backend);

} // namespace predator
