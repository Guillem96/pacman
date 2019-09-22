#pragma once

#include <iostream>
#include "game-object.h"
#include "cell.h"

class Map: public GameObject
{
private:
    int m_width;
    int m_height;

    Cell** m_map;

    void m_drawHome();
    
public:
    Map(int width, int height);
    Map();
    ~Map();

    void init() override;
    void render() const override;
    void destroy() override;

    Cell* operator()(int i, int j) const 
    {
        return m_map[m_width * i + j];
    }
};
