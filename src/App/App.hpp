#pragma once
#include "../DoublePendulum/DPendulum.hpp"
#include "SFML/Graphics.hpp"
#include <string>

class App {
private:
    // Custom Object
    DPendulum m_dpendulum;

    // App properties
    static const unsigned short m_windowLength { 800 };
    static const unsigned short m_windowHeight { 600 };

    sf::RenderWindow* m_window {}; // Window for rendering
    sf::Event m_eventHandler {}; // Handling of events
    sf::Clock m_clock {}; // Internal Clock for the app
    sf::Time m_frameTime {}; // Time taken between two frames
    sf::Text m_text {}; // Drawable text
    sf::Font m_textFont {}; // Text Font

    bool pause {};

private:
    void m_handleEvents();
    void m_updateLogic();
    void m_updateUI();
    void m_update();
    void m_render();

public:
    void run();

public:
    App();
    ~App();
};