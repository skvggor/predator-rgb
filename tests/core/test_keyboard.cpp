#include "core/keyboard.h"

#include <gtest/gtest.h>

#include <expected>
#include <vector>

using namespace predator;

namespace {

class FakeBackend : public KeyboardBackend {
public:
    std::expected<void, KeyboardError> write_dynamic(std::array<uint8_t, 16> payload) override {
        dynamic_writes.push_back(payload);
        return {};
    }
    std::expected<void, KeyboardError> write_static(std::array<uint8_t, 4> payload) override {
        static_writes.push_back(payload);
        return {};
    }

    std::vector<std::array<uint8_t, 16>> dynamic_writes;
    std::vector<std::array<uint8_t, 4>> static_writes;
};

class FailingBackend : public KeyboardBackend {
public:
    std::expected<void, KeyboardError> write_dynamic(std::array<uint8_t, 16>) override {
        return std::unexpected(KeyboardError{.kind = KeyboardError::DeviceNotFound});
    }
    std::expected<void, KeyboardError> write_static(std::array<uint8_t, 4>) override {
        return std::unexpected(KeyboardError{.kind = KeyboardError::DeviceNotFound});
    }
};

KeyboardState static_state_linked(uint8_t brightness, Color color) {
    return {
        .mode = Mode::Static,
        .speed = 0,
        .brightness = brightness,
        .direction = Direction::RightToLeft,
        .color = color,
        .zones_linked = true,
        .zones = {
            {ZoneId{1}, {255, 0, 0}},
            {ZoneId{2}, {0, 255, 0}},
            {ZoneId{3}, {0, 0, 255}},
            {ZoneId{4}, {255, 255, 0}},
        },
    };
}

} // namespace

TEST(KeyboardTest, ApplyStaticIndependentZones) {
    KeyboardState state{
        .mode = Mode::Static,
        .speed = 0,
        .brightness = 80,
        .direction = Direction::RightToLeft,
        .color = {255, 255, 255},
        .zones_linked = false,
        .zones = {
            {ZoneId{1}, {255, 0, 0}},
            {ZoneId{2}, {0, 255, 0}},
            {ZoneId{3}, {0, 0, 255}},
            {ZoneId{4}, {255, 255, 0}},
        },
    };

    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());

    ASSERT_EQ(backend.static_writes.size(), 4u);
    EXPECT_EQ(backend.static_writes[0], (std::array<uint8_t, 4>{0b0001, 255, 0, 0}));
    EXPECT_EQ(backend.static_writes[1], (std::array<uint8_t, 4>{0b0010, 0, 255, 0}));
    EXPECT_EQ(backend.static_writes[2], (std::array<uint8_t, 4>{0b0100, 0, 0, 255}));
    EXPECT_EQ(backend.static_writes[3], (std::array<uint8_t, 4>{0b1000, 255, 255, 0}));

    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{0, 0, 80, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyStaticLinkedZones) {
    auto state = static_state_linked(60, {128, 64, 192});

    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());

    ASSERT_EQ(backend.static_writes.size(), 4u);
    for (const auto& write : backend.static_writes) {
        EXPECT_EQ(write[1], 128);
        EXPECT_EQ(write[2], 64);
        EXPECT_EQ(write[3], 192);
    }
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{0, 0, 60, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyWave) {
    KeyboardState state{.mode = Mode::Wave, .speed = 5, .brightness = 100, .direction = Direction::RightToLeft, .color = {255, 0, 0}, .zones_linked = false, .zones = {}};
    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());
    EXPECT_TRUE(backend.static_writes.empty());
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{3, 5, 100, 8, 1, 255, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyBreath) {
    KeyboardState state{.mode = Mode::Breath, .speed = 3, .brightness = 90, .direction = Direction::LeftToRight, .color = {0, 255, 0}, .zones_linked = false, .zones = {}};
    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{1, 3, 90, 0, 2, 0, 255, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyNeon) {
    KeyboardState state{.mode = Mode::Neon, .speed = 4, .brightness = 85, .direction = Direction::RightToLeft, .color = {255, 255, 255}, .zones_linked = false, .zones = {}};
    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{2, 4, 85, 0, 1, 255, 255, 255, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyShifting) {
    KeyboardState state{.mode = Mode::Shifting, .speed = 6, .brightness = 75, .direction = Direction::LeftToRight, .color = {255, 128, 0}, .zones_linked = false, .zones = {}};
    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{4, 6, 75, 0, 2, 255, 128, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyZoom) {
    KeyboardState state{.mode = Mode::Zoom, .speed = 7, .brightness = 100, .direction = Direction::RightToLeft, .color = {0, 0, 255}, .zones_linked = false, .zones = {}};
    FakeBackend backend;
    ASSERT_TRUE(apply(state, backend).has_value());
    ASSERT_EQ(backend.dynamic_writes.size(), 1u);
    EXPECT_EQ(backend.dynamic_writes[0], (std::array<uint8_t, 16>{5, 7, 100, 0, 1, 0, 0, 255, 0, 1, 0, 0, 0, 0, 0, 0}));
}

TEST(KeyboardTest, ApplyPropagatesBackendError) {
    KeyboardState state{.mode = Mode::Static, .speed = 0, .brightness = 80, .direction = Direction::RightToLeft, .color = {255, 0, 0}, .zones_linked = false, .zones = {{ZoneId{1}, {255, 0, 0}}}};
    FailingBackend backend;
    auto result = apply(state, backend);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().kind, KeyboardError::DeviceNotFound);
}

TEST(KeyboardTest, DefaultStateHasFourWhiteZones) {
    auto state = KeyboardState::default_state();
    EXPECT_EQ(state.mode, Mode::Static);
    EXPECT_EQ(state.speed, 4);
    EXPECT_EQ(state.brightness, 100);
    EXPECT_EQ(state.direction, Direction::RightToLeft);
    EXPECT_EQ(state.color, (Color{255, 255, 255}));
    EXPECT_TRUE(state.zones_linked);
    ASSERT_EQ(state.zones.size(), 4u);
    for (const auto& zone : state.zones) {
        EXPECT_EQ(zone.color, (Color{255, 255, 255}));
    }
}
