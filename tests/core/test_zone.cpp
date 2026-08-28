#include "core/zone.h"

#include <gtest/gtest.h>

using namespace predator;

TEST(ZoneTest, ZoneIdValid) {
    for (uint8_t value = 1; value <= 4; ++value) {
        auto zone = ZoneId::create(value);
        ASSERT_TRUE(zone.has_value());
        EXPECT_EQ(zone->value, value);
    }
}

TEST(ZoneTest, ZoneIdInvalid) {
    EXPECT_FALSE(ZoneId::create(0).has_value());
    EXPECT_FALSE(ZoneId::create(5).has_value());
    EXPECT_FALSE(ZoneId::create(255).has_value());
    EXPECT_EQ(ZoneId::create(0).error().value, 0);
    EXPECT_EQ(ZoneId::create(5).error().value, 5);
}

TEST(ZoneTest, Bitmask) {
    EXPECT_EQ(ZoneId{1}.bitmask(), 0b0001);
    EXPECT_EQ(ZoneId{2}.bitmask(), 0b0010);
    EXPECT_EQ(ZoneId{3}.bitmask(), 0b0100);
    EXPECT_EQ(ZoneId{4}.bitmask(), 0b1000);
}
