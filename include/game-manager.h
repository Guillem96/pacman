#pragma once

#include "map.h"

class GameManager
{
private:
    int m_height;
    int m_width;

    int m_mapHeight;
    int m_mapWidth;

    Map* m_map;
    
    static void drawCallBack();

public:
    GameManager(int windowHeight, 
                int windowWidth,
                int mapHeight,
                int mapWidth);

    void init();
    void render();
    void destroy();
};
