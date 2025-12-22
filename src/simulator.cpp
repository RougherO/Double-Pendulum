#include <vector>
#include <cmath>
#include <ranges>
#include <algorithm>

#include "hud.hpp"
#include "ring_buffer.hpp"
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
    system.sprites.resize(1);
    system.environ = {
        .gravity = Physics::Constants::default_gravity,
        .damp    = Physics::Constants::default_damp,
    };

    std::ranges::for_each(std::views::zip(system.states, system.sprites), [](auto&& elem) {
        auto& [state, sprite] = elem;
        Pendulum::init_state(state);

        // NOLINTBEGIN(bugprone-integer-division)
        state.pos_x = window_width / 2;
        state.pos_y = window_height / 2;
        // NOLINTEND(bugprone-integer-division)

        Pendulum::init_sprite(sprite, state);
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

    DataStructures::RingBuffer<float, Constants::max_last_frame_time_count> rbuf; // for frame rate calculation

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
        std::ranges::for_each(std::views::zip(system.states, system.sprites), [&](auto&& elem) {
            auto& [state, sprite] = elem;

            using Constants::delta;
            /* Core simulation update loop */
            for (int delta_count = 0; delta_count != deltas; delta_count++) { // NOTE: using int type because of type of `deltas`
                // update pendulum simulation 8 times in one delta
                for (Core::usize iter = 0; iter != Constants::iters; iter++) {
                    Pendulum::update_state_on_transition(state, solver, system.environ, delta);
                }
            }
            Pendulum::update_sprite_on_transition(sprite, state);
        });

        ImGui::SFML::Update(system.window, frame_time);

        HUD::view(
            "HUD",

            [&] {
                float const x = static_cast<float>(system.window.getSize().x);
                float const l = std::min(HUD::Constants::max_hud_width, x / 4.F);

                ImGui::SetNextWindowSize({ l, 0 }, ImGuiCond_Always);
                ImGui::SetNextWindowPos({ x - l, 0.F });
            },

            [&] {
                using namespace Componenets;

                float const frame_rate = static_cast<float>(Constants::max_last_frame_time_count) / rbuf.reduce(std::plus<float> {});
                text(std::format("FPS: {:.2f}", frame_rate).c_str());

                slider("gravity",
                       buffer_env_state.gravity,
                       HUD::Constants::min_gravity,
                       HUD::Constants::max_gravity);

                slider("damp",
                       buffer_env_state.damp,
                       HUD::Constants::min_damp,
                       HUD::Constants::max_damp);

                // Rod 1
                text("Rod_1");
                HUD::mass_slider("mass##1", buffer_pend_state.mass1);
                HUD::length_slider("length##1", buffer_pend_state.length1);
                HUD::angle_slider("angle##1", buffer_pend_state.curr_angle1);

                // Rod 2
                text("Rod_2");
                HUD::mass_slider("mass##2", buffer_pend_state.mass2);
                HUD::length_slider("length##2", buffer_pend_state.length2);
                HUD::angle_slider("angle##2", buffer_pend_state.curr_angle2);

                horizontal_layout(
                    [&] {
                        button("Reset State", [&] {
                            auto& env    = system.environ;
                            auto& state  = system.states[curr_id];
                            auto& sprite = system.sprites[curr_id];

                            env   = buffer_env_state;
                            state = buffer_pend_state;

                            Pendulum::init_sprite(sprite, state);
                        });
                    },
                    [&] {
                        button("Add State", [&] {
                            system.states.emplace_back(buffer_pend_state);
                            system.sprites.emplace_back();

                            Pendulum::init_sprite(system.sprites.back(), system.states.back());
                        });
                    });
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