#include "cell.h"
#include <iostream>

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
    if (m_type == CellType::Wall)
        std::cout << '#';
    else
        std::cout << ' ';
}

const CellType& Cell::getType()
{
    return m_type;
}

void Cell::setType(CellType type)
{
    m_type = type;
}

void Cell::destroy()
{
    
}