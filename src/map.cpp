#include "map.h"
#include "util.h"
#include <bits/stdc++.h> 
#include <vector>
#include "map-generator.h"

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

void Map::init()
{
    this->m_map = new Cell*[m_height * m_width];
    for (int i = 0; i < m_height * m_width; i++)
    {   
        m_map[i] = new Cell(CellType::Wall);
        m_map[i]->init();
    }

    int w = odd(m_width);
    int h = odd(m_height);

    MapGenerator mgen = MapGenerator(5, 4, w, h);
    Cell** innerMap = mgen.generate();

    for (int i = 1; i < h; i ++)
        for (int j = 1; j <= w; j++)
            (*this)(i, j)->setType(innerMap[w * (i - 1) + (j - 1)]->getType());
    
    delete innerMap;
    // for (s_width * i + (m_width - 1 - j)] = new Cell((*this)(i, j)->getType());
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