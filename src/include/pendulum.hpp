#pragma once
#include "sprites.hpp"

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

struct Pendulum {
    Pendulum() = default;

    Pendulum(Pendulum const&)            = delete;
    Pendulum& operator=(Pendulum const&) = delete;

    Pendulum(Pendulum&&) noexcept            = default;
    Pendulum& operator=(Pendulum&&) noexcept = default;

    float pos_x {}, pos_y {};
    float angular_vel1 {}, angular_vel2 {};
    float angular_acc1 {}, angular_acc2 {};
    float curr_angle1 {}, curr_angle2 {};
    float length1 {}, length2 {};
    float mass1 {}, mass2 {};
};

struct PendulumSprite final : Sprite<Pendulum> {
    PendulumSprite(Pendulum const& p)
        : Sprite { p }
        , rod1 { { 1, p.length1 } }
        , rod2 { { 1, p.length2 } }
        , bob1 { p.mass1 }
        , bob2 { p.mass2 }
    {
        auto const r1 = bob1.getRadius();
        auto const r2 = bob2.getRadius();

        bob1.setOrigin({ r1, r1 });
        bob2.setOrigin({ r2, r2 });
    }

    void draw(sf::RenderTarget& target, sf::RenderStates) const final;

    mutable sf::RectangleShape rod1, rod2;
    mutable sf::CircleShape bob1, bob2;
};