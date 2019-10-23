#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "game-manager.h"
#include <jpeglib.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH  1000

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

    auto gm = GameManager(WINDOW_HEIGHT, WINDOW_WIDTH, 
                          MAP_HEIGHT, MAP_WIDTH);

    gm.init();
    gm.run();
    
    return 0;
}
