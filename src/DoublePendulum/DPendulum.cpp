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

std::pair<double, double> DPendulum::fDash(double x1, double x2)
{
    // Updating Angular acceleration of each pendulum per frame on this massive formula
    double num1_1 = -m_gravity * (2 * m_mass1 + m_mass2) * std::sin(m_currAngle1);
    double num2_1 = m_mass2 * m_gravity * std::sin(m_currAngle1 - 2 * m_currAngle2);
    double num3_1 = 2 * std::sin(m_currAngle1 - m_currAngle2) * m_mass2 * (x2 * x2 * m_length2 + x1 * x1 * m_length1 * std::cos(m_currAngle1 - m_currAngle2));
    double den = (2 * m_mass1 + m_mass2 - m_mass2 * std::cos(2 * (m_currAngle1 - m_currAngle2)));

    double num1_2 = 2 * std::sin(m_currAngle1 - m_currAngle2);
    double num2_2 = x1 * x1 * m_length1 * (m_mass1 + m_mass2);
    double num3_2 = m_gravity * (m_mass1 + m_mass2) * std::cos(m_currAngle1);
    double num4_2 = x2 * x2 * m_length2 * m_mass2 * std::cos(m_currAngle1 - m_currAngle2);

    return { (num1_1 - num2_1 - num3_1) / (m_length1 * den) - m_dampCoeff1 * m_aVel1, (num1_2 * (num2_2 + num3_2 + num4_2)) / (m_length2 * den) - m_dampCoeff2 * m_aVel2 };
}

void DPendulum::RKSolver(double dt)
{
    // Updating Angular velocity
    auto [k1_1, k1_2] = fDash(m_aVel1, m_aVel2); // k1 of bob 1 and 2
    auto [k2_1, k2_2] = fDash(m_aVel1 + k1_1 * (dt / 2), m_aVel2 + k1_2 * (dt / 2)); // k2 of bob 1 and 2
    auto [k3_1, k3_2] = fDash(m_aVel1 + k2_1 * (dt / 2), m_aVel2 + k2_2 * (dt / 2));
    auto [k4_1, k4_2] = fDash(m_aVel1 + k3_1 * (dt), m_aVel2 + k3_2 * (dt));

    m_aVel1 += (dt / 6) * (k1_1 + 2 * (k2_1 + k3_1) + k4_1);
    m_aVel2 += (dt / 6) * (k1_2 + 2 * (k2_2 + k3_2) + k4_2);
    m_currAngle1 += m_aVel1 * dt;
    m_currAngle2 += m_aVel2 * dt;

    // Keeping angles within 2pi not necessary just for fun
    m_currAngle1 -= static_cast<int>(m_currAngle1 / (std::numbers::pi * 2)) * std::numbers::pi * 2;
    m_currAngle2 -= static_cast<int>(m_currAngle2 / (std::numbers::pi * 2)) * std::numbers::pi * 2;
}

void DPendulum::rotate()
{
    // Rotating pendulum1 by m_currAngle1 degrees
    m_line1.setRotation(-(m_currAngle1 * 180) / std::numbers::pi);
    m_circle1.setPosition(
        m_line1.getPosition().x + m_length1 * std::sin(m_currAngle1),
        m_line1.getPosition().y + m_length1 * std::cos(m_currAngle1));

    // Positioning pendulum2 according to pendulum1's rotation
    m_line2.setPosition(m_circle1.getPosition().x, m_circle1.getPosition().y);
    m_line2.setRotation(-(m_currAngle2 * 180) / std::numbers::pi);
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

void DPendulum::update(double frameTime)
{
    while (frameTime > 0) {
        // Solving DE on every update call to calc next angle of each bob
        RKSolver(m_dt);

        // Rotating to updated angle
        rotate();
        if (trailEnable) {
            trail.emplace_back(m_circle2.getPosition());
            if (trail.size() == trailLength + 1) {
                trail.assign(trail.begin() + 1, trail.end());
            }

            // Setting an alpha value based on the recency of the point
            for (auto i = trail.begin(); i < trail.end(); ++i) {
                i->color = sf::Color(255, 255, 255, (i - trail.begin()) / static_cast<float>(trail.size()) * 100);
            }
        }
        frameTime -= m_dt;
    }
}