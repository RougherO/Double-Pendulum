#include <cmath>

#include "physics/modified_verlet.hpp"
#include "pendulum/state.hpp"

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

    float const a1 = state.curr_angle1;
    float const a2 = state.curr_angle2;
    float const m1 = state.mass1;
    float const m2 = state.mass2;
    float const l1 = state.length1;
    float const l2 = state.length2;
    float const w1 = state.angular_vel1;
    float const w2 = state.angular_vel2;

    float const delta = a1 - a2;

    float const sin_delta = sin(delta);
    float const cos_delta = cos(delta);

    float const num1_1 = -env.gravity * (2 * m1 + m2) * sin(a1);
    float const num2_1 = m2 * env.gravity * sin(a1 - 2.F * a2);
    float const num3_1 = 2 * sin_delta * m2;
    float const num4_1 = w2 * w2 * l2 + w1 * w1 * l1 * cos_delta;

    float const den = (2 * m1 + m2 - m2 * cos(2 * delta));

    float const num1_2 = 2 * sin_delta;
    float const num2_2 = w1 * w1 * l1 * (m1 + m2);
    float const num3_2 = env.gravity * (m1 + m2) * cos(a1);
    float const num4_2 = w2 * w2 * l2 * m2 * cos_delta;

    return {
        (num1_1 - num2_1 - num3_1 * num4_1) / (l1 * den) - env.damp * w1,
        (num1_2 * (num2_2 + num3_2 + num4_2)) / (l2 * den) - env.damp * w2,
    };
}
}