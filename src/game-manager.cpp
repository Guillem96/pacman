#include "game-manager.h"

#include <GL/glut.h>

GameManager* g_gameManager;

GameManager::GameManager(int windowHeight, 
                        int windowWidth,
                        int mapHeight,
                        int mapWidth): m_height(windowHeight),
                                       m_width(windowWidth),
                                       m_mapWidth(mapWidth),
                                       m_mapHeight(mapHeight) {}


void GameManager::init()
{
    /* Init opengl stuff */
    int argc = 0;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(m_width, m_height);
    glutCreateWindow("Pacman");

    glutDisplayFunc(drawCallBack);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,m_width - 1, 0, m_height - 1);

    /* Initialize maze */
    m_map = new Map(m_mapWidth, m_mapHeight);
    m_map->init();

    /* Run */
    g_gameManager = this;
    glutMainLoop();
}


void GameManager::drawCallBack()
{
    g_gameManager->render();
}

void GameManager::render() 
{
    glClearColor(0.0, 0.0, 0.0 ,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_map->render();
    m_map->textRender();
    
    glutSwapBuffers();
}

void GameManager::destroy() 
{
    m_map->destroy();
    delete m_map;
}