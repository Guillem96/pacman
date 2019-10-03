#pragma once

#include "game-object.h"

class Observer: public GameObject
{
private:
    int m_alpha;
    int m_beta;
    int m_radius;

public:
    Observer(int alpha, int beta, int radius);
    ~Observer();

    void init() override;
    void update(long) override;
    void render() const override;
    void destroy() override;

};

