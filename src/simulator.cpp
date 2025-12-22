#include <ranges>
#include <algorithm>

#include "hud.hpp"
#include "simulator.hpp"
#include "updater.hpp"

namespace Simulator {
void init()
{
    using namespace Constants;

    system.window = {
        sf::VideoMode { { window_width, window_height } },
        window_title,
    };
    // starting with single pendulum
    system.states.emplace_back();
    system.sprites.emplace_back();
    system.environ = {
        .gravity = Physics::Constants::default_gravity,
        .damp    = Physics::Constants::default_damp,
    };

    std::ranges::for_each(std::views::zip(system.states, system.sprites), [](auto&& elem) {
        auto& [state, sprite] = elem;

        // NOLINTBEGIN(bugprone-integer-division)
        state.pos_x = window_width / 2;
        state.pos_y = window_height / 2;
        // NOLINTEND(bugprone-integer-division)

        Pendulum::init(state, sprite);
    });

    ImGui::SFML::Init(system.window);
}

void start()
{
    Core::usize curr_id {};
    Pendulum::PendulumState buffer_pend_state { system.states[curr_id] };
    Physics::Environment buffer_env_state { system.environ };

    Physics::ModifiedVerlet constexpr solver {};

    float accumulator {};
    sf::Clock clock;

    while (system.window.isOpen()) {
        while (auto const event = system.window.pollEvent()) {
            ImGui::SFML::ProcessEvent(system.window, *event);

            if (event->is<sf::Event::Closed>()) {
                system.window.close();
            } else if (auto resize_event = event->getIf<sf::Event::Resized>()) {
                auto const window_size = static_cast<sf::Vector2f>(resize_event->size);
                system.window.setView(sf::View {
                    system.window.getView().getCenter(),
                    window_size,
                });
            }
        }

        sf::Time frame_time = clock.restart();

        accumulator += frame_time.asSeconds();

        /* Core simulation update loop */ {
            using Constants::delta;
            while (accumulator >= delta) {
                // update pendulum simulation 8 times in one delta
                for (Core::usize i = 0; i != 8; i++) {
                    Updater::update_state(system.states[curr_id], solver,
                                          system.environ, delta);
                }
                accumulator -= delta;
            }
            Updater::update_sprite(system.sprites[curr_id], system.states[curr_id]);
        }

        ImGui::SFML::Update(system.window, frame_time);

        HUD::view(
            "HUD",

            [&] {
                float const x = static_cast<float>(system.window.getSize().x);
                float const l = std::min(HUD::Constants::max_hud_width, x / 4.F);

                ImGui::SetNextWindowSize({ l, 0 }, ImGuiCond_Always);
                ImGui::SetNextWindowPos({ x - l, 0.F });
            },

            HUD::EnvParamSlider {
                "gravity",
                buffer_env_state.gravity,
                HUD::Constants::min_gravity,
                HUD::Constants::max_gravity,
            },
            HUD::EnvParamSlider {
                "damp",
                buffer_env_state.damp,
                HUD::Constants::min_damp,
                HUD::Constants::max_damp,
            },

            // Rod 1
            HUD::Text { "Rod_1" },
            HUD::MassSlider {
                "mass##1",
                buffer_pend_state.mass1,
            },
            HUD::LengthSlider {
                "length##1",
                buffer_pend_state.length1,
            },
            HUD::AngleSlider {
                "angle##1",
                buffer_pend_state.curr_angle1,
            },

            // Rod 2
            HUD::Text { "Rod_2" },
            HUD::MassSlider {
                "mass##2",
                buffer_pend_state.mass2,
            },
            HUD::LengthSlider {
                "length##2",
                buffer_pend_state.length2,
            },
            HUD::AngleSlider {
                "angle##2",
                buffer_pend_state.curr_angle2,
            },
            HUD::Button {
                "Reset State",
                [&] {
                    auto& env    = system.environ;
                    auto& state  = system.states[curr_id];
                    auto& sprite = system.sprites[curr_id];

                    env   = buffer_env_state;
                    state = buffer_pend_state;

                    float radius = state.mass1 / Pendulum::Constants::mass_to_radius_ratio;
                    sprite.bob1.setRadius(radius);
                    sprite.bob1.setOrigin({ radius, radius });

                    float const w1 = sprite.rod1.getSize().x;
                    sprite.rod1.setSize({ w1, state.length1 });

                    radius = state.mass2 / Pendulum::Constants::mass_to_radius_ratio;
                    sprite.bob2.setRadius(radius);
                    sprite.bob2.setOrigin({ radius, radius });

                    float const w2 = sprite.rod2.getSize().x;
                    sprite.rod2.setSize({ w2, state.length2 });
                },
            });

        system.window.clear();
        std::ranges::for_each(system.sprites, [](auto const& sprite) {
            system.window.draw(sprite);
        });

        ImGui::SFML::Render(system.window);

        system.window.display();
    }
}

void destroy()
{
    ImGui::SFML::Shutdown();
}
}