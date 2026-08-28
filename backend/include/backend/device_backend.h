#pragma once

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

#include "core/keyboard.h"

namespace predator {

class DeviceKeyboardBackend : public KeyboardBackend {
public:
    static std::expected<DeviceKeyboardBackend, KeyboardError> create(std::string dynamic_device_path,
                                                                       std::string static_device_path);
    static std::expected<DeviceKeyboardBackend, KeyboardError> with_default_paths();

    std::expected<void, KeyboardError> write_dynamic(std::array<uint8_t, 16> payload) override;
    std::expected<void, KeyboardError> write_static(std::array<uint8_t, 4> payload) override;

    static std::string resolve_device_path(const char* environment_variable, std::string_view fallback);

private:
    DeviceKeyboardBackend(std::string dynamic_device_path, std::string static_device_path);
    static std::expected<void, KeyboardError> write_to_file(const std::string& path, const void* data, std::size_t size);

    std::string dynamic_device_path_;
    std::string static_device_path_;
};

} // namespace predator
