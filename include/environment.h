#pragma once

#include <vector>

#include "world-types.h"

class Environment
{
private:
    
public:
    Environment() {}
    ~Environment() {}

    const std::vector<Actions> allActions() const
    {
        std::vector<Actions> actions;
        actions.push_back(Actions::Idle);
        actions.push_back(Actions::Left);
        actions.push_back(Actions::Right);
        actions.push_back(Actions::Up);
        actions.push_back(Actions::Down);
        return actions;
    }

    transition_t step(const state_t& state, Actions action) const;
    const std::vector<Actions> getPossibleActions(const state_t& state) const;
};
