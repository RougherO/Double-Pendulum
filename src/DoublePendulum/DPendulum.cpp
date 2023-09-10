#include "DPendulum.hpp"
#include <cmath>
#include <numbers>

// Specify initial angle, length and mass of pendulum
DPendulum::DPendulum(const double angle1, const double angle2, const double length1, const double length2, const double mass2, const double mass1)
    : m_mass1 { mass1 }
    , m_mass2 { mass2 }
    , m_length1 { length1 }
    , m_length2 { length2 }
    , m_currAngle1 { (std::numbers::pi * angle1) / 180 }
    , m_currAngle2 { (std::numbers::pi * angle2) / 180 }
{
    m_line1 = sf::RectangleShape(sf::Vector2f(1, length1)); // First Pendulum rod
    m_line2 = sf::RectangleShape(sf::Vector2f(1, length2)); // Second Pendulum rod
    m_circle1.setOrigin(m_circle1.getRadius(), m_circle1.getRadius()); // Setting origin of circle to the center
    m_circle2.setOrigin(m_circle2.getRadius(), m_circle2.getRadius());
}

double DPendulum::getLength1() const
{
    return m_length1;
}

double DPendulum::getLength2() const
{
    return m_length2;
}

double DPendulum::getMass1() const
{
    return m_mass1;
}

double DPendulum::getMass2() const
{
    return m_mass2;
}

double DPendulum::getCurrAngle1() const
{
    return m_currAngle1 * 180 / std::numbers::pi;
}

double DPendulum::getCurrAngle2() const
{
    return m_currAngle2 * 180 / std::numbers::pi;
}

double DPendulum::getAVel1() const
{
    return m_aVel1;
}

double DPendulum::getAVel2() const
{
    return m_aVel2;
}

double DPendulum::getAAcc1() const
{
    return m_aAcc1;
}

double DPendulum::getAAcc2() const
{
    return m_aAcc2;
}

double DPendulum::getGravity() const
{
    return m_gravity;
}

double DPendulum::getDampCoeff1() const
{
    return m_dampCoeff1;
}

double DPendulum::getDampCoeff2() const
{
    return m_dampCoeff2;
}

double DPendulum::getKE() const
{
    double KE1 = 0.5 * m_mass1 * std::pow(m_length1 * m_aVel1, 2);
    double KE2 = 0.5 * m_mass2 * (std::pow(m_length1 * m_aVel1, 2) + std::pow(m_length2 * m_aVel2, 2) + 2 * m_length1 * m_aVel1 * m_length2 * m_aVel2 * std::cos(m_currAngle1 - m_currAngle2));
    return KE1 + KE2;
}

double DPendulum::getPE() const
{
    double PE1 = m_mass1 * m_gravity * (m_Y - m_circle1.getPosition().y);
    double PE2 = m_mass2 * m_gravity * (m_Y - m_circle2.getPosition().y);
    return PE1 + PE2;
}

double DPendulum::getEnergy() const
{

    return getPE() + getKE();
}

void DPendulum::setPosition(double X, double Y)
{
    m_X = X, m_Y = Y;
    // Setting position of pendulum1
    m_line1.setPosition(m_X, m_Y);
    m_line1.setRotation(-(m_currAngle1 * 180) / std::numbers::pi); // negative angle because SFML considers clockwise as positive and anticlockwise as negative
    m_circle1.setPosition(
        m_line1.getPosition().x + m_length1 * std::sin(m_currAngle1),
        m_line1.getPosition().y + m_length1 * std::cos(m_currAngle1));

    // Setting position of pendulum2
    m_line2.setPosition(m_circle1.getPosition());
    m_line2.setRotation(-(m_currAngle2 * 180) / std::numbers::pi);
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

void DPendulum::setLength1(double length)
{
    m_length1 = length;
    m_line1.setSize(sf::Vector2f(1, length));
    m_circle1.setPosition(
        m_line1.getPosition().x + m_length1 * std::sin(m_currAngle1),
        m_line1.getPosition().y + m_length1 * std::cos(m_currAngle1));

    m_line2.setPosition(m_circle1.getPosition());
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

void DPendulum::setLength2(double length)
{
    m_length2 = length;
    m_line2.setSize(sf::Vector2f(1, length));
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

void DPendulum::setMass1(double mass)
{
    m_mass1 = mass;
}

void DPendulum::setMass2(double mass)
{
    m_mass2 = mass;
}

void DPendulum::setAngle1(double angle)
{
    m_currAngle1 = (angle * std::numbers::pi) / 180;

    // Setting pendulum1's angle
    m_line1.setRotation(-(m_currAngle1 * 180) / std::numbers::pi);
    m_circle1.setPosition(
        m_line1.getPosition().x + m_length1 * std::sin(m_currAngle1),
        m_line1.getPosition().y + m_length1 * std::cos(m_currAngle1));

    // Setting pendulum2
    m_line2.setPosition(m_circle1.getPosition());
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

void DPendulum::setAngle2(double angle)
{
    m_currAngle2 = (angle * std::numbers::pi) / 180;

    // Setting pendulum2's angle
    m_line2.setRotation(-(m_currAngle2 * 180) / std::numbers::pi);
    m_circle2.setPosition(
        m_line2.getPosition().x + m_length2 * std::sin(m_currAngle2),
        m_line2.getPosition().y + m_length2 * std::cos(m_currAngle2));
}

// Gravity has been purposfully kept modifiable
void DPendulum::setGravity(double g)
{
    m_gravity = g;
}

void DPendulum::setDampCoeff1(double b)
{
    m_dampCoeff1 = b;
}

void DPendulum::setDampCoeff2(double b)
{
    m_dampCoeff2 = b;
}

void DPendulum::enableTrail(bool val)
{
    trailEnable = val;
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