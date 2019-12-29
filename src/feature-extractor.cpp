#include "feature-extractor.h"

#include <stack>
#include <set>

#include "util.h"
#include "player.h"
#include "phantom.h"

typedef struct 
{
    Vector2<> pos;
    int dist;
} node_t;

int FeatureExtractor::getNumFeatures() const
{
    return 4;
}

static bool isNear(const Vector2<>& pos1, const Vector2<>& pos2)
{
    if (pos1 + Vector2<>::up == pos2)
        return true;
    
    if (pos1 + Vector2<>::down == pos2)
        return true;
    
    if (pos1 + Vector2<>::left == pos2)
        return true;
    
    if (pos1 + Vector2<>::right == pos2)
        return true;
    
    return false;
}

static Vector2<> move(const Vector2<>& origin, Actions a)
{
    if (a == Actions::Up)
        return origin + Vector2<>::up;
    
    if (a == Actions::Down)
        return origin + Vector2<>::down;
    
    if (a == Actions::Right)
        return origin + Vector2<>::right;
    
    if (a == Actions::Left)
        return origin + Vector2<>::left;
    
    return origin;
}

static std::vector<Vector2<>> getLegalNeighbors(
    const Map& map, const Vector2<> playerPos) 
{
    auto upLocation = playerPos + Vector2<>::up;
    auto downLocation = playerPos + Vector2<>::down;
    auto rightLocation = playerPos + Vector2<>::right;
    auto leftLocation = playerPos + Vector2<>::left;
    std::vector<Vector2<>> validNeighbors;

    if (!map(upLocation)->isWall())
        validNeighbors.push_back(upLocation);

    if (!map(downLocation)->isWall())
        validNeighbors.push_back(downLocation);

    if (!map(rightLocation)->isWall())
        validNeighbors.push_back(rightLocation);

    if (!map(leftLocation)->isWall())
        validNeighbors.push_back(leftLocation);
    
    return validNeighbors;
}

static int closestFood(const Map& map, const Vector2<> playerPos)
{
    std::stack<node_t> fringe;
    fringe.push({ playerPos, 0 });
    std::set<Vector2<>> expanded;
    
    while (!fringe.empty())
    {
        auto state = fringe.top(); fringe.pop();
        if (expanded.find(state.pos) != expanded.end())
            continue;
        expanded.insert(state.pos);
        if (map(state.pos)->hasFood())
            return state.dist;

        auto neighbors = getLegalNeighbors(map, state.pos);
        for (auto n: neighbors)
            fringe.push({n, state.dist + 1});
    }
    return -1;
}

std::vector<float> FeatureExtractor::features(
    const state_t& state, Actions action) const
{
    std::vector<float> features;

    auto phantoms = state.phantoms;
    auto player = state.player;
    auto map = state.map;

    auto nextPlayerPos = move(player.getPosition(), action);

    // #-of-ghosts-1-step-away
    int nearPhantoms = 0;
    for (auto phantom: phantoms)
        nearPhantoms += (int) isNear(player.getPosition(), 
                                     phantom.getPosition());
    features.push_back(nearPhantoms);

    // Has eaten food
    features.push_back((float)map(nextPlayerPos)->hasFood());

    // Closest food
    auto closestFoodDist = (float) closestFood(map, nextPlayerPos);
    features.push_back(
        closestFoodDist / (map.getHeight() * map.getWidth()));

    // Bias
    features.push_back(1.);

    for (int f = 0; f < features.size(); f++)
        features[f] = features[f] / (float)10;
    
    return features;
}