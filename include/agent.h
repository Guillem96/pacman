#pragma once

#include <vector>
#include "world-types.h"

class Environment;
class FeatureExtractor;

class Agent
{
private:
    float *m_weights;
    const FeatureExtractor* m_featureExtractor;
    const Environment* m_env;

    float m_alpha;
    float m_epsilon;
    float m_gamma;

    float m_qValue(const state_t& state, Actions action) const;
    float m_getValue(const state_t& state) const;
public:
    Agent(float alpha, float epsilon, float gamma, 
          const FeatureExtractor* fe);
    ~Agent();

    void init();
    void destroy();

    void stopLearning();
    
    void update(const state_t& state, 
                Actions action, 
                const state_t& nextState, 
                float reward);

    Actions takeAction(const state_t& state) const;
};
