#include "backend/device_backend.h"
#include "backend/fake_keyboard_backend.h"

#include <gtest/gtest.h>

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

using namespace predator;

namespace {

class TempFile {
public:
    TempFile() {
        auto directory = std::filesystem::temp_directory_path() / "predator_rgb_test";
        std::filesystem::create_directories(directory);
        path_ = directory / ("dev_" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + ".bin");
        std::ofstream(path_) << "";
    }
    ~TempFile() { std::filesystem::remove(path_); }
    std::string path() const { return path_.string(); }

private:
    std::filesystem::path path_;
};

class TempDirectory {
public:
    TempDirectory() {
        path_ = std::filesystem::temp_directory_path() / ("predator_rgb_dir_" + std::to_string(reinterpret_cast<std::uintptr_t>(this)));
        std::filesystem::create_directories(path_);
    }
    ~TempDirectory() { std::filesystem::remove_all(path_); }
    std::string path() const { return path_.string(); }

private:
    std::filesystem::path path_;
};

} // namespace

TEST(DeviceBackendTest, WriteDynamicAndStaticRoundtrip) {
    TempFile dynamic_file;
    TempFile static_file;

    auto backend = DeviceKeyboardBackend::create(dynamic_file.path(), static_file.path());
    ASSERT_TRUE(backend.has_value());

    std::array<uint8_t, 16> dynamic_payload{3, 5, 100, 8, 1, 255, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    std::array<uint8_t, 4> static_payload{0b0010, 128, 64, 192};

    ASSERT_TRUE(backend->write_dynamic(dynamic_payload).has_value());
    ASSERT_TRUE(backend->write_static(static_payload).has_value());

    std::ifstream dynamic_stream(dynamic_file.path(), std::ios::binary);
    std::string dynamic_content((std::istreambuf_iterator<char>(dynamic_stream)), std::istreambuf_iterator<char>());
    EXPECT_EQ(dynamic_content, std::string(reinterpret_cast<const char*>(dynamic_payload.data()), dynamic_payload.size()));

    std::ifstream static_stream(static_file.path(), std::ios::binary);
    std::string static_content((std::istreambuf_iterator<char>(static_stream)), std::istreambuf_iterator<char>());
    EXPECT_EQ(static_content, std::string(reinterpret_cast<const char*>(static_payload.data()), static_payload.size()));
}

TEST(DeviceBackendTest, NonexistentDynamicPathRejected) {
    TempFile static_file;
    auto backend = DeviceKeyboardBackend::create("/nonexistent/path", static_file.path());
    ASSERT_FALSE(backend.has_value());
    EXPECT_EQ(backend.error().kind, KeyboardError::DeviceNotFound);
}

TEST(DeviceBackendTest, NonexistentStaticPathRejected) {
    TempFile dynamic_file;
    auto backend = DeviceKeyboardBackend::create(dynamic_file.path(), "/nonexistent/path");
    ASSERT_FALSE(backend.has_value());
    EXPECT_EQ(backend.error().kind, KeyboardError::DeviceNotFound);
}

TEST(DeviceBackendTest, ResolveDevicePathUsesEnvironmentValue) {
    const char* variable = "PREDATOR_RGB_TEST_RESOLVE_SET";
    ::setenv(variable, "/dev/custom", 1);
    EXPECT_EQ(DeviceKeyboardBackend::resolve_device_path(variable, "/dev/default"), "/dev/custom");
    ::unsetenv(variable);
}

TEST(DeviceBackendTest, ResolveDevicePathFallsBackWhenUnset) {
    const char* variable = "PREDATOR_RGB_TEST_RESOLVE_UNSET";
    ::unsetenv(variable);
    EXPECT_EQ(DeviceKeyboardBackend::resolve_device_path(variable, "/dev/default"), "/dev/default");
}

TEST(DeviceBackendTest, ResolveDevicePathIgnoresEmptyEnvironmentValue) {
    const char* variable = "PREDATOR_RGB_TEST_RESOLVE_EMPTY";
    ::setenv(variable, "", 1);
    EXPECT_EQ(DeviceKeyboardBackend::resolve_device_path(variable, "/dev/default"), "/dev/default");
    ::unsetenv(variable);
}

TEST(DeviceBackendTest, WithDefaultPathsResolvesWithoutPanicking) {
    auto backend = DeviceKeyboardBackend::with_default_paths();
    if (backend.has_value()) {
        SUCCEED();
    } else {
        EXPECT_EQ(backend.error().kind, KeyboardError::DeviceNotFound);
    }
}

TEST(DeviceBackendTest, WritableThroughDynamicDispatch) {
    TempFile dynamic_file;
    TempFile static_file;

    auto created = DeviceKeyboardBackend::create(dynamic_file.path(), static_file.path());
    ASSERT_TRUE(created.has_value());
    KeyboardBackend& backend = *created;

    std::array<uint8_t, 16> dynamic_payload{3, 5, 100, 8, 1, 255, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    std::array<uint8_t, 4> static_payload{0b0010, 128, 64, 192};

    EXPECT_TRUE(backend.write_dynamic(dynamic_payload).has_value());
    EXPECT_TRUE(backend.write_static(static_payload).has_value());
}

TEST(DeviceBackendTest, WriteMapsIoErrors) {
    TempDirectory dynamic_directory;
    TempDirectory static_directory;

    auto backend = DeviceKeyboardBackend::create(dynamic_directory.path(), static_directory.path());
    ASSERT_TRUE(backend.has_value());

    auto dynamic_result = backend->write_dynamic({});
    ASSERT_FALSE(dynamic_result.has_value());
    EXPECT_EQ(dynamic_result.error().kind, KeyboardError::Io);

    auto static_result = backend->write_static({});
    ASSERT_FALSE(static_result.has_value());
    EXPECT_EQ(static_result.error().kind, KeyboardError::Io);
}

TEST(FakeBackendTest, RecordsPayloads) {
    FakeKeyboardBackend backend;
    std::array<uint8_t, 16> dynamic_payload{};
    dynamic_payload[0] = 1;
    std::array<uint8_t, 4> static_payload{};
    static_payload[0] = 2;

    EXPECT_TRUE(backend.write_dynamic(dynamic_payload).has_value());
    EXPECT_TRUE(backend.write_static(static_payload).has_value());

    ASSERT_EQ(backend.dynamic_payloads.size(), 1u);
    EXPECT_EQ(backend.dynamic_payloads[0][0], 1);
    ASSERT_EQ(backend.static_payloads.size(), 1u);
    EXPECT_EQ(backend.static_payloads[0][0], 2);

    backend.clear();
    EXPECT_TRUE(backend.dynamic_payloads.empty());
    EXPECT_TRUE(backend.static_payloads.empty());
}
