#pragma once

#include "util.h"
#include "game-object.h"
#include "lighting.h"
#include "map.h"

class Phantom;

class Player: public GameObject
{
private:
    Vector2<float> m_animDir;
    Vector2<float> m_animPos;

    Vector2<> m_dir;
    Vector2<> m_pos;

    const Map* m_map;
    std::vector<const Phantom*> m_phantoms;

    long m_animDuration = 200;
    long m_remaining = 200;

    float m_score = 0;

    void m_initMovement();
    void m_movementLogic(long deltaTime);
    void m_gameRulesLogic(long deltaTime);

public:
    friend Lighting;

    Player(const Map* map, std::vector<const Phantom*> phantoms);
    ~Player();

    void setDirection(Vector2<> dir);

    void init() override;
    void render() const override;
    void update(long deltaTime) override;
    void destroy();

    float getScore() const { return m_score; }
    void addScore(float score) { m_score += score; }

    const Vector2<>& getPosition() const { return m_pos; }
};
