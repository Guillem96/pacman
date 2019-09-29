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

static bool shouldChangeDirection(const Vector2<>& pos, const Map* map)
{
    std::vector<Cell*> adj = map->getAdjacent(pos);
    int nPaths = 0;
    for (auto cell: adj)
        nPaths += !cell->isWall();
    
    return nPaths > 2;
}

static Vector2<> getRandomDirection(const Vector2<>& pos, const Map* map)
{
    std::vector<Cell*> adj = map->getAdjacent(pos);
    std::vector<Vector2<>> directions;
    
    for (auto cell: adj)
        if(!cell->isWall())
            directions.push_back(cell->getPosition() - pos);

    return randomChoice(directions);
}

void Player::update(long deltaTime)
{
    if ((*m_map)(m_pos + m_dir)->isWall())
    {
        m_dir = getRandomDirection(m_pos, m_map);
        m_initMovement();
        return;
    } 

    Vector2<float> offset = m_animDir * min((int)deltaTime, (int) m_remaining);
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

