#include "map-generator.h"
#include "util.h"
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <chrono> 

Builder::Builder(const Vector2 &spawn,
                 int minDist,
                 int maxDist,
                 const Vector2 &initialDir,
                 MapGenerator *generator): m_position(spawn),
                                           m_initialPos(spawn),
                                            m_minDist(minDist),
                                            m_maxDist(maxDist),
                                            m_direction(initialDir),
                                            m_generator(generator)
{
    m_generateMinMaxDist();
}

static Vector2 getDirection(int dir)
{
    if (dir == 0)
        return Vector2(1, 0);
    else if (dir == 1)
        return Vector2(0, 1);
    else if (dir == 2)
        return Vector2(-1, 0);
    else if (dir == 3)
        return Vector2(0, -1);
}

bool Builder::isDead() const
{
    return m_dead;
}

Vector2 Builder::m_closerDirection(bool horizontal)
{
    auto spawns = m_generator->getSpawners();
    int weight = 0;
    for (int i = 0; i < spawns.size(); i++)
    {
        if (spawns[i] != m_initialPos)
        {
            int sval = !horizontal ? spawns[i].getX() : spawns[i].getY();
            int cval =  !horizontal ? m_position.getX() : m_position.getY();
            weight += sign(sval - cval);
        }
    }
    return horizontal 
                ? (weight > 0 ? Vector2(0, 1) : Vector2(0, -1))
                : (weight > 0 ? Vector2(1, 0) : Vector2(-1, 0));
}

void Builder::m_generateMinMaxDist()
{
    m_distance = (rand() % (m_maxDist - m_minDist)) + m_minDist;

    if (m_direction.getY() == 1)
        m_distance = min(m_distance, width() - 1 - m_position.getY());
    else if (m_direction.getY() == -1)
        m_distance = min(m_distance, m_position.getY());
    else if (m_direction.getX() == 1)
        m_distance = min(m_distance, height() - 1 - m_position.getX());
    else if (m_direction.getX() == -1)
        m_distance = min(m_distance, m_position.getX());
    
    even(m_distance);
    m_moved = 0;
}

void Builder::m_changeDirection()
{
    if (m_direction.getX() != 0)
    {
        bool canGoLeft = m_position.getY() >= 2;
        bool canGoRight = width() - 1 - m_position.getY() >= 2;
        
        if (canGoLeft && canGoRight)
            m_direction = m_closerDirection(true);
        else if (canGoLeft)
            m_direction = Vector2(0, -1);
        else
            m_direction = Vector2(0, 1);
    } 
    else if (m_direction.getY() != 0)
    {
        bool canGoUp = m_position.getX() >= 2;
        bool canGoDown = height() - 1 - m_position.getX() >= 2;
        
        if (canGoUp && canGoDown)
            m_direction = m_closerDirection(false);
        else if (canGoUp)
            m_direction = Vector2(-1, 0);
        else
            m_direction = Vector2(1, 0);
    }        
}

bool Builder::m_isPath(const Vector2 &pos)
{
    return maze()[width() * pos.getX() + pos.getY()]->getType() == CellType::Path;
}

bool Builder::step()
{
    if (m_dead)
        return true;

    if (m_moved >= m_distance)
    {
        m_changeDirection();
        m_generateMinMaxDist();
        return false;
    }

    m_position = m_direction + m_position;

    /* If the the current cell was already visited by another builder we die */
    if (m_isPath(m_position))
    {
        m_dead = true;
        return true;
    }

    /* If we pass through a cell we mark it as a path */
    m_generator->visit(m_position);
    m_moved++;

    return false;
}

MapGenerator::MapGenerator(int nSpawners,
                           int nBuildersSpawn,
                           int m_mazeWidth,
                           int m_mazeHeight) : m_height(m_mazeHeight), m_width(m_mazeWidth)
{
    auto rng = std::default_random_engine {};
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    
    m_maze = new Cell *[m_height * m_width];
    for (int i = 0; i < m_height * m_width; i++)
    {
        m_maze[i] = new Cell(CellType::Wall);
        m_maze[i]->init();
    }

    int *col_idx = range(0, m_width + 1, 2);
    int *row_idx = range(0, m_height + 1, 2);

    shuffle(col_idx, (int)(m_width / 2 + 1));
    shuffle(row_idx, (int)(m_height / 2 + 1));

    std::cout << "Width: " << m_width << " Height: " << m_height << std::endl;
    std::cout << "Spawners: " << min(m_width / 2 + 1, m_height / 2 + 1) << std::endl;

    for (int i = 0; i < min(m_width / 2, m_height / 2); i++)
    {
        Vector2 spawnPos = Vector2(row_idx[i], col_idx[i]);
        visit(spawnPos);
        m_spawners.push_back(spawnPos);

        std::vector<Vector2> validDirs = m_validDirections(spawnPos);
        std::shuffle(std::begin(validDirs), std::end(validDirs), rng);
        int count = min(validDirs.size(), randomRange(2, 4));
        std::cout << count << std::endl;
        for (int j = 0; j < count; j++)
        {
            m_builders.push_back(new Builder(spawnPos,
                                             2,
                                             min(m_width, m_height),
                                             validDirs[j],
                                             this));
        }
    }
}

std::vector<Vector2> MapGenerator::m_validDirections(const Vector2& pos)
{
    bool canGoLeft = pos.getY() >= 3;
    bool canGoRight = m_width - 1 - pos.getY() >= 2;
    bool canGoUp = pos.getX() >= 3;
    bool canGoDown = m_height - 1 - pos.getX() >= 3;

    std::vector<Vector2> valid;
    if (canGoLeft)
        valid.push_back(Vector2(0, -1));
    
    if (canGoRight)
        valid.push_back(Vector2(0, 1));
        
    if (canGoUp)
        valid.push_back(Vector2(-1, 0));
    
    if (canGoDown)
        valid.push_back(Vector2(1, 0));
        
    return valid;
}


Cell **MapGenerator::generate()
{
    while (!m_end())
        for (int i = 0; i < m_builders.size(); i++)
            m_builders[i]->step();

    for (int i = 0; i < m_builders.size(); i++)
        delete m_builders[i];

    return m_maze;
}

bool MapGenerator::m_end()
{
    for (int i = 0; i < m_builders.size(); i++)
        if (!m_builders[i]->isDead())
            return false;
    return true;
}

void MapGenerator::visit(const Vector2& pos)
{
    m_maze[m_width * pos.getX() + pos.getY()]->setType(CellType::Path);
}

int MapGenerator::getHeight() { return m_height; }
int MapGenerator::getWidth() { return m_width; }
Cell** MapGenerator::getMaze() { return m_maze; }
const std::vector<Vector2>&  MapGenerator::getSpawners() { return m_spawners; }

Cell** Builder::maze() { return m_generator->getMaze(); }
int Builder::height() { return m_generator->getHeight(); }
int Builder::width() { return m_generator->getWidth(); }
