#pragma once

#include "util.h"
#include "game-object.h"

class Map;

class Player: public GameObject
{
private:
    Vector2<> m_dir;
    Vector2<> m_pos;
    const Map* m_map;

    float m_v;
    Vector2<> m_posOffset;
    float m_animDuration = .5f;
    float m_remaining;

    void m_initMovement();

public:
    Player(const Map* map);
    ~Player();

    void setDirection(Vector2<> dir);

    void init() override;
    void render() const override;
    void update(long deltaTime) override;
    void destroy();
};
