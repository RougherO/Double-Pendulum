#pragma once
#include "../DoublePendulum/DPendulum.hpp"
#include "SFML/Graphics.hpp"

namespace App {
struct Defaults {
    inline uint16_t static constexpr FRAME_RATE { 150 };
    inline uint16_t static constexpr WIN_WIDTH { 800 };
    inline uint16_t static constexpr WIN_HEIGHT { 600 };
    inline float static constexpr DELTA { 1 / 15.F };
};

void init();
void run();
}