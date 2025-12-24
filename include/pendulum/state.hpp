#pragma once
#include "core/constants.hpp"
#include "physics/physics.hpp"

namespace Pendulum {
namespace Constants {
    inline constexpr float default_length1 = 50.f;
    inline constexpr float default_length2 = 50.f;

    inline constexpr float default_mass1 = 20.f;
    inline constexpr float default_mass2 = 20.f;

    inline constexpr float default_angle1 = Core::Constants::pi / 6; // 30-degree
    inline constexpr float default_angle2 = Core::Constants::pi / 4; // 45-degree

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

void init_state(PendulumState& state);

void update_state_on_transition(PendulumState& state, Physics::PhysicsSolver auto const& solver,
                                Physics::Environment env, float dt)
{
    solver(state, env, dt);
}
}