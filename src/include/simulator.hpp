#pragma once
#include "SFML/Graphics/RenderWindow.hpp"

#include "imgui-SFML.h"

#include "pendulum.hpp"
#include "physics.hpp"
#include "types.hpp"

namespace Simulator {
namespace Constants {
    // window constants
    inline constexpr Core::usize window_width { 800 };
    inline constexpr Core::usize window_height { 600 };
    inline std::string const window_title { "Double Pendulum Simulation" };

    // simulation constants
    inline constexpr float delta       = 0.01F;
    inline constexpr Core::usize iters = 8; // how many iterations of the simulation to run per frame

    // system constants
    inline constexpr Core::usize max_last_frame_time_count = 8;
}

namespace {
    struct {
        sf::RenderWindow window;
        std::vector<Pendulum::PendulumState> states;
        std::vector<Pendulum::PendulumSprite> sprites;
        Physics::Environment environ;
    } system;
}

void init();
void start();
void destroy();
}