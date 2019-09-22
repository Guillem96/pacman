#include <iostream>
#include "map.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    Map* map = new Map(5, 10);
    map->init();
    map->render();
    map->destroy();
    delete map;
    return 0;
}
