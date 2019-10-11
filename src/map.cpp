#include "map.h"
#include "util.h"
#include "map-generator.h"
#include "texture.h"

#include <vector>
#include <GL/glut.h>

Map::Map(int width, int height,
         const Texture* wall, 
         const Texture* ground) 
         : m_width(width), m_height(height), m_wallTex(wall),
         m_groundTex(ground)
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

    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            (*this)(i, j)->setPosition(Vector2<>(i, j));
            (*this)(i, j)->setMapReference(this);
        }
    }
}

static std::vector<Cell *> validDeleteWalls(int i, int j, Map *map)
{
    std::vector<Cell *> adjacent = map->getAdjacent(Vector2<>(i, j));
    std::vector<Cell *> res;
    int nWalls = 0;

    for (int i = 0; i < adjacent.size(); i++)
    {
        if (adjacent[i]->isWall())
        {
            nWalls++;
            auto pos = adjacent[i]->getPosition();
            /* Check if it is a breakable wall */
            if (pos.getX() > 0 && pos.getX() < map->getHeight() - 1 &&
                pos.getY() > 0 && pos.getY() < even(map->getWidth() / 2 + 1))
                res.push_back(adjacent[i]);
        }
    }
    /* If we are sorounded by less than three walls, none of the seen walls is breakable */
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
                   randomChoice(wallsToBreak)->setType(CellType::Path);
            }
        }
    }
}

void Map::m_makeSymetric()
{
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width / 2 + 1; j++)
            m_map[m_width * i + (m_width - j - 1)] = new Cell((*this)(i, j)->getType(), 
                                                              (*this)(i, j)->hasFood());
}


void Map::m_generateMap()
{
    /* Initialize the map filled of walls */
    this->m_map = new Cell *[m_height * m_width + 1];
    for (int i = 0; i < m_height * m_width; i++)
        m_map[i] = new Cell(CellType::Wall, true);

    int h = m_height - (m_height % 2 == 0);
    int w = even(m_width / 2 + 1);

    DFS mgen = DFS(h, w);
    Cell **mm = mgen.generate();

    /* Copy the half generated map to our map maze*/
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            delete m_map[m_width * i + j];
            m_map[m_width * i + j] = new Cell(mm[w * i + j]->getType(), true);
            m_map[m_width * i + j]->setPosition(Vector2<>(i, j));
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
    int homeWidth = 3;
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
        {
            m_map[m_width * (cx + i) + cy - j]->setType(CellType::Path);
            m_map[m_width * (cx + i) + cy - j]->eat();
        }
    /* Clear the entrance from possible blocking walls */
    if (m_width % 2 != 0)
        for (int i = 0; i < 2; i++)
        {
            m_map[m_width * (cx - i) + cy]->setType(CellType::Path);
            m_map[m_width * (cx + i) + cy]->eat();
        }
    else
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
            {
                m_map[m_width * (cx - j) + cy - i]->setType(CellType::Path);
                m_map[m_width * (cx + i) + cy - j]->eat();
            }
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

Vector2<float> Map::getGfxCellSize() const
{
    return Vector2<float>(
        glutGet(GLUT_WINDOW_HEIGHT) / (float)m_height,
        glutGet(GLUT_WINDOW_WIDTH) / (float)m_width);
}

std::vector<Cell*> Map::getAdjacent(const Vector2<>& v) const
{
    std::vector<Cell*> res;
    if (v.getX() > 0)
        res.push_back((*this)(v + Vector2<>::up));
    
    if (v.getX() < m_height - 1)
        res.push_back((*this)(v + Vector2<>::down));
    
    if (v.getY() > 0)
        res.push_back((*this)(v + Vector2<>::left));

    if (v.getY() < m_width - 1)
        res.push_back((*this)(v + Vector2<>::right));  

    return res;
}


int Map::getHeight() const { return m_height; }
int Map::getWidth() const { return m_width; }
Cell* Map::operator()(int i, int j) const { return m_map[m_width * i + j]; }    
Cell* Map::operator()(const Vector2<>& v) const { return m_map[m_width * v.getX() + v.getY()]; }
