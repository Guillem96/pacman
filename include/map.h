#pragma once

#include <iostream>
#include "game-object.h"
#include "cell.h"

class Texture;

class Map: public GameObject
{
private:
    int m_width;
    int m_height;
    Cell** m_map;
    
    const Texture* m_wallTex;
    const Texture* m_groundTex;

    void m_drawHome();      //> Draws phantoms house at the center of tha maze
    void m_generateMap();   //> Generates the whole map using DFS
    void m_clearDeadEnds(); //> Clears paths with a single exit
    void m_makeSymetric();  //> Mirrors the map on the y axis

public:
    friend Cell;
    
    Map(int width, int height,
        const Texture* wall, 
        const Texture* ground);
    Map();
    ~Map();

    void init() override;
    void render() const override;
    void textRender() const;
    void destroy() override;

    int getHeight() const;
    int getWidth() const;

    Vector2<float> getGfxCellSize() const;
    std::vector<Cell*> getAdjacent(const Vector2<>& v) const;

    Cell* operator()(int i, int j) const;
    Cell* operator()(const Vector2<>& v) const;
};
