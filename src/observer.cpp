#include "observer.h"

#include <GL/glut.h>
#include <math.h>

#define PI 3.1416

Observer::Observer(int alpha, int beta, int radius) : m_alpha(alpha), m_beta(beta), m_radius(radius)
{
}

Observer::~Observer()
{
}

void Observer::init()
{
}

void Observer::update(long deltaTime)
{
    if (m_dir.getX() < 0 && m_beta <= (90 - 4))
        m_beta = (m_beta + 3);
    else if (m_dir.getX() > 0 && m_beta >= (-90 + 4))
        m_beta = m_beta - 3;
    else if (m_dir.getY() < 0)
        m_alpha = (m_alpha + 3) % 360;
    else if (m_dir.getY() > 0)
        m_alpha = (m_alpha - 3 + 360) % 360;
    setDirection(Vector2<>());
}

void Observer::render() const
{    
    float x, y, z;
    float upx, upy, upz;
    float modul;

    x = (float)m_radius * cos(m_alpha * 2 * PI / 360.0) * cos(m_beta * 2 * PI / 360.0);
    y = (float)m_radius * sin(m_beta * 2 * PI / 360.0);
    z = (float)m_radius * sin(m_alpha * 2 * PI / 360.0) * cos(m_beta * 2 * PI / 360.0);

    if (m_beta > 0)
    {
        upx = -x;
        upz = -z;
        upy = (x * x + z * z) / y;
    }
    else if (m_beta == 0)
    {
        upx = 0;
        upy = 1;
        upz = 0;
    }
    else
    {
        upx = x;
        upz = z;
        upy = -(x * x + z * z) / y;
    }

    modul = sqrt(upx * upx + upy * upy + upz * upz);

    upx = upx / modul;
    upy = upy / modul;
    upz = upz / modul;

    gluLookAt(x, y, z, 0.0, 0.0, 0.0, upx, upy, upz);
}

void Observer::destroy()
{
}

void Observer::setDirection(Vector2<> dir) { m_dir = dir; }