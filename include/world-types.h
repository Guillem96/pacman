#pragma once

#include <vector>
#include "map.h"
#include "player.h"
#include "phantom.h"

typedef struct 
{
    Map map;
    Player player;
    std::vector<Phantom> phantoms;
} state_t;

typedef struct 
{
    const state_t nextState;
    const float reward;
    const bool isTerminal;
} transition_t;

enum Actions {
    Right,
    Left,
    Up,
    Down,
    Idle,
};
