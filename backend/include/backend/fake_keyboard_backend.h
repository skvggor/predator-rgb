#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "core/keyboard.h"

namespace predator {

class FakeKeyboardBackend : public KeyboardBackend {
public:
    std::expected<void, KeyboardError> write_dynamic(std::array<uint8_t, 16> payload) override {
        dynamic_payloads.push_back(payload);
        return {};
    }
    std::expected<void, KeyboardError> write_static(std::array<uint8_t, 4> payload) override {
        static_payloads.push_back(payload);
        return {};
    }

    void clear() {
        dynamic_payloads.clear();
        static_payloads.clear();
    }

    std::vector<std::array<uint8_t, 16>> dynamic_payloads;
    std::vector<std::array<uint8_t, 4>> static_payloads;
};

} // namespace predator
