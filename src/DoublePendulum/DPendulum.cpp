#include "DPendulum.hpp"
#include <cmath>
#include <numbers>
#include <ranges>

using std::sin, std::cos;
constexpr float pi = std::numbers::pi_v<float>;

inline static auto radToDeg(float angle) noexcept -> float
{
    return angle * 180.F / pi;
}

double DPendulum::getLength2() const
{
    return m_length2;
}

void DPendulum::position(sf::Vector2f const& coords) noexcept
{
    position_rod1(coords);
    m_rod1.setRotation(radToDeg(-m_currAngle1));   // negative angle because SFML considers clockwise as positive and anticlockwise as negative

    position_rod2(m_bob1.getPosition());
    m_rod2.setRotation(radToDeg(-m_currAngle2));

    std::ranges::for_each_n(trail.begin(), m_trailLength, [this](sf::Vertex& points) {
        points.position = m_bob2.getPosition();
    });
}

void DPendulum::position_rod1(sf::Vector2f const& coords) noexcept
{
    m_xCoord = coords.x;
    m_yCoord = coords.y;

    m_rod1.setPosition(m_xCoord, m_yCoord);
    position_bob1(coords);
}

void DPendulum::position_bob1(sf::Vector2f const& rod_coords) noexcept
{
    auto [xCoord, yCoord] = rod_coords;

    m_bob1.setPosition(
        xCoord + m_length1 * sin(m_currAngle1),
        yCoord + m_length1 * cos(m_currAngle1));
}

void DPendulum::position_rod2(sf::Vector2f const& coords) noexcept
{
    auto [xCoord, yCoord] = coords;

    m_rod2.setPosition(xCoord, yCoord);
    position_bob2(coords);
}

void DPendulum::position_bob2(sf::Vector2f const& rod_coords) noexcept
{
    auto [xCoord, yCoord] = rod_coords;

    m_bob2.setPosition(
        xCoord + m_length2 * sin(m_currAngle2),
        yCoord + m_length2 * cos(m_currAngle2));
}

void DPendulum::length1(float length) noexcept
{
    m_length1 = length;
    m_rod1.setSize({ 1, length });

    position_bob1(m_rod1.getPosition());
    position_rod2(m_bob1.getPosition());
}

void DPendulum::length2(float length) noexcept
{
    m_length2 = length;
    m_rod2.setSize({ 1, length });

    position_bob2(m_rod2.getPosition());
}
void DPendulum::angle1(float angle) noexcept
{
    m_currAngle1 = angle;
    m_rod1.setRotation(radToDeg(-m_currAngle1));

    position_bob1(m_rod1.getPosition());
    position_rod2(m_bob1.getPosition());
}

void DPendulum::angle2(float angle) noexcept
{
    m_currAngle2 = angle;
    m_rod2.setRotation(radToDeg(-m_currAngle2));
    position_bob2(m_rod2.getPosition());
}

auto DPendulum::m_accelFunction(float aVel1, float aVel2) const noexcept -> std::pair<float, float>
{
    // Updating Angular acceleration of each pendulum per frame on this massive formula
    float num1_1 = -m_gravity * (2 * m_mass1 + m_mass2) * sin(m_currAngle1);
    float num2_1 = m_mass2 * m_gravity * sin(m_currAngle1 - 2 * m_currAngle2);
    float num3_1 = 2 * sin(m_currAngle1 - m_currAngle2) * m_mass2 * (aVel2 * aVel2 * m_length2 + aVel1 * aVel1 * m_length1 * cos(m_currAngle1 - m_currAngle2));
    float den    = (2 * m_mass1 + m_mass2 - m_mass2 * cos(2 * (m_currAngle1 - m_currAngle2)));

    float num1_2 = 2 * sin(m_currAngle1 - m_currAngle2);
    float num2_2 = aVel1 * aVel1 * m_length1 * (m_mass1 + m_mass2);
    float num3_2 = m_gravity * (m_mass1 + m_mass2) * cos(m_currAngle1);
    float num4_2 = aVel2 * aVel2 * m_length2 * m_mass2 * cos(m_currAngle1 - m_currAngle2);

    return { (num1_1 - num2_1 - num3_1) / (m_length1 * den) - m_dampCoeff1 * m_aVel1, (num1_2 * (num2_2 + num3_2 + num4_2)) / (m_length2 * den) - m_dampCoeff2 * m_aVel2 };
}

void DPendulum::m_verletSolve(float delta) noexcept
{
    auto [newAcc1, newAcc2] = m_accelFunction(m_aVel1, m_aVel2);

    m_currAngle1 += m_aVel1 * delta + 0.5F * m_aAcc1 * delta * delta;
    m_currAngle2 += m_aVel2 * delta + 0.5F * m_aAcc2 * delta * delta;
    m_aVel1      += 0.5F * (m_aAcc1 + newAcc1) * delta;
    m_aVel2      += 0.5F * (m_aAcc2 + newAcc2) * delta;
    m_aAcc1       = newAcc1;
    m_aAcc2       = newAcc2;
}

void DPendulum::m_updatePositions() noexcept
{
    // After new angles are calculated, we update the angular
    // rotation of both pendulums

    m_rod1.setRotation(radToDeg(-m_currAngle1));
    position_bob1(m_rod1.getPosition());

    position_rod2(m_bob1.getPosition());
    m_rod2.setRotation(radToDeg(-m_currAngle2));
}

void DPendulum::m_updateTrail() noexcept
{
    for (auto* ptr = trail.begin() + m_trailLength - 1; ptr != trail.begin(); --ptr) {
        ptr[0].position = ptr[-1].position;
    }
    trail[0].position = m_bob2.getPosition();
}

void DPendulum::update(float delta)
{
    m_verletSolve(delta);
    m_updatePositions();
    m_updateTrail();
}