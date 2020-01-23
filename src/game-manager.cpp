#include "game-manager.h"

#include "map.h"
#include "player.h"
#include "phantom.h"
#include "lighting.h"
#include "game-object.h"
#include "texture-manager.h"
#include "observer.h"

#include "feature-extractor.h"
#include "agent.h"

#include <GL/glut.h>
#include <math.h>

GameManager *g_gameManager;

static Vector2<> getDir(Actions a);
static bool isTerminal(const state_t& state);
static state_t buildState(
    const Player* p, const Map* m, std::vector<const Phantom*> ps);

GameManager::GameManager(int windowHeight,
                         int windowWidth,
                         int mapHeight,
                         int mapWidth) : m_height(windowHeight),
                                         m_width(windowWidth),
                                         m_mapWidth(mapWidth),
                                         m_mapHeight(mapHeight) {}

void GameManager::init()
{
    /* Init opengl stuff */
    int argc = 0;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(m_width, m_height);
    m_windowId = glutCreateWindow("Pacman");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glutDisplayFunc(drawCallBack);
    glutIdleFunc(idleCallback);
    glutKeyboardFunc(keyboardCallback);

    m_initObjects();

    /* Initialize variables for reinforcement learning */
    m_fe = new FeatureExtractor();
    m_agent = new Agent(0.2, 0.3, 0.8, m_fe);
    m_agent->init();
}

void GameManager::run()
{
    glutMainLoop();
}

void GameManager::render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    m_observer->render();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width * 0.5,
            m_width * 0.5,
            -m_height * 0.5,
            m_height * 0.5,
            10, 2000);

    glClearColor(0.13, 0.13, 0.13, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | 
            GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    /* Translate all objects */
    glMatrixMode(GL_MODELVIEW); 
    glTranslatef(-m_width * 0.5,  0, -m_height * 0.5);
    
    for (int i = 0; i < m_gameObjects.size(); i++)
        m_gameObjects[i]->render();
    
    glutSwapBuffers();
}

void GameManager::update()
{
    long t = glutGet(GLUT_ELAPSED_TIME);

    if (m_lastT == 0)
        m_lastT = t;
    else
    {
        // Take an action based on the current state
        state_t state = buildState(m_player, m_map, m_phantoms);
        auto a = m_agent->takeAction(state);
        m_player->setDirection(getDir(a));
        
        m_observer->update(t - m_lastT);
        for (int i = 0; i < m_gameObjects.size(); i++)
            m_gameObjects[i]->update(t - m_lastT);
        
        // Observe the game state after taking the action
        // and update the agent with the experience
        state_t nextState = buildState(m_player, m_map, m_phantoms);
        float reward = nextState.player.getScore() - state.player.getScore();
        m_agent->update(state, a, nextState, reward);
        bool isTerminalState = isTerminal(nextState);
        
        if (isTerminalState)
        {
            m_lastT = 0;
            m_restart();
            std::cout << "Episode " << m_episodes <<" end" << std::endl;

            m_episodes++;
            if (m_episodes == MAX_EPISODES + 1)
            {
                m_agent->stopLearning();
                std::cout << "Agent will stop learning" << std::endl;
            }
        } else m_lastT = t;
    }

    glutPostRedisplay();
}

void GameManager::input(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 27: //> Escape key
        this->destroy();
        m_agent->destroy();

        delete m_agent;
        delete m_fe;
        glutDestroyWindow(m_windowId);
        exit(0);
        break;
    case 'r':
        std::cout << "restaring..." << std::endl;
        m_restart();
        break;
    case 'j':
        m_observer->setDirection(Vector2<>::left);
        break;
    case 'l':
        m_observer->setDirection(Vector2<>::right);
        break;
    case 'i':
        m_observer->setDirection(Vector2<>::up);
        break;
    case 'k':
        m_observer->setDirection(Vector2<>::down);
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void GameManager::destroy()
{
    m_observer->destroy();
    delete m_observer;

    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->destroy();
        delete m_gameObjects[i];
    }
    m_gameObjects.clear();
    m_phantoms.clear();
    
    m_textureManager->destroy();
    delete m_textureManager;
}

void GameManager::m_initObjects()
{
     /* Load textures */
    m_textureManager = new TextureManager();
    m_textureManager->loadTexture("assets/walls.jpg", "wall", 64);
    m_textureManager->loadTexture("assets/ground.jpg", "ground", 64);

    m_observer = new Observer(90, 30, 450);
    
    m_map = new Map(m_mapWidth, m_mapHeight,
                (*m_textureManager)["wall"],
                (*m_textureManager)["ground"]);


    m_phantoms.push_back(new Phantom(m_map, Color::red));
    m_phantoms.push_back(new Phantom(m_map));
    m_phantoms.push_back(new Phantom(m_map, Color::pink));
    m_phantoms.push_back(new Phantom(m_map, Color::yellowPacman));

    
    m_player = new Player(m_map, m_phantoms);
    auto lightManager = new Lighting(m_player, m_phantoms);

    m_gameObjects.push_back(m_map);
    m_gameObjects.push_back(m_player);
    m_gameObjects.push_back(lightManager);
    
    /* Append phantoms */
    for(int i = 0; i < m_phantoms.size(); i++)
        m_gameObjects.push_back((GameObject*)m_phantoms[i]);

    for (int i = 0; i < m_gameObjects.size(); i++)
        m_gameObjects[i]->init();

    g_gameManager = this;
}

void GameManager::m_restart()
{
    m_lastT = 0;
    destroy();
    m_initObjects();
}

void GameManager::drawCallBack() { g_gameManager->render(); }
void GameManager::idleCallback() { g_gameManager->update(); }
void GameManager::keyboardCallback(unsigned char c, int x, int y) { g_gameManager->input(c, x, y); }

static state_t buildState(
    const Player* p, const Map* m, std::vector<const Phantom*> ps)
{
    std::vector<Phantom> phantoms;
    for (auto p: ps)
        phantoms.push_back(*p);

    return {
        (*m),
        (*p),
        phantoms
    };
}

static Vector2<> getDir(Actions a)
{
    if (a == Actions::Up)
        return Vector2<>::up;
    
    if (a == Actions::Down)
        return Vector2<>::down;
    
    if (a == Actions::Left)
        return Vector2<>::left;
    
    if (a == Actions::Right)
        return Vector2<>::right;
    
    return Vector2<>(0);
}

static bool isTerminal(const state_t& state)
{
    // If phantom chases the pacman, the game ends
    for (auto p: state.phantoms)
    {
        if (p.getPosition() == state.player.getPosition())
            return true;
    }

    // If any food left the episode continues
    int w = state.map.getWidth();
    int h = state.map.getHeight();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; i++)
        {
            if (state.map(j, i)->hasFood())
                return false;
        }
    }

    // End because pacman has eaten all foods
    return true;
}
