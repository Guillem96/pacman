#pragma once

#include "util.h"
#include "game-object.h"

class Map;

class Phantom: public GameObject
{
private:
    Vector2<float> m_animDir;
    Vector2<float> m_animPos;

    Vector2<> m_dir;
    Vector2<> m_pos;
    
    Color m_color = Color::cyan;

    const Map *m_map;

    long m_animDuration = 200;
    long m_remaining = 200;

    bool m_userControl = false;
    
    void m_initMovement();

public:
    Phantom(const Map* map);
    Phantom(const Map* map, Color color);

    ~Phantom();

    void setDirection(Vector2<> dir);
    void toogleUserControl();
    
    void init() override;
    void render() const override;
    void update(long deltaTime) override;
    void destroy();
};
