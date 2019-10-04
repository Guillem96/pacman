#pragma once

#include "game-object.h"
#include "util.h"

class Observer: public GameObject
{
private:
    int m_alpha;
    int m_beta;
    int m_radius;

    Vector2<> m_dir;

public:
    Observer(int alpha, int beta, int radius);
    ~Observer();

    void init() override;
    void update(long) override;
    void render() const override;
    void destroy() override;

    void setDirection(Vector2<> dir);
};

