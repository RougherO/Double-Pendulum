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
    t(std::declval<Environment>(), std::declval<Pendulum::PendulumState&>(), 0.F);
};

struct Environment {
    float gravity;
    float damp;
};

struct ModifiedVerlet {
    void operator()(Environment env, Pendulum::PendulumState& state, float dt) const;

private:
    auto m_calculate_angular_acc(Environment env, Pendulum::PendulumState const&) const -> std::pair<float, float>;
};

struct RungeKutta {
    void operator()(Environment env, Pendulum::PendulumState& state, float dt) const;
};
}