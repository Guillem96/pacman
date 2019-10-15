#include "lighting.h"
#include "player.h"

Lighting::Lighting(const Player *player) : m_player(player) {}
Lighting::~Lighting() {}

void Lighting::init()
{
    auto center = Vector2<float>(
        glutGet(GLUT_WINDOW_HEIGHT) / 2.f,
        glutGet(GLUT_WINDOW_WIDTH) / 2.f);
    m_ambientColor = Color::darkGray.glColorfv();
    m_ambientLightPos[0] = center.getY();
    m_ambientLightPos[1] = 20;
    m_ambientLightPos[2] = center.getX();
}

void Lighting::update(long)
{
    auto dir = m_player->m_dir * 5.f;

    m_playerDir[0] = dir.getY();
    m_playerDir[1] = -0.2;
    m_playerDir[2] = dir.getX();

    auto cellSize = m_player->m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();
    auto playerPos = m_player->m_animPos;

    m_playerLightPos[0] = playerPos.getY() * (float)w;
    m_playerLightPos[1] = h / 2.f;
    m_playerLightPos[2] = playerPos.getX() * (float)h;
}

void Lighting::render() const
{

    // glPushMatrix();
    for (int i = 0; i < 3; i++)
        std::cout << m_ambientLightPos[i] << '-';

    std::cout << std::endl;

    /* Ambient light */
    glLightiv(GL_LIGHT0, GL_POSITION, m_ambientLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambientColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Color(0, 0, 0).glColorfv());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Color(0, 0, 0).glColorfv());
    glEnable(GL_LIGHT0);

    /* Player light */
    glLightfv(GL_LIGHT1, GL_POSITION, m_playerLightPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Color::yellowPacman.glColorfv());
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, m_playerDir);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.f);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);
    glEnable(GL_LIGHT1);
}

void Lighting::destroy()
{
    delete m_ambientColor;
}