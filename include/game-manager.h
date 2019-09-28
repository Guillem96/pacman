#pragma once

#include "map.h"
#include "game-object.h"

class GameManager
{
private:
    int m_height;       //> OpenGL window height
    int m_width;        //> OpenGL window width

    int m_mapHeight;    //> Pacman maze height
    int m_mapWidth;     //> Pacman maze width

    long m_lastT = 0;   //> Last time when frame was called

    std::vector<GameObject*> m_gameObjects; //> Reference to all instantiated gameobjects
    
    static void drawCallBack(); //> OpenGL render callback
    static void idleCallback();

public:
    GameManager(int windowHeight, 
                int windowWidth,
                int mapHeight,
                int mapWidth);

    void init();
    void run();
    void render();
    void update();
    void destroy();
};
