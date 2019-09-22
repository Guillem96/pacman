#pragma once

#include <GL/glut.h>
#include "game-object.h"

enum CellType {
    Wall,
    Path,
};

class Cell: public GameObject
{
private:
     CellType m_type;
     
public:
    Cell(CellType type);
    ~Cell();

    void init() override;
    void render() const override;
    void destroy() override;

    const CellType& getType();
    void setType(CellType type);
};