#include <cmath>

#include "pendulum.hpp"
#include "physics.hpp"

void ModifiedVerlet::operator()(Pendulum& p, float dt)
{
    // Modified Velocity Verlet Solver when acceleration is dependent on velocity
    // along with position.

    // Source: https://gamedev.stackexchange.com/a/41917
    p.curr_angle1 += p.angular_vel1 * dt + 0.5F * p.angular_acc1 * dt * dt;
    p.curr_angle2 += p.angular_vel2 * dt + 0.5F * p.angular_acc2 * dt * dt;

    // calculate half-step velocity
    p.angular_vel1 += p.angular_acc1 * dt;
    p.angular_vel2 += p.angular_acc2 * dt;

    // use half-step to calculate new acceleration
    auto [new_acc1, new_acc2] = m_calculate_angular_acc(p);

    // use new acceleration and update velocity
    p.angular_vel1 += 0.5F * (new_acc1 - p.angular_acc1) * dt;
    p.angular_vel2 += 0.5F * (new_acc2 - p.angular_acc2) * dt;

    p.angular_acc1 = new_acc1;
    p.angular_acc2 = new_acc2;
}

auto ModifiedVerlet::m_calculate_angular_acc(Pendulum const& p) -> std::pair<float, float>
{
    using std::sin, std::cos;

    float delta = p.curr_angle1 - p.curr_angle2;

    float num1_1 = -gravity * (2 * p.mass1 + p.mass2) * sin(p.curr_angle1);
    float num2_1 = p.mass2 * gravity * sin(p.curr_angle1 - 2 * p.curr_angle2);
    float num3_1 = 2 * sin(delta) * p.mass2;
    float num4_1 = p.angular_vel2 * p.angular_vel2 * p.length2 + p.angular_vel1 * p.angular_vel1 * p.length1 * cos(delta);
    float den    = (2 * p.mass1 + p.mass2 - p.mass2 * cos(2 * delta));

    float num1_2 = 2 * sin(delta);
    float num2_2 = p.angular_vel1 * p.angular_vel1 * p.length1 * (p.mass1 + p.mass2);
    float num3_2 = gravity * (p.mass1 + p.mass2) * cos(p.curr_angle1);
    float num4_2 = p.angular_vel2 * p.angular_vel2 * p.length2 * p.mass2 * cos(delta);

    return {
        (num1_1 - num2_1 - num3_1 * num4_1) / (p.length1 * den) - damp * p.angular_vel1,
        (num1_2 * (num2_2 + num3_2 + num4_2)) / (p.length2 * den) - damp * p.angular_vel2,
    };
}