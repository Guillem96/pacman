#pragma once

#include "map.h"

class GameManager
{
private:
    int m_height;       //> OpenGL window height
    int m_width;        //> OpenGL window width

    int m_mapHeight;    //> Pacman maze height
    int m_mapWidth;     //> Pacman maze width

    Map* m_map;         //> Map reference
    
    static void drawCallBack(); //> OpenGL render callback

public:
    GameManager(int windowHeight, 
                int windowWidth,
                int mapHeight,
                int mapWidth);

    void init();
    void run();
    void render();
    void destroy();
};
