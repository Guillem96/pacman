#include "player.h"

#include <GL/glut.h>
#include "map.h"
#include "util.h"

Player::Player(const Map *map)
    : m_map(map), m_dir(Vector2())
{

}

Player::~Player()
{

}

void Player::setDirection(Vector2 dir)
{

}

void Player::init() 
{
    /* Generate a valid initial pos */
    m_pos = Vector2::getRandom(m_map->getHeight(),
                               m_map->getWidth());

    while ((*m_map)(m_pos.getX(), m_pos.getY())->isWall())
        m_pos = Vector2::getRandom(m_map->getHeight(),
                                   m_map->getWidth());
    
}

void Player::render() const 
{
    Vector2 normPos = Vector2(m_pos.getY(), 
                              m_map->getHeight() -1 - m_pos.getX());
    auto x = normPos.getX();
    auto y = normPos.getY();

    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.second;
    auto h = cellSize.first;

    glColor3f(45 / (float)255, 85 / (float)255, 94 / (float)255);
    drawCircle(w * x + w / (float)2, 
               h * y + h / (float)2, 
               w * 0.4, 20);
}

void Player::destroy() 
{

}