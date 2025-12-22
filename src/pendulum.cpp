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

void init(PendulumState& state, PendulumSprite& sprite)
{
    using namespace Constants;

    state.length1     = default_length1;
    state.length2     = default_length2;
    state.mass1       = default_mass1;
    state.mass2       = default_mass2;
    state.curr_angle1 = default_angle1;
    state.curr_angle2 = default_angle2;

    sprite.bob1.setRadius(state.mass1 / Constants::mass_to_radius_ratio);
    sprite.bob1.setOrigin({ sprite.bob1.getRadius(), sprite.bob1.getRadius() });
    sprite.bob2.setRadius(state.mass2 / Constants::mass_to_radius_ratio);
    sprite.bob2.setOrigin({ sprite.bob2.getRadius(), sprite.bob2.getRadius() });
    sprite.rod1.setSize({ 1, state.length1 });
    sprite.rod2.setSize({ 1, state.length2 });
}
}