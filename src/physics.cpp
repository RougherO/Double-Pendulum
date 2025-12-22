#include <cmath>

#include "physics.hpp"
#include "pendulum.hpp"

namespace Physics {
void ModifiedVerlet::operator()(Pendulum::PendulumState& state, Environment env, float dt) const
{
    // Modified Velocity Verlet Solver when acceleration is dependent on velocity
    // along with position.

    // Source: https://gamedev.stackexchange.com/a/41917
    state.curr_angle1 += state.angular_vel1 * dt + 0.5F * state.angular_acc1 * dt * dt;
    state.curr_angle2 += state.angular_vel2 * dt + 0.5F * state.angular_acc2 * dt * dt;

    // calculate half-step velocity
    state.angular_vel1 += state.angular_acc1 * dt;
    state.angular_vel2 += state.angular_acc2 * dt;

    // use half-step to calculate new acceleration
    auto [new_acc1, new_acc2] = m_calculate_angular_acc(env, state);

    // use new acceleration and update velocity
    state.angular_vel1 += 0.5F * (new_acc1 - state.angular_acc1) * dt;
    state.angular_vel2 += 0.5F * (new_acc2 - state.angular_acc2) * dt;

    state.angular_acc1 = new_acc1;
    state.angular_acc2 = new_acc2;
}

auto ModifiedVerlet::m_calculate_angular_acc(Environment env, Pendulum::PendulumState const& state) const -> std::pair<float, float>
{
    using std::sin, std::cos;

    float delta = state.curr_angle1 - state.curr_angle2;

    float num1_1 = -env.gravity * (2 * state.mass1 + state.mass2) * sin(state.curr_angle1);
    float num2_1 = state.mass2 * env.gravity * sin(state.curr_angle1 - 2 * state.curr_angle2);
    float num3_1 = 2 * sin(delta) * state.mass2;
    float num4_1 = state.angular_vel2 * state.angular_vel2 * state.length2 + state.angular_vel1 * state.angular_vel1 * state.length1 * cos(delta);
    float den    = (2 * state.mass1 + state.mass2 - state.mass2 * cos(2 * delta));

    float num1_2 = 2 * sin(delta);
    float num2_2 = state.angular_vel1 * state.angular_vel1 * state.length1 * (state.mass1 + state.mass2);
    float num3_2 = env.gravity * (state.mass1 + state.mass2) * cos(state.curr_angle1);
    float num4_2 = state.angular_vel2 * state.angular_vel2 * state.length2 * state.mass2 * cos(delta);

    return {
        (num1_1 - num2_1 - num3_1 * num4_1) / (state.length1 * den) - env.damp * state.angular_vel1,
        (num1_2 * (num2_2 + num3_2 + num4_2)) / (state.length2 * den) - env.damp * state.angular_vel2,
    };
}
}