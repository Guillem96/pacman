#include "cell.h"
#include "map.h"

#include <iostream>
#include <GL/glut.h>

Cell::Cell(CellType type): m_type(type)
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
    if (m_type == CellType::Path)
        return;

    Vector2<> normPos = Vector2<>(m_pos.getY(), 
                                  m_map->getHeight() -1 - m_pos.getX());
    auto x = normPos.getX();
    auto y = normPos.getY();

    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    glColor3f(45 / (float)255, 85 / (float)255, 94 / (float)255);
    
    glBegin(GL_QUADS);

    glVertex2i(x * w, y * h);
    glVertex2i((x + 1) * w, y * h); 
    glVertex2i((x + 1) * w, (y + 1) * h); 
    glVertex2i(x * w, (y + 1) * h); 

    glEnd();
}

void Cell::textRender() const
{
    if (m_type == CellType::Wall)
        std::cout << '#';
    else
        std::cout << ' ';
}

const CellType& Cell::getType() { return m_type; }
void Cell::setType(CellType type) { m_type = type; }

const Vector2<>& Cell::getPosition() { return m_pos; }
void Cell::setPosition(Vector2<> pos) { m_pos = pos; }

void Cell::setMapReference(const Map* map) { m_map = map; }

bool Cell::isWall() { return m_type == CellType::Wall; }
    
void Cell::destroy()
{
    
}