#include "player.h"

#include <GL/glut.h>
#include "map.h"
#include "util.h"

Player::Player(const Map *map)
    : m_map(map), m_dir(Vector2<>(0, 1))
{

}

Player::~Player()
{

}

void Player::setDirection(Vector2<> dir)
{

}

void Player::init() 
{
    /* Generate a valid initial pos */
    m_pos = Vector2<>::getRandom(m_map->getHeight(),
                               m_map->getWidth());

    while ((*m_map)(m_pos.getX(), m_pos.getY())->isWall())
        m_pos = Vector2<>::getRandom(m_map->getHeight(),
                                   m_map->getWidth());
    
}

void Player::m_initMovement()
{
    m_remaining = m_animDuration;
}

void Player::update(long deltaTime)
{

    Vector2<> nextPos = m_pos + m_dir;
    if ((*m_map)(nextPos.getX(), nextPos.getY())->isWall())
    {
        m_pos = m_pos - m_dir;
        m_dir = Vector2<>();
    }
}

void Player::render() const 
{
    auto normPos = normalizeCoords(m_pos, m_map->getHeight());

    auto x = normPos.getX();
    auto y = normPos.getY();

    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    glColor3f(244 / (float)255, 182 / (float)255, 7 / (float)255);
    drawCircle(w * x + w / (float)2, 
               h * y + h / (float)2, 
               w * 0.4);
}

void Player::destroy() 
{

}

