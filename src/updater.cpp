#include <cmath>

#include "updater.hpp"

namespace Updater {
void update_sprite(Pendulum::PendulumSprite& sprite, Pendulum::PendulumState const& state)
{
    using std::sin, std::cos;
    auto const sin_t1 = sin(state.curr_angle1);
    auto const sin_t2 = sin(state.curr_angle2);
    auto const cos_t1 = cos(state.curr_angle1);
    auto const cos_t2 = cos(state.curr_angle2);

    auto const rod1_pos = sf::Vector2f { state.pos_x, state.pos_y };
    auto const rod1_ang = sf::radians(state.curr_angle1);
    auto const bob1_pos = rod1_pos + state.length1 * sf::Vector2f { sin_t1, cos_t1 };

    auto const rod2_pos = bob1_pos;
    auto const rod2_ang = sf::radians(state.curr_angle2);
    auto const bob2_pos = rod2_pos + state.length2 * sf::Vector2f { sin_t2, cos_t2 };

    sprite.rod1.setPosition(rod1_pos);
    sprite.rod1.setRotation(-rod1_ang);

    sprite.bob1.setPosition(bob1_pos);

    sprite.rod2.setPosition(rod2_pos);
    sprite.rod2.setRotation(-rod2_ang);

    sprite.bob2.setPosition(bob2_pos);
}
}