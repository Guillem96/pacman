#include "player.h"

#include <GL/glut.h>
#include "map.h"
#include "util.h"

Player::Player(const Map *map)
    : m_map(map), m_dir(Vector2<>(0, 1)), m_animDir(Vector2<float>(0, 1))
{
}

Player::~Player()
{

}

void Player::setDirection(Vector2<> dir)
{
    if (m_dir.getX() == 0 && m_dir.getY() == 0)
    {
        m_dir = dir;
        m_animDir = Vector2<float>(dir.getX(), dir.getY());
        m_initMovement();
    }
}

void Player::init() 
{
    /* Generate a valid initial pos */
    m_pos = Vector2<>::getRandom(m_map->getHeight(),
                                 m_map->getWidth());

    while ((*m_map)(m_pos.getX(), m_pos.getY())->isWall())
        m_pos = Vector2<>::getRandom(m_map->getHeight(),
                                     m_map->getWidth());

    m_initMovement();
}

void Player::m_initMovement()
{
    m_animPos = Vector2<float>(m_pos.getX(), m_pos.getY());
    m_animDir = Vector2<float>(m_dir.getX() / (float)m_animDuration,
                               m_dir.getY() / (float)m_animDuration);
    m_remaining = m_animDuration;
}

void Player::update(long deltaTime)
{
    Vector2<> nextPos = m_pos + m_dir;
    if ((*m_map)(nextPos.getX(), nextPos.getY())->isWall())
    {
        m_dir = Vector2<>();
        m_animDir = Vector2<float>();
        m_remaining = 0;
        return;
    } 

    m_animPos = Vector2<float>(m_animPos.getX() + m_animDir.getX() * min((int)deltaTime, (int) m_remaining), 
                               m_animPos.getY() + m_animDir.getY() * min((int)deltaTime, (int) m_remaining));

    m_remaining -= deltaTime;
    if (m_remaining <= 0)
    {
        m_pos = m_pos + m_dir;
        m_dir = Vector2<>();
        m_animDir = Vector2<float>();
        m_remaining = 0;
    }
}

void Player::render() const 
{
    auto normPos = normalizeCoords<float>(m_animPos, m_map->getHeight());

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

