#pragma once

#include <cstdint>
#include <expected>

#include "core/color.h"
#include "core/errors.h"

namespace predator {

struct ZoneId {
    uint8_t value = 1;

    static std::expected<ZoneId, ZoneError> create(uint8_t value);
    uint8_t bitmask() const;

    bool operator==(const ZoneId&) const = default;
};

struct ZoneState {
    ZoneId zone;
    Color color;
    bool operator==(const ZoneState&) const = default;
};

} // namespace predator
