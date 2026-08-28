#include "core/payload.h"

#include <gtest/gtest.h>

#include <array>

using namespace predator;

namespace {

DynamicSpec dynamic_spec(Mode mode, uint8_t speed, uint8_t brightness, Direction direction, Color color) {
    return {mode, speed, brightness, direction, color};
}

} // namespace

TEST(PayloadTest, DynamicPayloadWave) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Wave, 5, 100, Direction::RightToLeft, {255, 0, 0}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{3, 5, 100, 8, 1, 255, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadBreath) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Breath, 3, 80, Direction::LeftToRight, {0, 255, 0}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{1, 3, 80, 0, 2, 0, 255, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadStatic) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Static, 0, 60, Direction::RightToLeft, {128, 64, 192}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{0, 0, 60, 0, 1, 128, 64, 192, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadZoom) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Zoom, 7, 100, Direction::LeftToRight, {255, 255, 255}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{5, 7, 100, 0, 2, 255, 255, 255, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadNeon) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Neon, 4, 90, Direction::RightToLeft, {0, 0, 255}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{2, 4, 90, 0, 1, 0, 0, 255, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadShifting) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Shifting, 6, 85, Direction::LeftToRight, {255, 128, 0}));
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{4, 6, 85, 0, 2, 255, 128, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, StaticPayloadZone1) {
    auto payload = build_static_payload({ZoneId{1}, {255, 0, 0}});
    EXPECT_EQ(payload, (std::array<uint8_t, 4>{0b0001, 255, 0, 0}));
}

TEST(PayloadTest, StaticPayloadZone2) {
    auto payload = build_static_payload({ZoneId{2}, {10, 20, 30}});
    EXPECT_EQ(payload, (std::array<uint8_t, 4>{0b0010, 10, 20, 30}));
}

TEST(PayloadTest, StaticPayloadZone3) {
    auto payload = build_static_payload({ZoneId{3}, {128, 128, 128}});
    EXPECT_EQ(payload, (std::array<uint8_t, 4>{0b0100, 128, 128, 128}));
}

TEST(PayloadTest, StaticPayloadZone4) {
    auto payload = build_static_payload({ZoneId{4}, {0, 255, 255}});
    EXPECT_EQ(payload, (std::array<uint8_t, 4>{0b1000, 0, 255, 255}));
}

TEST(PayloadTest, StaticBrightnessPayload) {
    auto payload = build_static_brightness_payload(75);
    EXPECT_EQ(payload, (std::array<uint8_t, 16>{0, 0, 75, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(PayloadTest, DynamicPayloadAlwaysHasCommitFlag) {
    const Mode modes[] = {Mode::Static, Mode::Breath, Mode::Neon, Mode::Wave, Mode::Shifting, Mode::Zoom};
    const uint8_t expected_discriminant = 0;
    for (uint8_t index = 0; index < 6; ++index) {
        Mode mode = modes[index];
        auto payload = build_dynamic_payload(dynamic_spec(mode, 0, 0, Direction::RightToLeft, {}));
        EXPECT_EQ(payload[0], index + expected_discriminant);
        EXPECT_EQ(payload[9], 1);
    }
}

TEST(PayloadTest, DynamicPayloadUnusedBytesAreZero) {
    auto payload = build_dynamic_payload(dynamic_spec(Mode::Wave, 5, 100, Direction::RightToLeft, {255, 0, 0}));
    for (uint8_t index : {8u, 10u, 11u, 12u, 13u, 14u, 15u}) {
        EXPECT_EQ(payload[index], 0);
    }
}

TEST(PayloadTest, WaveByte3Is8Others0) {
    EXPECT_EQ(build_dynamic_payload(dynamic_spec(Mode::Wave, 0, 0, Direction::RightToLeft, {}))[3], 8);
    for (Mode mode : {Mode::Static, Mode::Breath, Mode::Neon, Mode::Shifting, Mode::Zoom}) {
        EXPECT_EQ(build_dynamic_payload(dynamic_spec(mode, 0, 0, Direction::RightToLeft, {}))[3], 0);
    }
}
