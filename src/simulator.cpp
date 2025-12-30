#include <ranges>
#include <vector>
#include <cmath>
#include <algorithm>

#include "physics/modified_verlet.hpp"
#include "data_structs/ring_buffer.hpp"
#include "ui/hud.hpp"
#include "simulator.hpp"

namespace Simulator {
void init()
{
    using namespace Constants;

    system.window = {
        sf::VideoMode { { window_width, window_height } },
        window_title,
    };

    // starting with single pendulum
    system.states.resize(1);

    system.environ = {
        .gravity = Physics::Constants::default_gravity,
        .damp    = Physics::Constants::default_damp,
    };

    std::ranges::for_each(system.states, [&](auto& state) {
        Pendulum::init_state(state);

        // NOLINTBEGIN(bugprone-integer-division)
        state.pos_x = window_width / 2;
        state.pos_y = window_height / 2;
        // NOLINTEND(bugprone-integer-division)

        Pendulum::push_sprite_with_state(system.sprites, state);
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

    DataStructs::RingBuffer<float, Constants::max_last_frame_time_count> rbuf; // for frame rate calculation

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
        rbuf.push_back(frame_time.asSeconds());
        accumulator += frame_time.asSeconds();

        int deltas;
        accumulator = std::remquo(accumulator, Constants::delta, &deltas);

        std::ranges::for_each(system.states | std::views::enumerate,
                              [&](auto&& elem) mutable {
                                  auto& [index, state] = elem;

                                  using Constants::delta;
                                  /* Core simulation update loop */
                                  for (int delta_count = 0; delta_count != deltas; delta_count++) { // NOTE: using int type because of type of `deltas`
                                      // update pendulum simulation 8 times in one delta
                                      for (Core::usize iter = 0; iter != Constants::iters; iter++) {
                                          Pendulum::update_state_on_transition(state, solver, system.environ, delta);
                                      }
                                  }
                                  Pendulum::update_sprite_with_state(index, system.sprites, state);
                              });

        ImGui::SFML::Update(system.window, frame_time);

        ImGui::ShowDemoWindow();

        UI::HUD::view(
            "HUD",

            [&] {
                float const x = static_cast<float>(system.window.getSize().x);
                float const l = std::min(UI::HUD::Constants::max_hud_width, x / 4.F);

                ImGui::SetNextWindowSize({ l, 0 }, ImGuiCond_Always);
                ImGui::SetNextWindowPos({ x - l, 0.F });
            },

            [&] {
                using namespace UI::Components;

                float const frame_rate = static_cast<float>(Constants::max_last_frame_time_count) / rbuf.reduce(std::plus<float> {});
                text("FPS: %.2f", frame_rate);
                text("states: %zu", std::size(system.states));

                slider("select", curr_id, 0, std::size(system.states) - 1);

                slider("gravity",
                       buffer_env_state.gravity,
                       UI::HUD::Constants::min_gravity,
                       UI::HUD::Constants::max_gravity);

                slider("damp",
                       buffer_env_state.damp,
                       UI::HUD::Constants::min_damp,
                       UI::HUD::Constants::max_damp);

                // Rod 1
                text("%s", "Rod_1");
                UI::HUD::mass_slider("mass##1", buffer_pend_state.mass1);
                UI::HUD::length_slider("length##1", buffer_pend_state.length1);
                UI::HUD::angle_slider("angle##1", buffer_pend_state.curr_angle1);

                // Rod 2
                text("%s", "Rod_2");
                UI::HUD::mass_slider("mass##2", buffer_pend_state.mass2);
                UI::HUD::length_slider("length##2", buffer_pend_state.length2);
                UI::HUD::angle_slider("angle##2", buffer_pend_state.curr_angle2);

                horizontal_layout(
                    [&] {
                        button("Reset State", [&] {
                            auto& env    = system.environ;
                            auto& state  = system.states[curr_id];

                            env   = buffer_env_state;
                            state = buffer_pend_state;

                            Pendulum::update_sprite_with_state(curr_id, system.sprites, state);
                        });
                    },
                    [&] {
                        button("Add State", [&] {
                            system.states.emplace_back(buffer_pend_state);

                            Pendulum::push_sprite_with_state(system.sprites, system.states.back());
                        });
                    });
            });

        system.window.clear();

        system.window.draw(system.sprites.rods.data(), system.sprites.rods.size(), sf::PrimitiveType::Lines);
        system.window.draw(system.sprites.bobs.data(), system.sprites.bobs.size(), sf::PrimitiveType::Points);

        ImGui::SFML::Render(system.window);

        system.window.display();
    }
}

void destroy()
{
    ImGui::SFML::Shutdown();
}
}