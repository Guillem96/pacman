#include "map.h"
#include "util.h"
#include <bits/stdc++.h> 
#include <vector>

typedef std::pair<Vector2, Vector2> edge_t;

Map::Map(int width, int height): m_width(width), m_height(height)
{
}

Map::Map()
{
}

Map::~Map()
{
}

static Vector2 step(Vector2 vector, int width, int height)
{
    std::vector<Vector2> valid;

    Vector2 right(1, 0);
    Vector2 left(-1, 0);
    Vector2 up(0, 1);
    Vector2 down(0, -1);

    if (vector.getX() == 0 && vector.getX() < height - 1) 
        valid.push_back(right);
    
    if (vector.getX() > 0 && vector.getX() < height - 1)
        valid.push_back(left);

    if (vector.getY() >= 0 && vector.getY() < width - 1)
        valid.push_back(up);
    
    if (vector.getY() > 0 && vector.getY() < width - 1)
        valid.push_back(down);

    int rndIdx = rand() % valid.size();
    return vector + valid[rndIdx];
}

static bool all(bool* a, int len) 
{
    for (int i = 0; i < len; i++)
        if(!a[i])
            return false;
    return true;
}

static edge_t hunt(bool* maze, int width, int height)
{
    for (int x = 0; x < height; x++)
    {
        for(int y = 0; y < width; y++)
        {
            if (!maze[width * x + y])
            {
                if (x < height - 1 && maze[width * (x + 1) + y])
                    return edge_t(Vector2(x + 1, y), Vector2(x, y));
                
                if (x > 0 && maze[width * (x - 1) + y])
                    return edge_t(Vector2(x - 1, y), Vector2(x, y));
                
                if (y < width - 1 && maze[width * x + (y + 1)])
                    return edge_t(Vector2(x, y + 1), Vector2(x, y));

                if (y > 0 && maze[width * x + (y - 1)])
                    return edge_t(Vector2(x, y - 1), Vector2(x, y));
            }
        }
    }
    return edge_t(Vector2(), Vector2());
}

static std::vector<edge_t> killHunt(int width, int height) 
{
    bool* maze = new bool[width * height];
    for (int i = 0; i < width * height; i++)
        maze[i] = false;

    std::vector<edge_t> edges;
    Vector2 pos = Vector2::getRandom(height, width);
    maze[width * pos.getX() + pos.getY()] = true;
    while(!all(maze, width * height))
    {
        Vector2 newPos = step(pos, width, height);
        if (maze[width * newPos.getX() + newPos.getY()])
        {
            auto huntEdge = hunt(maze, width, height);
            edges.push_back(huntEdge);
            pos = huntEdge.second;
            maze[width * pos.getX() + pos.getY()] = true;
        }
        else 
        {
            maze[width * newPos.getX() + newPos.getY()] = true;
            edges.push_back(edge_t(pos, newPos));
            pos = newPos;
        }
    }

    delete maze;
    return edges;
}

void Map::init()
{
    this->m_map = new Cell*[m_height * m_width];
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j ++)
        {
            if (i == 0 || j == 0 || i == m_height - 1 || j == m_width - 1)
                m_map[i * m_width + j] = new Cell(CellType::Wall);
            else
                m_map[i * m_width + j] = new Cell(CellType::Path);
            m_map[i * m_width + j]->init();
        }
    }
    auto edges = killHunt(m_width - 1, m_height - 1);

    for (int i = 0; i < edges.size(); i++) 
    {
        auto p = edges.at(i);
        auto dir = p.second - p.first;

        if (dir == Vector2(1, 0) || dir == Vector2(-1, 0))
        {
            (*this)(p.second.getX() + 1, p.second.getY())->setType(CellType::Path);
        }
        std::cout << '(' << p.first.getX() << ',' << p.first.getY() << ") -> (" << p.second.getX() << ',' << p.second.getY() << ')' << std::endl;
    }
}

void Map::render() const
{
    for (int i = 0; i < m_height * m_width; i++)
    {
        m_map[i]->render();
        if ((i + 1) % m_width == 0)
            std::cout << std::endl;
    }
};

void Map::destroy()
{
    for (int i = 0; i < m_height * m_width; i++)
        delete m_map[i];
        
    delete m_map;
}