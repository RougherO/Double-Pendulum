#include "SFML/Graphics/RenderWindow.hpp"

#include "imgui-SFML.h"

#include "physics.hpp"
#include "types.hpp"
#include "pendulum.hpp"
#include "updater.hpp"
#include "hud.hpp"

int main()
{
    sf::RenderWindow window { sf::VideoMode { { 800, 600 } },
                              "Double Pendulum Simulation" };

    Pendulum::PendulumState state, start_state;
    Pendulum::PendulumSprite sprite;
    Pendulum::init(state, sprite);

    auto [width, height] = static_cast<sf::Vector2f>(window.getSize());
    state.pos_x          = width / 2;
    state.pos_y          = height / 2;

    start_state = state;

    auto solver = Physics::ModifiedVerlet {};

    Physics::Environment env, start_env;
    env.gravity = 9.8F,
    env.damp    = 0.F;
    start_env   = env;

    ImGui::SFML::Init(window);
    float dt  = 0.01f;
    float acc = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        while (auto const event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (auto resize_event = event->getIf<sf::Event::Resized>()) {
                auto window_size = static_cast<sf::Vector2f>(resize_event->size);
                window.setView(sf::View { window.getView().getCenter(), window_size });
            }
        }

        sf::Time frame_time = clock.restart();

        acc += frame_time.asSeconds();
        while (acc >= dt) {
            // update pendulum simulation 8 times in one delta
            for (Core::usize i = 0; i != 8; i++) {
                Updater::update_state(state, solver, env, dt);
            }
            acc -= dt;
        }
        Updater::update_sprite(sprite, state);
        // update ImGui controller window to reflect changes
        ImGui::SFML::Update(window, frame_time);

        HUD::view(
            "HUD",

            [&] {
                auto const [x, y] = static_cast<sf::Vector2f>(window.getSize());
                float const l     = std::min(HUD::Constants::max_hud_width, x / 4.F);

                ImGui::SetNextWindowSize({ l, 0 }, ImGuiCond_Always);
                ImGui::SetNextWindowPos({ x - l, 0.F });
            },

            HUD::EnvParamSlider {
                "gravity",
                start_env.gravity,
                HUD::Constants::min_gravity,
                HUD::Constants::max_gravity,
            },
            HUD::EnvParamSlider {
                "damp",
                start_env.damp,
                HUD::Constants::min_damp,
                HUD::Constants::max_damp,
            },

            // Rod 1
            HUD::Text { "Rod_1" },
            HUD::MassSlider {
                "mass##1",
                start_state.mass1,
            },
            HUD::LengthSlider {
                "length##1",
                start_state.length1,
            },
            HUD::AngleSlider {
                "angle##1",
                start_state.curr_angle1,
            },

            // Rod 2
            HUD::Text { "Rod_2" },
            HUD::MassSlider {
                "mass##2",
                start_state.mass2,
            },
            HUD::LengthSlider {
                "length##2",
                start_state.length2,
            },
            HUD::AngleSlider {
                "angle##2",
                start_state.curr_angle2,
            },
            HUD::Button {
                "Reset State",
                [&] {
                    env   = start_env;
                    state = start_state;

                    float radius = state.mass1 / Pendulum::Constants::mass_to_radius_ratio;
                    sprite.bob1.setRadius(radius);
                    sprite.bob1.setOrigin({ radius, radius });

                    sprite.rod1.setSize({ 1, state.length1 });

                    radius = state.mass2 / Pendulum::Constants::mass_to_radius_ratio;
                    sprite.bob2.setRadius(radius);
                    sprite.bob2.setOrigin({ radius, radius });

                    sprite.rod2.setSize({ 1, state.length2 });
                },
            });

        window.clear();

        window.draw(sprite);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}