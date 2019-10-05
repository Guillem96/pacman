#pragma once

#include "game-object.h"
#include "util.h"
#include "texture.h"

class Map;

enum CellType {
    Wall,
    Path,
};

class Cell: public GameObject
{
private:
    Vector2<> m_pos;
    CellType m_type;
    const Map* m_map;

    bool m_food;

public:
    Cell(CellType type);
    Cell(CellType type, bool hasFood);

    ~Cell();

    void init() override;
    void render() const override;
    void destroy() override;

    /* Renders the cell in text mode */
    void textRender() const;
    
    const Vector2<>& getPosition();
    void setPosition(Vector2<> pos);

    const CellType& getType();
    void setType(CellType type);

    /* Is the cell a wall? */
    bool isWall() const;
    
    /* Set a reference to the currently instanciated map */
    void setMapReference(const Map* map);

    /* Empties the cell of food */
    void eat();

    /* Fills the cell with food */
    void fill();

    bool hasFood() const;
};