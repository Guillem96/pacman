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

    void m_drawHome();      //> Draws phantoms house at the center of tha maze
    void m_generateMap();   //> Generates the whole map using DFS
    void m_clearDeadEnds(); //> Clears paths with a single exit
    void m_makeSymetric();  //> Mirrors the map on the y axis

public:
    Map(int width, int height);
    Map();
    ~Map();

    void init() override;
    void render() const override;
    void textRender() const;
    void destroy() override;

    int getHeight() const;
    int getWidth() const;

    std::pair<float, float> getGfxCellSize() const;

    Cell* operator()(int i, int j) const;
};
