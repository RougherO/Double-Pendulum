#include <cmath>

#include "SFML/Graphics/RenderTarget.hpp"

#include "pendulum.hpp"

namespace Pendulum {
void PendulumSprite::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    target.draw(rod1);
    target.draw(rod2);
    target.draw(bob1);
    target.draw(bob2);
}

void init_state(PendulumState& state)
{
    using namespace Constants;

    state.length1     = default_length1;
    state.length2     = default_length2;
    state.mass1       = default_mass1;
    state.mass2       = default_mass2;
    state.curr_angle1 = default_angle1;
    state.curr_angle2 = default_angle2;
}

void init_sprite(PendulumSprite& sprite, PendulumState const& state)
{
    float radius;

    radius = state.mass1 / Constants::mass_to_radius_ratio;
    sprite.bob1.setRadius(radius);
    sprite.bob1.setOrigin({ radius, radius });

    sprite.rod1.setSize({ 1, state.length1 });

    radius = state.mass2 / Constants::mass_to_radius_ratio;
    sprite.bob2.setRadius(radius);
    sprite.bob2.setOrigin({ radius, radius });

    sprite.rod2.setSize({ 1, state.length2 });
}

void update_sprite_on_transition(PendulumSprite& sprite, PendulumState const& state)
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