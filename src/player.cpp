#include "player.h"

#include <GL/glut.h>
#include "map.h"
#include "util.h"

Player::Player(const Map *map)
    : m_map(map)
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

    m_dir = getRandomDirection(m_pos, m_map);
    m_initMovement();
}

void Player::m_initMovement()
{
    m_animPos = Vector2<float>(m_pos.getX(), m_pos.getY());

    m_animDir = Vector2<float>(m_dir.getX(), m_dir.getY());
    m_animDir = m_animDir / (float)m_animDuration;

    m_remaining = m_animDuration;
}

void Player::m_movementLogic(long deltaTime)
{
    if ((*m_map)(m_pos + m_dir)->isWall())
    {
        m_dir = getRandomDirection(m_pos, m_map);
        m_initMovement();
        return;
    }

    Vector2<float> offset = m_animDir * min((int)deltaTime, (int)m_remaining);
    m_animPos = m_animPos + offset;
    m_remaining -= deltaTime;

    if (m_remaining <= 0)
    {
        m_pos = m_pos + m_dir;
        if (shouldChangeDirection(m_pos, m_map))
            m_dir = getRandomDirection(m_pos, m_map);

        m_initMovement();
    }
}

void Player::m_gameRulesLogic(long deltaTime)
{
    auto cell = (*m_map)(m_pos);
    if (cell->getType() == CellType::Path && cell->hasFood())
    {
        // TODO: Player score
        cell->eat();
    }
}

void Player::update(long deltaTime)
{
    m_movementLogic(deltaTime);
    m_gameRulesLogic(deltaTime);
}

void Player::render() const
{
    auto normPos = normalizeCoords<float>(m_animPos, m_map->getHeight());

    auto x = normPos.getX();
    auto y = normPos.getY();

    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    Color::yellowPacman.glColor();

    drawCircle(w * x + w / (float)2,
               h * y + h / (float)2,
               w * 0.4);
}

void Player::destroy()
{
}
