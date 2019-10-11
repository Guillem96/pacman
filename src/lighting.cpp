#include "lighting.h"

Lighting::Lighting() {}
Lighting::~Lighting() {}


void Lighting::init()
{
    m_ambientColor = Color::darkGreen.glColorfv();
}

void Lighting::update(long) {}
    
void Lighting::render() const
{
    /* Ambient light */
    glLightiv(GL_LIGHT0, GL_POSITION, m_ambientLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambientColor);
    glEnable(GL_LIGHT0);
}

void Lighting::destroy() 
{
    delete m_ambientColor;
}