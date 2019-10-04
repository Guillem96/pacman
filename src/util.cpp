#include <GL/glut.h>
#include "util.h"

#include <stdlib.h>
#include <math.h>

#include "cell.h"
#include "map.h"

Color::Color(int r, int g, int b): r(r), g(g), b(b) {}

void Color::glColor() const
{
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

const Color Color::darkGray = Color(33, 33, 33);
const Color Color::yellowPacman = Color(244, 182, 7);
const Color Color::darkGreen = Color(45, 85, 94);
const Color Color::red = Color(255, 0, 0);
const Color Color::cyan = Color(0, 255, 255);
const Color Color::pink = Color(255, 194, 239);

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

int min(float a, float b)
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

int *range(int start, int end, int step)
{
    int size = end - start;
    size /= step;
    size += 1;

    int *res = new int[size];
    int it = 0;
    for (int i = start; i < end; i += step)
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

void drawCircle(float x, float y, float r)
{
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.1415f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (r * cos(i * twicePi / triangleAmount)),
            y + (r * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

bool shouldChangeDirection(const Vector2<>& pos, const Map* map)
{
    std::vector<Cell*> adj = map->getAdjacent(pos);
    int nPaths = 0;
    for (auto cell: adj)
        nPaths += !cell->isWall();
    
    return nPaths > 2;
}

Vector2<> getRandomDirection(const Vector2<>& pos, const Map* map)
{
    std::vector<Cell*> adj = map->getAdjacent(pos);
    std::vector<Vector2<>> directions;
    
    for (auto cell: adj)
        if(!cell->isWall())
            directions.push_back(cell->getPosition() - pos);

    return randomChoice(directions);
}

void drawCube(const Vector2<>& pos, float w, float h)
{
    auto x = pos.getX();
    auto y = pos.getY();
    auto zSize = 5.0;

    glPushMatrix();
    glTranslatef(x * w, y * h, 0);

    glBegin(GL_QUADS);
    
    w = w / 2.f;
    h = h / 2.f;

    // FRONT
    glVertex3f(-w, -h, zSize);
    glVertex3f(w, -h, zSize);
    glVertex3f(w, h, zSize);
    glVertex3f(-w, h, zSize);

    // BACK
    glVertex3f(-w, -h, -zSize);
    glVertex3f(-w, h, -zSize);
    glVertex3f(w, h, -zSize);
    glVertex3f(w, -h, -zSize);

    // LEFT
    glVertex3f(-w, -h, zSize);
    glVertex3f(-w, h, zSize);
    glVertex3f(-w, h, -zSize);
    glVertex3f(-w, -h, -zSize);

    // RIGHT
    glVertex3f(w, -h, -zSize);
    glVertex3f(w, h, -zSize);
    glVertex3f(w, h, zSize);
    glVertex3f(w, -h, zSize);

    // TOP
    glVertex3f(-w, h, zSize);
    glVertex3f(w, h, zSize);
    glVertex3f(w, h, -zSize);
    glVertex3f(-w, h, -zSize);

    // BOTTOM
    glVertex3f(-w, -h, zSize);
    glVertex3f(-w, -h, -zSize);
    glVertex3f(w, -h, -zSize);
    glVertex3f(w, -h, zSize);

    glEnd();
    glPopMatrix();
}  