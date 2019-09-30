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
    Vector2<> normPos = Vector2<>(m_pos.getY(), 
                                  m_map->getHeight() -1 - m_pos.getX());
    auto x = normPos.getX();
    auto y = normPos.getY();

    auto cellSize = m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();

    if (isWall())
    {
        Color::darkGreen.glColor();
        
        glBegin(GL_QUADS);

        glVertex2i(x * w, y * h);
        glVertex2i((x + 1) * w, y * h); 
        glVertex2i((x + 1) * w, (y + 1) * h); 
        glVertex2i(x * w, (y + 1) * h); 

        glEnd();
        return;
    }
    
    if (hasFood())
    {
        Color(97, 77, 25).glColor();
        drawCircle(w * x + w / 2.f, 
                   h * y + h / 2.f, 
                   w * .1f);
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