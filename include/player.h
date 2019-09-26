#pragma once

#include "util.h"
#include "game-object.h"

class Map;

class Player: public GameObject
{
private:
    Vector2 m_dir;
    Vector2 m_pos;
    const Map* m_map;

public:
    Player(const Map* map);
    ~Player();

    void setDirection(Vector2 dir);

    void init() override;
    void render() const override;
    void destroy();
};
