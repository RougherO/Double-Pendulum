#pragma once
#include "physics.hpp"
#include "pendulum.hpp"

namespace Updater {
void update_state(Pendulum::PendulumState& state, Physics::PhysicsSolver auto solver,
                  Physics::Environment env, float dt)
{
    solver(env, state, dt);
}
void update_sprite(Pendulum::PendulumSprite& sprite, Pendulum::PendulumState const& state);
}