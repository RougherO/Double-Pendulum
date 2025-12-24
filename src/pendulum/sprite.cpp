#include <cmath>
#include <ranges>

#include "pendulum/sprite.hpp"
#include "pendulum/state.hpp"

namespace Pendulum {
void init_sprite_with_state(std::span<sf::Vertex, 2 * Constants::rod_poly_count> rod_points,
                            std::span<sf::Vertex, 2 * Constants::bob_poly_count> bob_points,
                            PendulumState const& state)
{
    using std::sin, std::cos;
    float const sin_t1 = sin(state.curr_angle1);
    float const sin_t2 = sin(state.curr_angle2);
    float const cos_t1 = cos(state.curr_angle1);
    float const cos_t2 = cos(state.curr_angle2);

    auto const pivot = sf::Vector2f { state.pos_x, state.pos_y };

    auto const p1 = pivot + state.length1 * sf::Vector2f { sin_t1, cos_t1 };
    auto const p2 = p1 + state.length2 * sf::Vector2f { sin_t2, cos_t2 };

    rod_points[0].position = pivot;
    rod_points[1].position = p1;

    rod_points[2].position = p1;
    rod_points[3].position = p2;

    float const r1 = state.mass1 / Constants::mass_to_radius_ratio;
    float const r2 = state.mass2 / Constants::mass_to_radius_ratio;

    // updating bob1 points
    for (auto const [pidx, unit_vec] : Constants::unit_vecs | std::views::enumerate) {
        bob_points[pidx].position = p1 + r1 * unit_vec; // NOLINT(clang-diagnostic-sign-conversion)
    }

    // updating bob2 points
    for (auto const [pidx, unit_vec] : Constants::unit_vecs | std::views::enumerate) {
        bob_points[Constants::bob_poly_count + pidx].position = p2 + r2 * unit_vec; // NOLINT(clang-diagnostic-sign-conversion)
    }
}

void push_sprite_with_state(PendulumSpriteArray& sprites, Pendulum::PendulumState const& state)
{
    // rods
    sprites.rods.resize(sprites.rods.size() + 2 * Constants::rod_poly_count);

    // bobs
    sprites.bobs.resize(sprites.bobs.size() + 2 * Constants::bob_poly_count);

    init_sprite_with_state(std::span { sprites.rods }.last<2 * Constants::rod_poly_count>(),
                           std::span { sprites.bobs }.last<2 * Constants::bob_poly_count>(),
                           state);
}

void update_sprite_with_state(Core::usize index,
                              PendulumSpriteArray& sprites,
                              PendulumState const& state)
{
    init_sprite_with_state(std::span {
                               sprites.rods.begin() + index * 2 * Constants::rod_poly_count, // NOLINT(clang-diagnostic-sign-conversion,bugprone-narrowing-conversions)
                               sprites.rods.end(),
                           }
                               .first<2 * Constants::rod_poly_count>(),
                           std::span {
                               sprites.bobs.begin() + index * 2 * Constants::bob_poly_count, // NOLINT(clang-diagnostic-sign-conversion,bugprone-narrowing-conversions)
                               sprites.bobs.end(),
                           }
                               .first<2 * Constants::bob_poly_count>(),
                           state);
}

}