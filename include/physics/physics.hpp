#pragma once
#include <utility>

namespace Pendulum {
struct PendulumState;
}

namespace Physics {
namespace Constants {
    inline constexpr float default_gravity = 9.8F;
    inline constexpr float default_damp    = 0.F;
}

struct Environment;

template <typename T>
concept PhysicsSolver = requires(T t) {
    // Expect the call operator to be overloaded to
    // take the environement, the pendulum state and
    // duration for update
    t(std::declval<Pendulum::PendulumState&>(), std::declval<Environment>(), 0.F);
};

struct Environment {
    float gravity;
    float damp;
};
}