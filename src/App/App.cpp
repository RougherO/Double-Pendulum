#include "App.hpp"

App::App()
{
    m_window = new sf::RenderWindow(sf::VideoMode(m_windowLength, m_windowHeight), sf::String("Double Pendulum Simulation"), sf::Style::Default);
    m_window->setFramerateLimit(120);
}

App::~App()
{
    delete m_window;
}

void App::run()
{
    // Initial configuration of object
    m_dpendulum.setLength1(100.);
    m_dpendulum.setLength2(100.);
    m_dpendulum.setAngle1(90.);
    m_dpendulum.setAngle2(90.);
    m_dpendulum.setPosition(m_windowLength / 2, m_windowHeight / 2);
    m_dpendulum.enableTrail(1);

    while (m_window->isOpen()) {
        m_frameTime = m_clock.restart();
        m_handleEvents();
        if (!pause) {
            m_update();
        }
        m_render();
    }
}

void App::m_handleEvents()
{
    // Event handling
    while (m_window->pollEvent(m_eventHandler)) {
        switch (m_eventHandler.type) {
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::KeyPressed:
            if (m_eventHandler.key.code == sf::Keyboard::Space) {
                pause = !pause;
            }
            break;
        default:
            break;
        }
    }
}

void App::m_updateLogic()
{
    // Update logic
    m_dpendulum.update(m_frameTime.asSeconds());
    // m_frameTime -= m_dt;
}

void App::m_updateUI()
{
}

void App::m_update()
{
    m_updateLogic();
    m_updateUI();
}

void App::m_render()
{
    // By default clears previous frame with black color
    m_window->clear();

    m_window->draw(m_dpendulum);

    m_window->display();
}