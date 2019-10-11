#pragma once

#include <map>
#include <GL/glut.h>

#include "game-object.h"
#include "util.h"

class Lighting : public GameObject
{
private:
    int m_ambientLightPos[3] = {100, 75, 1};
    float* m_ambientColor;

public:
    Lighting();
    ~Lighting();

    void init() override;
    void update(long);
    void render() const override;
    void destroy() override;
};
