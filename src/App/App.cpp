#include "App.hpp"

namespace App {
namespace {
    DPendulum m_pend;
    sf::RenderWindow m_window { sf::VideoMode(Defaults::WIN_WIDTH, Defaults::WIN_HEIGHT), "Double Pendulum" };
    sf::Event m_event;
    bool paused {};
}

void init()
{
    m_window.setFramerateLimit(Defaults::FRAME_RATE);
    m_pend.position({ Defaults::WIN_WIDTH >> 1, Defaults::WIN_HEIGHT >> 1 });   // set pendulum to middle of window
}

namespace {
    void m_handleEvents()
    {
        while (m_window.pollEvent(m_event)) {
            switch (m_event.type) {
                case sf::Event::Closed:
                    m_window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (m_event.key.code == sf::Keyboard::Space) {
                        paused = !paused;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void m_update()
    {
        m_pend.update(Defaults::DELTA);
    }

    void m_render()
    {
        m_window.clear();
        m_window.draw(m_pend);
        m_window.display();
    }
}

void run()
{
    while (m_window.isOpen()) {
        m_handleEvents();
        if (!paused) {
            m_update();
            m_render();
        }
    }
}
}