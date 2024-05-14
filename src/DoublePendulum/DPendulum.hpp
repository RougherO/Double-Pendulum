#pragma once
#include "SFML/Graphics.hpp"
#include <array>
#include <numbers>

class DPendulum
    : public sf::Drawable {
public:
    DPendulum() noexcept;

    void position(sf::Vector2f const& coords) noexcept;
    void position_rod1(sf::Vector2f const& coords) noexcept;
    void position_bob1(sf::Vector2f const& rod_coords) noexcept;
    void position_rod2(sf::Vector2f const& coords) noexcept;
    void position_bob2(sf::Vector2f const& rod_coords) noexcept;
    void set_mass1(float mass1) noexcept;
    void set_mass2(float mass2) noexcept;
    void set_length1(float length1) noexcept;
    void set_length2(float length2) noexcept;
    void set_angle1(float angle1) noexcept;
    void set_angle2(float angle2) noexcept;
    void set_gravity(float gravity) noexcept;
    void pause(bool val) noexcept;
    auto pause() const noexcept -> bool;
    void update(float delta) noexcept;
    void render_imgui();

    // Set of all default parameters for the pendulum
    class Defaults {
        inline float static constexpr PI = std::numbers::pi_v<float>;

    public:
        inline float static constexpr MASS1 { 70. };
        inline float static constexpr MASS2 { 70. };
        inline float static constexpr MIN_MASS { 50. };
        inline float static constexpr MAX_MASS { 300. };
        inline float static constexpr LENGTH1 { 50. };
        inline float static constexpr LENGTH2 { 50. };
        inline float static constexpr MIN_LENGTH { 10. };
        inline float static constexpr MAX_LENGTH { 200. };
        inline float static constexpr ANGLE1 { PI / 2 };
        inline float static constexpr ANGLE2 { PI / 2 };
        inline float static constexpr MIN_ANGLE { 0. };
        inline float static constexpr MAX_ANGLE { PI * 2 };
        inline float static constexpr GRAVITY { 10. };
        inline float static constexpr MIN_GRAVITY { 0.01F };
        inline float static constexpr MAX_GRAVITY { 100 };
        inline uint16_t static constexpr TRAIL { 100 };
        inline uint16_t static constexpr MIN_TRAIL { 50 };
        inline uint16_t static constexpr MAX_TRAIL { 1000 };
        inline float static constexpr MASS_TO_RADIUS_RATIO { 10. };   // We use this ratio to scale down object's mass and use it as radius to
                                                                      // indicate increased mass -> increased radius
        inline uint16_t static constexpr MAX_TRAIL_COLOR_LENGTH { 30 };
    };

private:
    float m_gravity { Defaults::GRAVITY };
    float m_a_vel1 {};
    float m_a_vel2 {};
    float m_a_acc1 {};
    float m_a_acc2 {};
    float m_initial_angle1 { Defaults::ANGLE1 };
    float m_initial_angle2 { Defaults::ANGLE2 };
    float m_curr_angle1 { m_initial_angle1 };
    float m_curr_angle2 { m_initial_angle2 };
    float m_damp_coeff {};
    float m_mass1 { Defaults::MASS1 };
    float m_mass2 { Defaults::MASS2 };
    float m_length1 { Defaults::LENGTH1 };
    float m_length2 { Defaults::LENGTH2 };
    float m_x_coord {};
    float m_y_coord {};
    bool m_pause {};
    bool m_pause_req {};   // pause_req is used to check if any internal component is requesting to pause the updation of state
    bool m_trail_enable {};
    bool m_trail_multi_color {};
    uint16_t m_trail_length { Defaults::TRAIL };
    std::array<float, 4> m_trail_color_picker { 1., 1., 1. };

    sf::RectangleShape m_rod1 {
        {2, m_length1}  // taking thickness of rectangle as 1 to make it look like a rod
    };
    sf::RectangleShape m_rod2 {
        {2, m_length2}
    };
    sf::CircleShape m_bob1 { Defaults::MASS1 / Defaults::MASS_TO_RADIUS_RATIO };
    sf::CircleShape m_bob2 { Defaults::MASS2 / Defaults::MASS_TO_RADIUS_RATIO };
    std::array<sf::Vertex, Defaults::MAX_TRAIL> m_trail {};

    auto m_accel_func(float a_vel1, float a_vel2) const noexcept -> std::pair<float, float>;
    void m_verlet_solve(float delta) noexcept;
    void m_update_positions() noexcept;
    void m_update_trail() noexcept;
    void m_render_mass1_imgui();
    void m_render_mass2_imgui();
    void m_render_length1_imgui();
    void m_render_length2_imgui();
    void m_render_angle1_imgui();
    void m_render_angle2_imgui();
    void m_render_gravity_imgui();
    void m_render_trail_imgui();
    void m_reset_state() noexcept;
    void m_reset_trail() noexcept;

    void draw(sf::RenderTarget& target, [[maybe_unused]] sf::RenderStates states) const override
    {
        target.draw(m_rod1);
        target.draw(m_rod2);
        target.draw(m_bob1);
        target.draw(m_bob2);
        if (m_trail_enable) {
            target.draw(m_trail.data(), m_trail_length, sf::PrimitiveType::LinesStrip);
        }
    }
};