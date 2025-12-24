#pragma once
#include "physics.hpp"

namespace Physics {
struct ModifiedVerlet {
    void operator()(Pendulum::PendulumState& state, Environment env, float dt) const;

private:
    auto m_calculate_angular_acc(Environment env, Pendulum::PendulumState const&) const -> std::pair<float, float>;
};
}