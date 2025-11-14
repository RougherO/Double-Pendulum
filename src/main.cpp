#include "physics.hpp"
#include "types.hpp"
#include "entity.hpp"
#include "pendulum.hpp"
#include "sprites.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

using ETTManager = EntityManager<Pendulum>;
using SPTManager = SpriteManager<PendulumSprite>;

int main()
{
    auto& entity_manager = ::ETTManager::get_instance();
    auto& sprite_manager = SPTManager::get_instance();

    sf::RenderWindow window { sf::VideoMode { { 800, 600 } },
                              "Double Pendulum Simulation" };

    auto window_view = window.getDefaultView();

    auto& pendulum       = entity_manager.create_entity<Pendulum>();
    pendulum.pos_x       = window.getSize().x / 2u;
    pendulum.pos_y       = window.getSize().y / 2u;
    pendulum.curr_angle1 = sf::degrees(90).asRadians();
    pendulum.curr_angle2 = sf::degrees(100).asRadians();
    pendulum.length1     = 25.f;
    pendulum.length2     = 25.f;
    pendulum.mass1       = 5.f;
    pendulum.mass2       = 5.f;

    auto& pendulum_sprite = sprite_manager.create_sprite<PendulumSprite>(pendulum);

    auto solver    = ModifiedVerlet {};
    solver.gravity = 9.8f;

    {
        float dt  = 0.01f;
        float acc = 0;
        sf::Clock clock;
        while (window.isOpen()) {
            window.handleEvents([&](sf::Event::Closed) { window.close(); },
                                [&](sf::Event::Resized resize_event) {
                                    window.setView(sf::View { window.getView().getCenter(),
                                                              static_cast<sf::Vector2f>(resize_event.size) });
                                });

            float frame_time = clock.restart().asSeconds();

            acc += frame_time;
            while (acc >= dt) {
                for (i32 i = 0; i != 8; i++) {
                    solver.update(pendulum, dt);
                }
                acc -= dt;
            }

            window.clear();
            window.draw(pendulum_sprite);
            window.display();
        }
    }
}