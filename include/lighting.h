#pragma once

#include <map>
#include <GL/glut.h>

#include "game-object.h"
#include "util.h"

class Player;

class Lighting : public GameObject
{
private:
    const Player* m_player;

    int m_ambientLightPos[4] = {0, 0, 0, 1};
    float* m_ambientColor;

    float m_playerDir[3];
    float m_playerLightPos[4] = {0, 0, 0, 1};

public:
    Lighting(const Player* player);
    ~Lighting();

    void init() override;
    void update(long);
    void render() const override;
    void destroy() override;
};
