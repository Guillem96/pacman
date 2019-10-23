#include "lighting.h"
#include "player.h"
#include "phantom.h"

Lighting::Lighting(const Player *player,
                   std::vector<const Phantom *> phantoms)
    : m_player(player), m_phantoms(phantoms) {}
Lighting::~Lighting() {}

void Lighting::init()
{
    auto center = Vector2<float>(
        glutGet(GLUT_WINDOW_HEIGHT) / 2.f,
        glutGet(GLUT_WINDOW_WIDTH) / 2.f);

    // Init ambient light
    m_ambientColor = Color(10, 10, 10).glColorfv();
    m_ambientLightPos[0] = center.getY();
    m_ambientLightPos[1] = 100.f;
    m_ambientLightPos[2] = center.getX();

    // Allocate memory for phantoms lights
    for (int i = 0; i < m_phantoms.size(); i++)
    {
        m_phantomLightsPos.push_back(new float[4]);
        m_phantomDirs.push_back(new float[3]);
    }
}

void Lighting::update(long)
{
    auto dir = m_player->m_dir * 5.f;

    m_playerDir[0] = dir.getY();
    m_playerDir[1] = 0.2f;
    m_playerDir[2] = dir.getX();

    auto cellSize = m_player->m_map->getGfxCellSize();
    auto w = cellSize.getY();
    auto h = cellSize.getX();
    auto playerPos = m_player->m_animPos;

    m_playerLightPos[0] = playerPos.getY() * (float)w + w / 2.f;
    m_playerLightPos[1] = h / 2.f;
    m_playerLightPos[2] = playerPos.getX() * (float)h + h / 2.f;

    for (int i = 0; i < m_phantoms.size(); i++)
    {
        float *pos = m_phantomLightsPos[i];
        float *pdir = m_phantomDirs[i];
        auto phantom = m_phantoms[i];

        pos[0] = phantom->m_animPos.getY() * (float)w;
        pos[1] = h;
        pos[2] = phantom->m_animPos.getX() * (float)h;
        pos[3] = 1.f;

        auto phantomDir = phantom->m_dir;
        pdir[0] = phantomDir.getY();
        pdir[1] = 0.f;
        pdir[2] = phantomDir.getX();
    }
}

void Lighting::render() const
{
    unsigned int light = 0x4000;

    /* Ambient light */
    glLightiv(light, GL_POSITION, m_ambientLightPos);
    glLightfv(light, GL_AMBIENT, m_ambientColor);
    glLightfv(light, GL_DIFFUSE, Color(60, 60, 60).glColorfv());
    glEnable(light);

    /* Player light */
    light++;
    glLightfv(light, GL_POSITION, m_playerLightPos);
    glLightfv(light, GL_DIFFUSE, Color(255, 255, 255).glColorfv());
    glLightfv(light, GL_SPOT_DIRECTION, m_playerDir);

    glLightf(light, GL_SPOT_CUTOFF, 15.f);
    glLightf(light, GL_CONSTANT_ATTENUATION, .5f);
    glLightf(light, GL_LINEAR_ATTENUATION, 0);
    glLightf(light, GL_QUADRATIC_ATTENUATION, 0);
    glLightf(light, GL_SPOT_EXPONENT, 128);
    glEnable(light);

    light++;
    for (int i = 0; i < m_phantoms.size(); i++)
    {
        glLightfv(light, GL_POSITION, m_phantomLightsPos[i]);
        glLightfv(light, GL_DIFFUSE, m_phantoms[i]->m_color.whiten(1).glColorfv());
        glLightfv(light, GL_SPOT_DIRECTION, m_phantomDirs[i]);

        glLightf(light, GL_SPOT_CUTOFF, 15.f);
        glLightf(light, GL_CONSTANT_ATTENUATION, .5f);
        glLightf(light, GL_LINEAR_ATTENUATION, 0);
        glLightf(light, GL_QUADRATIC_ATTENUATION, 0);
        glLightf(light, GL_SPOT_EXPONENT, 128);

        glEnable(light);
        light++;
    }
}

void Lighting::destroy()
{
    for (int i = 0; i < m_phantoms.size(); i++)
    {
        delete m_phantomLightsPos[i];
        delete m_phantomDirs[i];
    }

    m_phantoms.clear();
    m_phantomDirs.clear();
    m_phantomLightsPos.clear();

    delete m_ambientColor;
}