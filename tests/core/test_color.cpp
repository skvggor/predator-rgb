#include "core/color.h"

#include <gtest/gtest.h>

using namespace predator;

TEST(ColorTest, HsvToRgbPrimaryHues) {
    EXPECT_EQ(hsv_to_rgb({0, 1, 1}), (Color{255, 0, 0}));
    EXPECT_EQ(hsv_to_rgb({120, 1, 1}), (Color{0, 255, 0}));
    EXPECT_EQ(hsv_to_rgb({240, 1, 1}), (Color{0, 0, 255}));
}

TEST(ColorTest, HsvToRgbGrayscale) {
    EXPECT_EQ(hsv_to_rgb({0, 0, 0.5f}), (Color{128, 128, 128}));
    EXPECT_EQ(hsv_to_rgb({180, 0, 1}), (Color{255, 255, 255}));
}

TEST(ColorTest, HsvToRgbYellowCyanMagenta) {
    EXPECT_EQ(hsv_to_rgb({60, 1, 1}), (Color{255, 255, 0}));
    EXPECT_EQ(hsv_to_rgb({180, 1, 1}), (Color{0, 255, 255}));
    EXPECT_EQ(hsv_to_rgb({300, 1, 1}), (Color{255, 0, 255}));
}

TEST(ColorTest, RgbToHsvRoundtrip) {
    const Color colors[] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {128, 64, 192}};
    for (auto original : colors) {
        auto restored = hsv_to_rgb(rgb_to_hsv(original));
        EXPECT_EQ(restored, original);
    }
}

TEST(ColorTest, FromHexValid) {
    EXPECT_TRUE(Color::from_hex("#ff0000").has_value());
    EXPECT_TRUE(Color::from_hex("#00FF00").has_value());
    EXPECT_TRUE(Color::from_hex("#0000ff").has_value());
    EXPECT_TRUE(Color::from_hex("#ff8800").has_value());
}

TEST(ColorTest, FromHexInvalid) {
    EXPECT_EQ(Color::from_hex("ff0000").error(), ColorError::InvalidFormat);
    EXPECT_EQ(Color::from_hex("#ff00").error(), ColorError::InvalidLength);
    EXPECT_EQ(Color::from_hex("#ff000000").error(), ColorError::InvalidLength);
    EXPECT_EQ(Color::from_hex("#gg0000").error(), ColorError::InvalidHex);
    EXPECT_EQ(Color::from_hex("").error(), ColorError::InvalidFormat);
}

TEST(ColorTest, ToHex) {
    EXPECT_EQ((Color{255, 0, 0}.to_hex()), "#FF0000");
    EXPECT_EQ((Color{0, 255, 0}.to_hex()), "#00FF00");
    EXPECT_EQ((Color{0, 0, 255}.to_hex()), "#0000FF");
    EXPECT_EQ((Color{255, 136, 0}.to_hex()), "#FF8800");
}

TEST(ColorTest, HexRoundtrip) {
    const std::string hexes[] = {"#FF0000", "#00FF00", "#0000FF", "#FF8800", "#ABCDEF", "#123456"};
    for (const auto& hex : hexes) {
        auto color = Color::from_hex(hex);
        ASSERT_TRUE(color.has_value());
        EXPECT_EQ(color->to_hex(), hex);
    }
}

TEST(ColorTest, Clamp) {
    EXPECT_EQ(Color::clamp(-10), 0);
    EXPECT_EQ(Color::clamp(0), 0);
    EXPECT_EQ(Color::clamp(255), 255);
    EXPECT_EQ(Color::clamp(300), 255);
    EXPECT_EQ(Color::clamp(128), 128);
}
