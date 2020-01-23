#pragma once

#include <vector>
#include "world-types.h"

class FeatureExtractor
{
private:

public:
    FeatureExtractor() {}
    ~FeatureExtractor() {}

    int getNumFeatures() const;
    std::vector<float> features(
        const state_t& state, Actions action) const;
};
