#pragma once
#include <span>
#include <array>
#include <cmath>
#include <vector>

#include "SFML/Graphics/Vertex.hpp"

#include "core/constants.hpp"
#include "core/types.hpp"

namespace Pendulum {
struct PendulumState;

// TODO (C++26): make this consteval and use for constexpr initialization
template <Core::usize point_count>
auto constexpr get_unit_vecs()
{
    using std::sin, std::cos;

    auto constexpr t_pi = 2 * Core::Constants::pi;

    std::array<sf::Vector2f, point_count> points;
    for (Core::usize i = 0; i != point_count; i++) {
        auto const theta = t_pi * static_cast<float>(i) / point_count;

        points[i] = { cos(theta), sin(theta) };
    }

    return points;
}

namespace Constants {
    inline Core::usize constexpr rod_poly_count { 2 };
    inline Core::usize constexpr bob_poly_count { 8 };
    inline std::array<sf::Vector2f, bob_poly_count> const unit_vecs { get_unit_vecs<bob_poly_count>() };
}

using VertexVector = std::vector<sf::Vertex>;

struct PendulumSpriteArray {
    VertexVector rods; // PrimitiveType = Lines
    VertexVector bobs; // PrimitiveType = Points
};

void init_sprite_with_state(std::span<sf::Vertex, 2 * Constants::rod_poly_count> rod_points,
                            std::span<sf::Vertex, 2 * Constants::bob_poly_count> bob_points,
                            Pendulum::PendulumState const& state);

void push_sprite_with_state(PendulumSpriteArray& sprites, Pendulum::PendulumState const& state);

void update_sprite_with_state(Core::usize index,
                              PendulumSpriteArray& sprites,
                              Pendulum::PendulumState const& state);
}