#pragma once

#include "util.h"
#include "game-object.h"

class Map;

class Player: public GameObject
{
private:
    Vector2<float> m_animDir;
    Vector2<float> m_animPos;

    Vector2<> m_dir;
    Vector2<> m_pos;

    const Map* m_map;

    long m_animDuration = 200;
    long m_remaining = 200;

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
