#include "game-manager.h"

#include "map.h"
#include "player.h"
#include "phantom.h"

#include <GL/glut.h>

GameManager *g_gameManager;

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(m_width, m_height);
    m_windowId = glutCreateWindow("Pacman");

    glutDisplayFunc(drawCallBack);
    glutIdleFunc(idleCallback);
    glutKeyboardFunc(keyboardCallback);
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, m_width - 1, 0, m_height - 1);

    /* Initialize maze */
    m_map = new Map(m_mapWidth, m_mapHeight);
    m_gameObjects.push_back(m_map);

    /* Initialize player */
    m_player = new Player(m_map);    
    m_gameObjects.push_back(m_player);

    /* Initialize user controlled phantom */
    m_userCtrlPhantom = new Phantom(m_map, Color::red);
    m_userCtrlPhantom->toogleUserControl();
    m_gameObjects.push_back(m_userCtrlPhantom);

    /* Append extra phantoms */
    m_gameObjects.push_back(new Phantom(m_map));
    m_gameObjects.push_back(new Phantom(m_map, Color::pink));
    m_gameObjects.push_back(new Phantom(m_map, Color::yellowPacman));

    for (int i = 0; i < m_gameObjects.size(); i++)
        m_gameObjects[i]->init();

    g_gameManager = this;
}

void GameManager::run()
{
    glutMainLoop();
}

void GameManager::render()
{
    glClearColor(0.13, 0.13, 0.13, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

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
        for (int i = 0; i < m_gameObjects.size(); i++)
            m_gameObjects[i]->update(t - m_lastT);
        m_lastT = t;
    }

    glutPostRedisplay();
}

void GameManager::input(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 'w':
        m_userCtrlPhantom->setDirection(Vector2<>::up);
        break;
    
    case 's':
        m_userCtrlPhantom->setDirection(Vector2<>::down);
        break;
    
    case 'a':
        m_userCtrlPhantom->setDirection(Vector2<>::left);
        break;

    case 'd':
        m_userCtrlPhantom->setDirection(Vector2<>::right);
        break;
    case 27: //> Escape key
        this->destroy();
        glutDestroyWindow(m_windowId);
        exit(0);
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void GameManager::destroy()
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->destroy();
        delete m_gameObjects[i];
    }
    m_gameObjects.clear();
}

void GameManager::drawCallBack() { g_gameManager->render(); }
void GameManager::idleCallback() { g_gameManager->update(); }
void GameManager::keyboardCallback(unsigned char c, int x, int y) { g_gameManager->input(c, x, y); }