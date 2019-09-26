#include <iostream>
#include "map.h"
#include <stdlib.h>
#include <time.h>
#include "game-manager.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  800

#define MAP_HEIGHT 19
#define MAP_WIDTH  31

int main(int argc, char const *argv[])
{
    if (MAP_HEIGHT < 10 || MAP_WIDTH < 10)
    {
        std::cout << "Window width and height must be larger than 10." << std::endl;
        return -1;
    }

    srand(time(NULL));

    GameManager* gm = new GameManager(WINDOW_HEIGHT, 
                                      WINDOW_WIDTH, 
                                      MAP_HEIGHT, MAP_WIDTH);

    gm->init();
    gm->run();
    gm->destroy();

    delete gm;
    
    return 0;
}
