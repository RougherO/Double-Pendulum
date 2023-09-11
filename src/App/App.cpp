#include "App.hpp"
#include <optional>
#include <vector>

App::App()
{
    m_window = new sf::RenderWindow(sf::VideoMode(m_windowLength, m_windowHeight), sf::String("Double Pendulum Simulation"), sf::Style::Default);
    m_window->setFramerateLimit(120);
}

App::~App()
{
    delete m_window;
}

void App::run(const std::vector<std::optional<double>>& vals)
{
    // Initial configuration of object
    m_dpendulum.setAngle1(vals[4].value_or(90.));
    m_dpendulum.setAngle2(vals[5].value_or(90.));
    m_dpendulum.setLength1(vals[0].value_or(70.));
    m_dpendulum.setLength2(vals[1].value_or(70.));
    m_dpendulum.setMass1(vals[2].value_or(10.));
    m_dpendulum.setMass2(vals[3].value_or(10.));
    m_dpendulum.setGravity(vals[6].value_or(0)); // 0 will be handled to use default gravity in pendulum object
    m_dpendulum.enableTrail(vals[7].has_value());
    m_dpendulum.setTrailLength(vals[7].value_or(250));
    m_dpendulum.setPosition(m_windowLength / 2, m_windowHeight / 2);

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