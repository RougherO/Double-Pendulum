#pragma once
#include "SFML/Graphics.hpp"
#include "../DoublePendulum/DPendulum.hpp"
#include <string_view>

class App {
public:
    App();
    void run();
    ~App();

    App(App const&)                    = delete;
    auto operator=(App const&) -> App& = delete;
    App(App&&)                         = delete;
    auto operator=(App&&) -> App&      = delete;

private:
    void m_handleEvents();
    void m_update();
    void m_draw_ui();
    void m_render();

    struct Defaults {
        inline uint16_t static constexpr FRAME_RATE { 150 };
        inline uint16_t static constexpr WIN_WIDTH { 800 };
        inline uint16_t static constexpr WIN_HEIGHT { 600 };
        inline uint16_t static constexpr MENU_WIDTH { 400 };
        inline float static constexpr DELTA { 1 / 15.F };
        inline float static constexpr FONT_SIZE { 15. };
    };

    DPendulum m_pend;
    sf::RenderWindow m_window;
    sf::Event m_event {};
    sf::Clock m_clock {};
    std::string_view m_description {
        "This is an approximate visualization of a double pendulum using modified Velocity Verlet algorithm. All values that cause a state change(marked with *) in the simulation have purposefully"
        " been made to reset the entire state of the simulation (acceleration, velocity -> 0). This is done so that any change in the state(mass, length etc.) does not"
        " make the simulation blow up unexpectedly.\n\n* All values in sliders can be manually adjusted by clicking CTRL+LEFT.\n\n* Press Space to pause\n\n"
    };
};