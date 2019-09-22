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
    this->m_map = new Cell*[m_height * m_width + 1];
    // for (int i = 0; i < m_height * m_width; i++)
    // {   
    //     m_map[i] = new Cell(CellType::Path);
    //     m_map[i]->init();
    // }
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (i == 0 || j == 0 || i == m_height - 1 || j == m_width - 1)
            {
                m_map[m_width * i + j] = new Cell(CellType::Wall);
            } else m_map[m_width * i + j] = new Cell(CellType::Path);
            m_map[m_width * i + j]->init();
        }
    }

    int h = odd(m_height);
    int w = even(m_width / 2 + 1);

    DFS mgen = DFS(h, w);
    Cell** mm = mgen.generate();
    
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            m_map[m_width * i + j] = mm[w * i + j];
            
    m_drawHome();

    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width / 2; j++)
            m_map[m_width * i + (m_width - j - 1)] = new Cell((*this)(i, j)->getType());
}  

void Map::m_drawHome()
{
    int cx = m_height / 2 - 2;  /* Home entrance point */
    int cy = m_width % 2 == 0 ? m_width / 2 : m_width / 2 - 1;

    /* Draw top wall */
    for (int i = (m_width % 2 == 0); i < 3 + (m_width % 2 == 0); i++)
        m_map[m_width * cx + cy - i - (m_width % 2 == 0)]->setType(CellType::Wall);

    /* Draw left vertical wall*/
    for (int i = 0; i < 5; i++)
        m_map[m_width * (cx + i) + cy - 2 - 2 * (m_width % 2 == 0)]->setType(CellType::Wall);

    /* Draw bottom wall */
    for (int i = 0; i < 4; i++)
        m_map[m_width * (cx + 4) + cy - i + (m_width % 2 == 1)]->setType(CellType::Wall);

    /* Fill the home with corridor cells */
    for (int i = 1; i <= 3; i++)
        for (int j = 0; j < 4; j++)
            m_map[m_width * (cx + i) + j + cy - 1 - 2 * (m_width % 2 == 0)]->setType(CellType::Path);
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