#include "App.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

App::App()
    : m_window { sf::VideoMode(Defaults::WIN_WIDTH + Defaults::MENU_WIDTH, Defaults::WIN_HEIGHT), "Double Pendulum" }
{
    m_window.setFramerateLimit(Defaults::FRAME_RATE);

    m_pend.position({ Defaults::WIN_WIDTH >> 1, Defaults::WIN_HEIGHT >> 1 });   // set pendulum to middle of window

    static_cast<void>(ImGui::SFML::Init(m_window, false));                      // static casting to void means purposefully ignoring a return value marked as [[nodiscard]]

    // Loading fonts
    // FONT_DIR macro value is passed using CMake at compile time compile_definition
    // See CMakeList.txt for the actual value
    ImGui::GetIO().Fonts->AddFontFromFileTTF(FONT_DIR "/arial.ttf", Defaults::FONT_SIZE);
    static_cast<void>(ImGui::SFML::UpdateFontTexture());
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
                    m_pend.pause(!m_pend.pause());
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
}

void App::m_draw_ui()
{
    ImGui::Begin("Controller", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Configuring window
    ImGui::SetWindowSize({ Defaults::MENU_WIDTH, Defaults::WIN_HEIGHT });
    ImGui::SetWindowPos({ Defaults::WIN_WIDTH, 0 });

    // Using the loaded font
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

    ImGui::TextWrapped(m_description.data());

    m_pend.render_imgui();

    ImGui::PopFont();
    ImGui::End();
}

void App::m_render()
{
    m_window.clear();

    m_window.draw(m_pend);

    m_draw_ui();

    ImGui::SFML::Render(m_window);

    m_window.display();
}

void App::run()
{
    while (m_window.isOpen()) {
        m_handleEvents();
        m_update();
        m_render();
    }
}

App::~App()
{
    ImGui::SFML::Shutdown();
}