#include "App.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

App::App()
{
    m_window.setFramerateLimit(Defaults::FRAME_RATE);
    m_pend.position({ Defaults::WIN_WIDTH >> 1, Defaults::WIN_HEIGHT >> 1 });   // set pendulum to middle of window
    static_cast<void>(ImGui::SFML::Init(m_window));                             // static casting to void of a return value explicitly means
                                                                                // intentionally ignoring a [[nodiscard]] value
}

void App::m_handleEvents()
{
    while (m_window.pollEvent(m_event)) {
        ImGui::SFML::ProcessEvent(m_window, m_event);
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

void App::m_update()
{
    m_pend.update(Defaults::DELTA);
    ImGui::SFML::Update(m_window, m_clock.restart());
    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello");
    ImGui::Button("Cute button");
    ImGui::End();
}

void App::m_render()
{
    m_window.clear();
    ImGui::SFML::Render(m_window);
    m_window.draw(m_pend);
    m_window.display();
}

void App::run()
{
    while (m_window.isOpen()) {
        m_handleEvents();
        if (!paused) {
            m_update();
            m_render();
        }
    }
}

App::~App()
{
    ImGui::SFML::Shutdown();
}