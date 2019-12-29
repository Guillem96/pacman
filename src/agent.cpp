#include "agent.h"

#include "util.h"

#include "environment.h"
#include "feature-extractor.h"

Agent::Agent(float alpha, float epsilon, float gamma, 
             const FeatureExtractor* fe): 
                m_alpha(alpha), 
                m_epsilon(epsilon),
                m_gamma(gamma),
                m_featureExtractor(fe) {}

Agent::~Agent() 
{
}

void Agent::init()
{
    int nFeatures = m_featureExtractor->getNumFeatures();
    m_weights = new float[nFeatures];
    for (int i = 0; i < nFeatures; i++)
        m_weights[i] = 0;

    m_env = new Environment();
}

void Agent::destroy()
{
    delete m_env;
    delete[] m_weights;
}

void Agent::update(const state_t& state, 
                   Actions action, 
                   const state_t& nextState, 
                   float reward)
{
    float futureUtility = reward + m_gamma * m_getValue(nextState);
    float currentUtility = m_qValue(state, action);

    float difference = futureUtility - currentUtility;
    std::vector<float> features = m_featureExtractor->features(state, action);

    for (int f = 0; f < features.size(); f++)
        m_weights[f] = m_weights[f] + m_alpha * difference * features[f];
}

Actions Agent::takeAction(const state_t& state) const
{
    float random = ((float) rand()) / (float) RAND_MAX;
    auto actions = m_env->getPossibleActions(state);
    if (random < m_epsilon)
        return randomChoice(actions);
    
    Actions best;
    float maxQValue;

    for (auto a: actions)
    {
        float val = m_qValue(state, a);
        if (val > maxQValue)
        {
            best = a;
            maxQValue = val;
        }
    }
    return best;
}

float Agent::m_getValue(const state_t& state) const
{
    auto actions = m_env->getPossibleActions(state);
    if (actions.empty())
        return 0;
    
    float maxValue = -9999;

    for (Actions a: actions)
    {
        float qVal = m_qValue(state, a);
        if (qVal > maxValue)
            maxValue = qVal;
    }
    return maxValue;
}

float Agent::m_qValue(const state_t& state, Actions action) const
{
    auto features = m_featureExtractor->features(state, action);
    float qValue = 0.0;
    for (int f = 0; f < features.size(); f++)
        qValue += m_weights[f] * features[f];
    return qValue;
}

void Agent::stopLearning()
{
    m_epsilon = 0;
    m_alpha = 0;
}
