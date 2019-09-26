#include <GL/glut.h>
#include "util.h"

#include <stdlib.h>
#include <math.h>

Vector2::Vector2() : m_x(0), m_y(0)
{
}

Vector2::Vector2(int value) : m_x(value), m_y(value)
{
}

Vector2::Vector2(int x, int y) : m_x(x), m_y(y)
{
}

Vector2 Vector2::getRandom(int maxX, int maxY)
{
    return Vector2(rand() % maxX, rand() % maxY);
}

Vector2::~Vector2()
{
}

bool Vector2::operator==(Vector2 other)
{
    return m_x == other.m_x && other.m_y == m_y;
}

bool Vector2::operator!=(Vector2 other)
{
    return !((*this) == other);
}

Vector2 Vector2::operator+(Vector2 other) const
{
    return Vector2(m_x + other.m_x, other.m_y + m_y);
}

Vector2 Vector2::operator-(Vector2 other) const
{
    return Vector2(m_x - other.m_x, m_y - other.m_y);
}

int Vector2::getX() const
{
    return m_x;
}

int Vector2::getY() const
{
    return m_y;
}

void shuffle(int *array, int n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int sign(int x)
{
    if (x < 0)
        return -1;
    return 1;
}

int* range(int start, int end, int step)
{
    int size = end - start;
    size /= step;
    size += 1;
    
    int* res = new int[size];
    int it = 0;
    for (int i = start; i < end; i+=step)
    {
        res[it] = i;
        it++;
    }
    return res;
}

int randomRange(int low, int high)
{
    return (rand() % (high - low + 1)) + low;
}

int even(int val)
{
    if (val % 2 == 1)
        val--;
    return val;
}

int odd(int val)
{
    if (val % 2 == 0)
        val--;
    return val;
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
        float x = r * cosf(theta); //> calculate the x component 
        float y = r * sinf(theta); //> calculate the y component 
        glVertex2f(x + cx, y + cy); //> output vertex 
    }
    glEnd();
}
