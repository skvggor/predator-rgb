#include "backend/device_backend.h"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace predator {

namespace {

constexpr const char* default_dynamic_device_path = "/dev/acer-gkbbl-0";
constexpr const char* default_static_device_path = "/dev/acer-gkbbl-static-0";
constexpr const char* dynamic_device_path_environment_variable = "PREDATOR_RGB_DYNAMIC_DEVICE";
constexpr const char* static_device_path_environment_variable = "PREDATOR_RGB_STATIC_DEVICE";

bool path_exists(const std::string& path) {
    return ::access(path.c_str(), F_OK) == 0;
}

} // namespace

std::string DeviceKeyboardBackend::resolve_device_path(const char* environment_variable, std::string_view fallback) {
    if (const char* value = std::getenv(environment_variable)) {
        if (value[0] != '\0') {
            return std::string(value);
        }
    }
    return std::string(fallback);
}

DeviceKeyboardBackend::DeviceKeyboardBackend(std::string dynamic_device_path, std::string static_device_path)
    : dynamic_device_path_(std::move(dynamic_device_path)), static_device_path_(std::move(static_device_path)) {}

std::expected<DeviceKeyboardBackend, KeyboardError> DeviceKeyboardBackend::create(std::string dynamic_device_path,
                                                                                   std::string static_device_path) {
    if (!path_exists(dynamic_device_path) || !path_exists(static_device_path)) {
        return std::unexpected(KeyboardError{.kind = KeyboardError::DeviceNotFound});
    }
    return DeviceKeyboardBackend(std::move(dynamic_device_path), std::move(static_device_path));
}

std::expected<DeviceKeyboardBackend, KeyboardError> DeviceKeyboardBackend::with_default_paths() {
    return create(resolve_device_path(dynamic_device_path_environment_variable, default_dynamic_device_path),
                  resolve_device_path(static_device_path_environment_variable, default_static_device_path));
}

std::expected<void, KeyboardError> DeviceKeyboardBackend::write_to_file(const std::string& path, const void* data,
                                                                        std::size_t size) {
    int descriptor = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (descriptor < 0) {
        return std::unexpected(KeyboardError{.kind = KeyboardError::Io, .detail = std::strerror(errno)});
    }
    ssize_t written = ::write(descriptor, data, size);
    int close_result = ::close(descriptor);
    if (written < 0) {
        return std::unexpected(KeyboardError{.kind = KeyboardError::Io, .detail = std::strerror(errno)});
    }
    if (close_result < 0) {
        return std::unexpected(KeyboardError{.kind = KeyboardError::Io, .detail = std::strerror(errno)});
    }
    return {};
}

std::expected<void, KeyboardError> DeviceKeyboardBackend::write_dynamic(std::array<uint8_t, 16> payload) {
    return write_to_file(dynamic_device_path_, payload.data(), payload.size());
}

std::expected<void, KeyboardError> DeviceKeyboardBackend::write_static(std::array<uint8_t, 4> payload) {
    return write_to_file(static_device_path_, payload.data(), payload.size());
}

} // namespace predator
