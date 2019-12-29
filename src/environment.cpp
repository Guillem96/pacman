#include "environment.h"

const std::vector<Actions> Environment::getPossibleActions(
    const state_t& state) const
{
    std::vector<Actions> validActions;
    auto playerPos = state.player.getPosition();
    auto map = state.map;

    auto upLocation = playerPos + Vector2<>::up;
    auto downLocation = playerPos + Vector2<>::down;
    auto rightLocation = playerPos + Vector2<>::right;
    auto leftLocation = playerPos + Vector2<>::left;

    if (!map(upLocation)->isWall())
        validActions.push_back(Actions::Up);

    if (!map(downLocation)->isWall())
        validActions.push_back(Actions::Down);

    if (!map(rightLocation)->isWall())
        validActions.push_back(Actions::Right);

    if (!map(leftLocation)->isWall())
        validActions.push_back(Actions::Left);
    
    validActions.push_back(Actions::Idle);

    return validActions;
}

transition_t Environment::step(const state_t& state, Actions action) const
{
    transition_t transition = { state, 5, false };
    return transition;
}

