#include "phantom.h"
#include "map.h"

#include <GL/glut.h>

Phantom::Phantom(const Map *map) : m_map(map)
{
}

Phantom::Phantom(const Map *map, Color color) : m_map(map), m_color(color)
{
}

Phantom::~Phantom() {}

void Phantom::setDirection(Vector2<> dir)
{
    if (m_userControl && m_dir.getX() == 0 && m_dir.getY() == 0)
    {
        m_dir = dir;
        m_initMovement();
    }
}

void Phantom::init()
{
    auto mapCenter = Vector2<>(m_map->getHeight(), m_map->getWidth()) / 2;
    auto offset = randomRange(-1, 1);
    m_pos = mapCenter + offset;
    m_dir = m_userControl ? Vector2<>() : getRandomDirection(m_pos, m_map);
    m_initMovement();
}

void Phantom::m_initMovement()
{
    m_animPos = Vector2<float>(m_pos.getX(), m_pos.getY());
    m_animDir = Vector2<float>(m_dir.getX(), m_dir.getY());
    m_animDir = m_animDir / (float)m_animDuration;
    m_remaining = m_dir.getX() == 0 && m_dir.getY() == 0 ? 0 : m_animDuration;
}

void Phantom::render() const
{
    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    m_color.glColor();
    glPushMatrix();

    glTranslatef(w * m_animPos.getY() + w / 2.f,
                 0,
                 h * m_animPos.getX() + h / 2.f);
    glRotatef(-90, 1.0, 0, 0);
    glutSolidCone(w / 2.5f, h * .8f, 20, 20);
    glPopMatrix();
}

void Phantom::update(long deltaTime)
{
    if ((*m_map)(m_pos + m_dir)->isWall())
    {
        m_dir = m_userControl ? Vector2<>() : getRandomDirection(m_pos, m_map);
        m_initMovement();
        return;
    }

    Vector2<float> offset = m_animDir * min((int)deltaTime, (int)m_remaining);
    m_animPos = m_animPos + offset;
    m_remaining -= deltaTime;

    if (m_remaining <= 0)
    {
        m_pos = m_pos + m_dir;
        if (!m_userControl && shouldChangeDirection(m_pos, m_map))
            m_dir = getRandomDirection(m_pos, m_map);
        else if (m_userControl && shouldChangeDirection(m_pos, m_map))
            m_dir = Vector2<>();

        m_initMovement();
    }
}

void Phantom::destroy()
{
}

void Phantom::toogleUserControl() { m_userControl = !m_userControl; }
