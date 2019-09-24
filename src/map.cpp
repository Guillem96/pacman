#include "map.h"
#include "util.h"
#include "map-generator.h"

#include <vector>
#include <GL/glut.h>

Map::Map(int width, int height) : m_width(width), m_height(height)
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
    /* Generate a random maze */
    m_generateMap();

    /* Append gfx data */
    std::pair<float, float> cellSize = std::pair<float, float>(
        glutGet(GLUT_WINDOW_HEIGHT) / (float)m_height,
        glutGet(GLUT_WINDOW_WIDTH) / (float)m_width);

    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            (*this)(i, j)->setPosition(Vector2(i, j));
            (*this)(i, j)->setCellSize(cellSize);
            (*this)(i, j)->setMapReference(this);
        }
    }
}

static std::vector<Cell *> validDeleteWalls(int i, int j, Map *map)
{
    std::vector<Cell *> res;

    int nWalls = 0;

    if (i > 0 && (*map)(i - 1, j)->isWall())
    {
        nWalls++;
        if ((i - 1) != 0)
            res.push_back((*map)(i - 1, j));
    }

    if (i < map->getHeight() - 1 && (*map)(i + 1, j)->isWall())
    {
        nWalls++;
        if ((i + 1) != map->getHeight() - 1)
            res.push_back((*map)(i + 1, j));
    }

    if (j > 0 && (*map)(i, j - 1)->isWall())
    {
        nWalls++;
        if ((j - 1) != 0)
            res.push_back((*map)(i, j - 1));
    }

    if (j < map->getWidth() - 1 && (*map)(i, j + 1)->isWall())
    {
        nWalls++;
        if ((j + 1) != even(map->getWidth() / 2 + 1))
            res.push_back((*map)(i, j + 1));
    }

    if (nWalls < 3)
        res.clear();

    return res;
}

void Map::m_clearDeadEnds()
{
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if ((*this)(i, j)->getType() == CellType::Path)
            {
                auto wallsToBreak = validDeleteWalls(i, j, this);
                if (wallsToBreak.size() != 0)
                {
                    int rndIdx = randomRange(0, wallsToBreak.size() - 1);
                    wallsToBreak[rndIdx]->setType(CellType::Path);
                }
            }
        }
    }
}

void Map::m_makeSymetric()
{
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width / 2 + 1; j++)
            m_map[m_width * i + (m_width - j - 1)] = new Cell((*this)(i, j)->getType());
}


void Map::m_generateMap()
{
    /* Initialize the map filled of walls */
    this->m_map = new Cell *[m_height * m_width + 1];
    for (int i = 0; i < m_height * m_width; i++)
        m_map[i] = new Cell(CellType::Wall);

    int h = m_height;
    int w = even(m_width / 2 + 1);

    DFS mgen = DFS(h, w);
    Cell **mm = mgen.generate();

    /* Copy the half generated map to our map maze*/
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            delete m_map[m_width * i + j];
            m_map[m_width * i + j] = new Cell(mm[w * i + j]->getType());
        }
    
    /* Clear the medium map */
    mgen.destroy();

    m_clearDeadEnds();
    m_drawHome();

    /* Make the maze symmetric */
    m_makeSymetric();
}

void Map::m_drawHome()
{
    int homeWidth = 3 + (m_width % 2 == 0);
    int homeHeight = 4;

    int cx = m_height / 2 - 2; /* Home entrance point */
    int cy = m_width / 2;
    
    int i = 0;
    int j = 0;

    /* Draw top wall */
    for (i = 1 + (m_width % 2 == 0); i < homeWidth; i++)
    {
        m_map[m_width * cx + cy - i]->setType(CellType::Wall);
        m_map[m_width * (cx - 1) + cy - i]->setType(CellType::Path);
    }
    
    // Top left corner to path
    m_map[m_width * (cx - 1) + cy - i]->setType(CellType::Path);
    m_map[m_width * (cx - 1) + cy - i - 1]->setType(CellType::Path);

    /* Draw left vertical wall*/
    for (j = 0; j <= homeHeight; j++)
    {
        m_map[m_width * (cx + j) + cy - i]->setType(CellType::Wall);
        m_map[m_width * (cx + j) + cy - i - 1]->setType(CellType::Path);
    }

    /* Draw bottom wall */
    for (i = i; i > -(m_width % 2 == 1); i--)
    {
        m_map[m_width * (cx + j - 1) + cy - i ]->setType(CellType::Wall);
        m_map[m_width * (cx + j) + cy - i]->setType(CellType::Path);
    }

    // Bottom left corner to path
    m_map[m_width * (cx + j) + cy - homeWidth - 1]->setType(CellType::Path);

    /* Fill the home with corridor cells */
    for (int i = 1; i < homeHeight; i++)
        for (int j = 0; j < homeWidth; j++)
            m_map[m_width * (cx + i) + cy - j]->setType(CellType::Path);

    /* Clear the entrance from possible blocking walls */
    if (m_width % 2 != 0)
        for (int i = 0; i < 2; i++)
            m_map[m_width * (cx - i) + cy]->setType(CellType::Path);
    else
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                m_map[m_width * (cx - j) + cy - i]->setType(CellType::Path);
}   

void Map::textRender() const
{
    for (int i = 0; i < m_height * m_width; i++)
    {
        m_map[i]->textRender();
        if ((i + 1) % m_width == 0)
            std::cout << std::endl;
    }
}

void Map::render() const
{
    for (int i = 0; i < m_height * m_width; i++)
        m_map[i]->render();
}

void Map::destroy()
{
    for (int i = 0; i < m_height * m_width; i++)
        delete m_map[i];

    delete m_map;
}

int Map::getHeight() const { return m_height; }
int Map::getWidth() const { return m_width; }
Cell* Map::operator()(int i, int j) const { return m_map[m_width * i + j]; }