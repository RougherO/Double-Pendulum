#pragma once
#include "../DoublePendulum/DPendulum.hpp"
#include "SFML/Graphics.hpp"

class App {
public:
    App();
    void run();
    ~App();

private:
    void m_handleEvents();
    void m_update();
    void m_render();

    struct Defaults {
        inline uint16_t static constexpr FRAME_RATE { 150 };
        inline uint16_t static constexpr WIN_WIDTH { 800 };
        inline uint16_t static constexpr WIN_HEIGHT { 600 };
        inline float static constexpr DELTA { 1 / 15.F };
    };

    DPendulum m_pend;
    sf::RenderWindow m_window { sf::VideoMode(Defaults::WIN_WIDTH, Defaults::WIN_HEIGHT), "Double Pendulum" };
    sf::Event m_event {};
    sf::Clock m_clock {};
    bool paused {};
};