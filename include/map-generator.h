#pragma once

#include <vector>
#include <iostream>
#include "cell.h"
#include "util.h"

class MapGenerator;

class Builder
{
private:
    int m_moved = 0;

    int m_minDist;
    int m_maxDist;
    int m_distance;
    
    bool m_dead = false;

    MapGenerator* m_generator;

    Vector2 m_direction;
    Vector2 m_initialPos;
    Vector2 m_prevDirection;
    Vector2 m_position;

    void m_generateMinMaxDist();
    void m_changeDirection();
    bool m_isPath(const Vector2& pos);
    Vector2 m_closerDirection(bool horizontal);

    Cell** maze();
    int height();
    int width();
    
public:
    Builder(const Vector2& spawn,
            int minDist,
            int maxDist,
            const Vector2& initialDir, 
            MapGenerator* generator);

    bool step();
    bool isDead() const;
};


class MapGenerator
{
private:
    Cell** m_maze;
    std::vector<Builder*> m_builders;
    std::vector<Vector2> m_directions = {
        Vector2(1, 0),
        Vector2(-1, 0),
        Vector2(0, 1),
        Vector2(0, -1)
    };
    std::vector<Vector2> m_spawners;

    int m_width;
    int m_height;

    
    bool m_end();
    std::vector<Vector2> m_validDirections(const Vector2& pos);

public:
    MapGenerator(int nSpawners, 
                int nBuildersSpawn, 
                int m_mazeWidth,
                int m_mazeHeight);

    Cell** generate();
    int getHeight();
    int getWidth();
    Cell** getMaze();
    void visit(const Vector2& pos);
    const std::vector<Vector2>& getSpawners();
};

