#include <cmath>

#include "pendulum.hpp"

void PendulumSprite::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    auto const sin_t1 = std::sin(m_entity->curr_angle1);
    auto const cos_t1 = std::cos(m_entity->curr_angle1);
    auto const sin_t2 = std::sin(m_entity->curr_angle2);
    auto const cos_t2 = std::cos(m_entity->curr_angle2);

    auto const rod1_pos = sf::Vector2f { m_entity->pos_x, m_entity->pos_y };
    auto const rod1_ang = sf::radians(m_entity->curr_angle1);
    auto const bob1_pos = rod1_pos + m_entity->length1 * sf::Vector2f { sin_t1, cos_t1 };

    auto const rod2_pos = bob1_pos;
    auto const rod2_ang = sf::radians(m_entity->curr_angle2);
    auto const bob2_pos = rod2_pos + m_entity->length2 * sf::Vector2f { sin_t2, cos_t2 };

    rod1.setPosition(rod1_pos);
    rod1.setRotation(-rod1_ang);

    bob1.setPosition(bob1_pos);

    rod2.setPosition(rod2_pos);
    rod2.setRotation(-rod2_ang);

    bob2.setPosition(bob2_pos);

    target.draw(rod1);
    target.draw(rod2);
    target.draw(bob1);
    target.draw(bob2);
}