#pragma once

#include <map>
#include <vector>
#include <GL/glut.h>

#include "util.h"
#include "game-object.h"

class Player;
class Phantom;

class Lighting : public GameObject
{
private:
    const Player* m_player;

    int m_ambientLightPos[4] = {0, 0, 0, 1};
    float* m_ambientColor;

    float m_playerDir[3];
    float m_playerLightPos[4] = {0, 0, 0, 1};

    std::vector<float*> m_phantomLightsPos;
    std::vector<float*> m_phantomDirs;
    std::vector<const Phantom*> m_phantoms;

public:
    Lighting(const Player* player, 
             std::vector<const Phantom*> phantoms);
    ~Lighting();

    void init() override;
    void update(long);
    void render() const override;
    void destroy() override;
};
