#pragma once

#include <vector>

class Map;
class Player;
class Phantom;
class Observer;
class GameObject;
class Lighting;
class TextureManager;

class Agent;
class FeatureExtractor;

class GameManager
{
private:
    int m_windowId;     //> OpenGL window ID
    int m_height;       //> OpenGL window height
    int m_width;        //> OpenGL window width

    int m_mapHeight;    //> Pacman maze height
    int m_mapWidth;     //> Pacman maze width

    long m_lastT = 0;   //> Last time when frame was called

    std::vector<GameObject*> m_gameObjects; //> Reference to all instantiated gameobjects
    
    static void drawCallBack(); //> OpenGL render callback
    static void idleCallback();
    static void keyboardCallback(unsigned char c, int x, int y);

    Player* m_player;
    Map* m_map;
    Observer* m_observer;
    TextureManager* m_textureManager;
    Lighting* m_lighting;
    std::vector<const Phantom*> m_phantoms;

    // Reinforcement learning variables
    FeatureExtractor* m_fe;
    Agent* m_agent;

    int m_episodes = 0;
    const int MAX_EPISODES = 4;
    const float LIVING_REWARD = -0.5;

    void m_initObjects();
    void m_restart();
public:
    GameManager(int windowHeight, 
                int windowWidth,
                int mapHeight,
                int mapWidth);

    void init();
    void run();

    void render();
    void input(unsigned char c, int x, int y);
    void update();
    
    void destroy();
};
