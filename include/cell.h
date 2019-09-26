#pragma once

#include "game-object.h"
#include "util.h"

class Map;

enum CellType {
    Wall,
    Path,
};

class Cell: public GameObject
{
private:
    Vector2 m_pos;
    CellType m_type;
    const Map* m_map;
    
public:
    Cell(CellType type);
    ~Cell();

    void init() override;
    void render() const override;
    void destroy() override;

    /* Renders the cell in text mode */
    void textRender() const;
    
    const Vector2& getPosition();
    void setPosition(Vector2 pos);

    const CellType& getType();
    void setType(CellType type);

    /* Is the cell a wall? */
    bool isWall();
    
    /* Set a reference to the currently instanciated map */
    void setMapReference(const Map* map);
};