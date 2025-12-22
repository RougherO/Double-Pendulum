#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "physics.hpp"

namespace Pendulum {
namespace Constants {
    inline constexpr float default_length1 = 50.f;
    inline constexpr float default_length2 = 50.f;

    inline constexpr float default_mass1 = 20.f;
    inline constexpr float default_mass2 = 20.f;

    inline constexpr float default_angle1 = std::numbers::pi_v<float> / 6; // 30-degree
    inline constexpr float default_angle2 = std::numbers::pi_v<float> / 4; // 45-degree

    // to scale the mass at a lower rate than radii
    inline constexpr float mass_to_radius_ratio = 4;
}
struct PendulumState {
    float pos_x {}, pos_y {};
    float length1 {}, length2 {};
    float mass1 {}, mass2 {};

    float angular_vel1 {}, angular_vel2 {};
    float angular_acc1 {}, angular_acc2 {};
    float curr_angle1 {}, curr_angle2 {};
};

struct PendulumSprite final : sf::Drawable {
    void draw(sf::RenderTarget& target, sf::RenderStates) const final;

    sf::RectangleShape rod1, rod2;
    sf::CircleShape bob1, bob2;
};

void init_state(PendulumState& state);
void init_sprite(PendulumSprite& sprite, PendulumState const& state);

void update_state_on_transition(PendulumState& state, Physics::PhysicsSolver auto const& solver,
                                Physics::Environment env, float dt)
{
    solver(state, env, dt);
}

void update_sprite_on_transition(PendulumSprite& sprite, PendulumState const& state);
}