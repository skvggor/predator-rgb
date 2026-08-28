#pragma once

#include <cstdint>
#include <string>

namespace predator {

enum class ColorError {
    InvalidFormat,
    InvalidLength,
    InvalidHex,
};

struct ZoneError {
    uint8_t value = 0;
};

struct KeyboardError {
    enum Kind { DeviceNotFound, Io } kind;
    std::string detail;
};

} // namespace predator
