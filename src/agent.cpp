#include "agent.h"

#include "util.h"

#include "feature-extractor.h"

static const std::vector<Actions> getPossibleActions(
    const state_t& state);

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
}

void Agent::destroy()
{
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
    
    m_epsilon -= 1e-5;
    // std::cout << "Epsilon: " << m_epsilon << std::endl;
}

Actions Agent::takeAction(const state_t& state) const
{
    float random = ((float) rand()) / (float) RAND_MAX;
    auto actions = getPossibleActions(state);
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
    auto actions = getPossibleActions(state);
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


static const std::vector<Actions> getPossibleActions(
    const state_t& state)
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