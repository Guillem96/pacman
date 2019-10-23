#include "cell.h"
#include "map.h"

#include <iostream>
#include <GL/glut.h>

Cell::Cell(CellType type): m_type(type), m_food(false)
{
}

Cell::Cell(CellType type, bool hasFood): m_type(type), m_food(hasFood)
{
}

Cell::~Cell()
{
}

void Cell::init()
{

}

void Cell::render() const
{
    auto x = m_pos.getY();
    auto y = m_pos.getX();
    
    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    if (isWall())
    {
        m_map->m_wallTex->active();
        drawCube(m_pos, w, h);
        glDisable(GL_TEXTURE_2D);
        return;
    }

    glPushMatrix();
    glTranslatef(w * x, -1, h * y);

    m_map->m_groundTex->active();
    glBegin(GL_QUADS);

    glNormal3f(0, 1, 0);
    glTexCoord2f(1, 1); glVertex3f(w, 0, h);
    glTexCoord2f(0, 1); glVertex3f(0, 0, h);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(w, 0, 0);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    if (hasFood())
    {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, 
                     GL_AMBIENT_AND_DIFFUSE, 
                     Color(245, 104, 104).glColorfv());
        glTranslatef(w * x + w / 2.f, 
                     h / 2.f,
                     h * y + h / 2.f);
        glutSolidSphere (w * .1f, 20, 20);
        glPopMatrix();
    }
}

void Cell::textRender() const
{
    if (m_type == CellType::Wall)
        std::cout << '#';
    else if (m_food)
        std::cout << "·";
    else 
        std::cout << ' ';
}

const CellType& Cell::getType() { return m_type; }
void Cell::setType(CellType type) { m_type = type; }

const Vector2<>& Cell::getPosition() { return m_pos; }
void Cell::setPosition(Vector2<> pos) { m_pos = pos; }

void Cell::setMapReference(const Map* map) { m_map = map; }

bool Cell::isWall() const { return m_type == CellType::Wall; }

void Cell::eat() { m_food = false; }
void Cell::fill() { m_food = true; }
bool Cell::hasFood() const { return !isWall() && m_food; }

void Cell::destroy()
{
    
}