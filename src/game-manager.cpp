#include "game-manager.h"

#include "map.h"
#include "player.h"
#include "phantom.h"
#include "game-object.h"
#include "observer.h"

#include <GL/glut.h>
#include <math.h>

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(m_width, m_height);
    m_windowId = glutCreateWindow("Pacman");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(drawCallBack);
    glutIdleFunc(idleCallback);
    glutKeyboardFunc(keyboardCallback);

    m_observer = new Observer(90, 30, 450);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    /* Translate all objects */
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-m_width * 0.5, -m_height * 0.5, 0);

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
        m_observer->update(t - m_lastT);
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
}

void GameManager::drawCallBack() { g_gameManager->render(); }
void GameManager::idleCallback() { g_gameManager->update(); }
void GameManager::keyboardCallback(unsigned char c, int x, int y) { g_gameManager->input(c, x, y); }