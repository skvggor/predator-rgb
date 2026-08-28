#include "core/effect.h"

#include <gtest/gtest.h>

using namespace predator;

TEST(EffectTest, ModeDiscriminants) {
    EXPECT_EQ(static_cast<uint8_t>(Mode::Static), 0);
    EXPECT_EQ(static_cast<uint8_t>(Mode::Breath), 1);
    EXPECT_EQ(static_cast<uint8_t>(Mode::Neon), 2);
    EXPECT_EQ(static_cast<uint8_t>(Mode::Wave), 3);
    EXPECT_EQ(static_cast<uint8_t>(Mode::Shifting), 4);
    EXPECT_EQ(static_cast<uint8_t>(Mode::Zoom), 5);
}

TEST(EffectTest, DirectionDiscriminants) {
    EXPECT_EQ(static_cast<uint8_t>(Direction::RightToLeft), 1);
    EXPECT_EQ(static_cast<uint8_t>(Direction::LeftToRight), 2);
}

TEST(EffectTest, ModeSerialization) {
    EXPECT_EQ(mode_to_string(Mode::Static), "static");
    EXPECT_EQ(mode_to_string(Mode::Wave), "wave");
    EXPECT_EQ(mode_to_string(Mode::Zoom), "zoom");
}

TEST(EffectTest, ModeDeserialization) {
    EXPECT_EQ(mode_from_string("static"), Mode::Static);
    EXPECT_EQ(mode_from_string("wave"), Mode::Wave);
    EXPECT_EQ(mode_from_string("zoom"), Mode::Zoom);
}

TEST(EffectTest, ModeDeserializationInvalid) {
    EXPECT_FALSE(mode_from_string("fogo").has_value());
    EXPECT_FALSE(mode_from_string("staticx").has_value());
}

TEST(EffectTest, DirectionSerialization) {
    EXPECT_EQ(direction_to_string(Direction::RightToLeft), "right-to-left");
    EXPECT_EQ(direction_to_string(Direction::LeftToRight), "left-to-right");
}

TEST(EffectTest, DirectionDeserialization) {
    EXPECT_EQ(direction_from_string("right-to-left"), Direction::RightToLeft);
    EXPECT_EQ(direction_from_string("left-to-right"), Direction::LeftToRight);
}

TEST(EffectTest, DirectionDeserializationInvalid) {
    EXPECT_FALSE(direction_from_string("up").has_value());
    EXPECT_FALSE(direction_from_string("down").has_value());
}
