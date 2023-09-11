#pragma once
#include "SFML/Graphics.hpp"
#include <utility>
#include <vector>

class DPendulum : public sf::Drawable, sf::Transformable {
private:
    double m_gravity { 9.80665f };
    double m_aVel1 {};
    double m_aVel2 {};
    double m_aAcc1 {};
    double m_aAcc2 {};
    double m_currAngle1 {};
    double m_currAngle2 {};
    double m_dampCoeff1 {};
    double m_dampCoeff2 {};
    double m_mass1 {};
    double m_mass2 {};
    double m_length1 {};
    double m_length2 {};
    double m_X {};
    double m_Y {};

    double m_dt { 1. / 15. };

    bool trailEnable {};
    std::size_t trailLength {};

    sf::RectangleShape m_line1 {};
    sf::RectangleShape m_line2 {};
    sf::CircleShape m_circle1 { 5. };
    sf::CircleShape m_circle2 { 5. };
    std::vector<sf::Vertex> trail {};

public:
    DPendulum();
    DPendulum(const double angle1, const double angle2, const double length1, const double length2, const double mass1, const double mass2, std::size_t traillen);

public:
    void setPosition(double X, double Y);
    void setLength1(double length);
    void setLength2(double length);
    void setMass1(double mass);
    void setMass2(double mass);
    void setAngle1(double angle);
    void setAngle2(double angle);
    void setGravity(double g);
    void setDampCoeff1(double b);
    void setDampCoeff2(double b);
    void setTrailLength(std::size_t len);
    void enableTrail(bool val);

public:
    void update(double dt);

public:
    double getLength1() const;
    double getLength2() const;
    double getMass1() const;
    double getMass2() const;
    double getAVel1() const;
    double getAVel2() const;
    double getAAcc1() const;
    double getAAcc2() const;
    double getCurrAngle1() const;
    double getCurrAngle2() const;
    double getGravity() const;
    double getDampCoeff1() const;
    double getDampCoeff2() const;
    double getPE() const;
    double getKE() const;
    double getEnergy() const;

private:
    std::pair<double, double> fDash(double x1, double x2);
    void RKSolver(double frameTime);
    void rotate();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        // states.texture = NULL;
        target.draw(m_line1, states);
        target.draw(m_line2, states);
        target.draw(m_circle1, states);
        target.draw(m_circle2, states);
        target.draw(&trail[0], trail.size(), sf::PrimitiveType::LinesStrip);
    }
};