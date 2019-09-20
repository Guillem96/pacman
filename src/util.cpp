#include "util.h"
#include <stdlib.h>

Vector2::Vector2(): m_x(0), m_y(0)
{
}

Vector2::Vector2(int value): m_x(value), m_y(value)
{
}

Vector2::Vector2(int x, int y): m_x(x), m_y(y)
{
}

Vector2 Vector2::getRandom(int maxX, int maxY)
{
    return Vector2(rand() % maxX, rand() % maxY);
}

Vector2::~Vector2()
{
}

bool Vector2::operator==(Vector2 other){
    return m_x == other.m_x && other.m_y == m_y;
}

Vector2 Vector2::operator+(Vector2 other) {
    return Vector2(m_x + other.m_x, other.m_y + m_y);
}

Vector2 Vector2::operator-(Vector2 other) {
    return Vector2(m_x - other.m_x, other.m_y - m_y);
}

int Vector2::getX() const
{
    return m_x;
}

int Vector2::getY() const
{
    return m_y;
}