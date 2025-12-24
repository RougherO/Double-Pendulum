#pragma once
#include "physics.hpp"

namespace Physics {
struct RungeKutta {
    void operator()(Pendulum::PendulumState& state, Environment env, float dt) const;
};
}