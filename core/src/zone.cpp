#include "core/zone.h"

namespace predator {

std::expected<ZoneId, ZoneError> ZoneId::create(uint8_t value) {
    if (value >= 1 && value <= 4) return ZoneId{value};
    return std::unexpected(ZoneError{value});
}

uint8_t ZoneId::bitmask() const {
    return static_cast<uint8_t>(1 << (value - 1));
}

} // namespace predator
