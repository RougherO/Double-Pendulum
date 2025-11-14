#pragma once
#include <utility>

struct Pendulum;
struct Physics {
    float gravity {};
    float damp {};

    template <typename Derived>
    void update(this Derived&& self, Pendulum& p, float dt) { std::forward<Derived>(self)(p, dt); }
};

struct ModifiedVerlet : Physics {
    void operator()(Pendulum& p, float dt);

private:
    auto m_calculate_angular_acc(Pendulum const&) -> std::pair<float, float>;
};

struct RungeKutta : Physics {
    void operator()(Pendulum& p, float dt);
};