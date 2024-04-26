#pragma once
#include "SFML/Graphics.hpp"
#include <array>

class DPendulum
    : public sf::Drawable
    , sf::Transformable {
public:
    DPendulum();

    void position(sf::Vector2f const& coords) noexcept;
    void position_rod1(sf::Vector2f const& coords) noexcept;
    void position_bob1(sf::Vector2f const& rod_coords) noexcept;
    void position_rod2(sf::Vector2f const& coords) noexcept;
    void position_bob2(sf::Vector2f const& rod_coords) noexcept;
    void length1(float length1) noexcept;
    void length2(float length2) noexcept;
    void angle1(float angle1) noexcept;
    void angle2(float angle2) noexcept;
    void update(float delta);

private:
    // Set of all default parameters for the pendulum
    struct Defaults {
        inline float static constexpr LENGTH1 { 70. };
        inline float static constexpr LENGTH2 { 70. };
        inline float static constexpr MIN_LENGTH { 1. };
        inline float static constexpr MAX_LENGTH { 500. };
        inline float static constexpr MASS1 { 10. };
        inline float static constexpr MASS2 { 10. };
        inline float static constexpr MIN_MASS { 1. };
        inline float static constexpr MAX_MASS { 200. };
        inline float static constexpr ANGLE1 { 90. };
        inline float static constexpr ANGLE2 { 90. };
        inline float static constexpr MIN_ANGLE { 0. };
        inline float static constexpr MAX_ANGLE { 360. };
        inline float static constexpr GRAVITY { 10. };
        inline float static constexpr MIN_GRAVITY { 0.01F };
        inline float static constexpr MAX_GRAVITY { 100 };
        inline uint16_t static constexpr TRAIL { 250 };
        inline uint16_t static constexpr MIN_TRAIL {};
        inline uint16_t static constexpr MAX_TRAIL { 1000 };
        inline uint16_t static constexpr MASS_TO_RADIUS_RATIO { 2 };   // We use this ratio to scale down object's mass and use it as radius to
                                                                       // indicate increased mass -> increased radius
        inline sf::Color static const TRAIL_COLOR { 255, 255, 255 };
    };

    float m_gravity { Defaults::GRAVITY };
    float m_aVel1 {};
    float m_aVel2 {};
    float m_aAcc1 {};
    float m_aAcc2 {};
    float m_currAngle1 { Defaults::ANGLE1 };
    float m_currAngle2 { Defaults::ANGLE2 };
    float m_dampCoeff1 {};
    float m_dampCoeff2 {};
    float m_mass1 { Defaults::MASS1 };
    float m_mass2 { Defaults::MASS2 };
    float m_length1 { Defaults::LENGTH1 };
    float m_length2 { Defaults::LENGTH2 };
    float m_xCoord {};
    float m_yCoord {};
    uint16_t m_trailLength { Defaults::TRAIL };
    bool trailEnabled {};

    sf::RectangleShape m_rod1 {
        {1, m_length1}  // taking thickness of rectangle as 1 to make it look like a rod
    };
    sf::RectangleShape m_rod2 {
        {1, m_length2}
    };
    sf::CircleShape m_bob1 { Defaults::MASS1 / Defaults::MASS_TO_RADIUS_RATIO };
    sf::CircleShape m_bob2 { Defaults::MASS2 / Defaults::MASS_TO_RADIUS_RATIO };
    std::array<sf::Vertex, Defaults::MAX_TRAIL> trail {};

    auto m_accelFunction(float aVel1, float aVel2) const noexcept -> std::pair<float, float>;
    void m_verletSolve(float delta) noexcept;
    void m_updatePositions() noexcept;
    void m_updateTrail() noexcept;

    void draw(sf::RenderTarget& target, [[maybe_unused]] sf::RenderStates states) const override
    {
        target.draw(m_rod1);
        target.draw(m_rod2);
        target.draw(m_bob1);
        target.draw(m_bob2);
        target.draw(trail.data(), m_trailLength, sf::PrimitiveType::LinesStrip);
    }
};