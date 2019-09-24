#include "map-generator.h"
#include "util.h"
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <chrono>

typedef std::pair<Vector2, Vector2> edge_t;

Node::Node(Vector2 state, Node* parent, Action action): m_state(state), m_parent(parent), m_action(action)
{

}

std::vector<Action> DFS::m_validDirections(const Vector2& pos)
{
    bool canGoLeft = pos.getY() - 2 >= 0;
    bool canGoRight =  pos.getY() + 2 < m_width;
    bool canGoUp = pos.getX() - 2 >= 0;
    bool canGoDown = pos.getX() + 2 < m_height;

    std::vector<Action> valid;
    if (canGoLeft)
        valid.push_back(Action::LEFT);

    if (canGoRight)
        valid.push_back(Action::RIGHT);

    if (canGoUp)
        valid.push_back(Action::UP);

    if (canGoDown)
        valid.push_back(Action::DOWN);

    return valid;
}

static Vector2 getActionVector(Action a)
{
    if (a == Action::RIGHT)
        return Vector2(0, 2);
    else if (a == Action::LEFT)
        return  Vector2(0, -2);
    else if (a == Action::UP)
        return  Vector2(-2, 0);
    else if (a == Action::DOWN)
        return  Vector2(2, 0);
    return Vector2();
}

std::vector<Successor> DFS::m_getChildren(const Node& n)
{
    auto possibleActions = m_validDirections(n.getState());
    std::vector<Successor> s;
    for (int i = 0; i < possibleActions.size(); i++)
    {
        Vector2 direction = getActionVector(possibleActions[i]);
        s.push_back(Successor(direction + n.getState(), possibleActions[i]));
    }
    return s;
}


DFS::DFS(int height, int width): m_width(width), m_height(height)
{
    /* Initialize only the odd cells to wall */
    m_maze = new Cell*[m_width * m_height];
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            auto cellType = (i * j) % 2 == 1 ? CellType::Path : CellType::Wall;
            m_maze[m_width * i + j] = new Cell(cellType);
            m_maze[m_width * i + j]->init();
        }
    }
}

Vector2 DFS::getStartState() const
{
    int* col = range(1, m_width, 2);
    int* row = range(1, m_height, 2);
    shuffle(col, m_width / 2);
    shuffle(row, m_height / 2);

    return Vector2(row[0], col[0]);
}

static bool all(bool* a, int n)
{
    for (int i = 0; i < n; i++)
        if (!a[i])
            return false;
    return true;
}

Cell** DFS::generate()
{
    bool* visited = new bool[m_width * m_height];
    std::stack<Node> fringe;

    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++)
            visited[m_width * i + j] = m_maze[m_width * i + j]->getType() == CellType::Wall;

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    fringe.push(Node(getStartState(), nullptr, Action::NONE));

    while (true)
    {
        if (fringe.empty())
            break; /* Done */

        Node n = fringe.top(); fringe.pop();
        visited[m_width * n.getState().getX() + n.getState().getY()] = true;

        auto s = m_getChildren(n);
        shuffle(s.begin(), s.end(), std::default_random_engine(seed));

        for (int i = 0; i < s.size(); i++)
        {
            auto ns = Node(s[i].state, &n, s[i].action);
           if (!m_maze[m_width * s[i].state.getX() + s[i].state.getY()]->isWall() &&
                !visited[m_width * ns.getState().getX() + ns.getState().getY()])
            {
                /* Clear the wall that is between the two points */
                Vector2 dir = getActionVector(s[i].action);
                dir = Vector2(dir.getX() / 2, dir.getY() / 2);
                auto toClear = ns.getState() - dir;
                m_maze[m_width * toClear.getX() + toClear.getY()]->setType(CellType::Path);

                /* Mark the node as visited so we do not generate again their children */
                visited[m_width * ns.getState().getX() + ns.getState().getY()] = true;
                fringe.push(ns);
            }
        }
    }
    
    delete visited;
    return m_maze;
}

void DFS::visit(const Node& n)
{
    auto a = n.getAction();
    auto pos = n.getState();

    if (a == Action::NONE)
        return;

    auto dir = getActionVector(a);
    dir = Vector2(dir.getX() / - 2, dir.getY() / -2);
    pos = pos + dir;
    m_maze[m_width * pos.getX() + pos.getY()]->setType(CellType::Path);
}

void DFS::destroy()
{
    for (int i = 0; i < m_height * m_width; i++)
        delete m_maze[i];
    delete m_maze;
}

int DFS::getHeight() { return m_height; }
int DFS::getWidth() { return m_width; }
Cell** DFS::getMaze() { return m_maze; }

Vector2 Node::getState() const { return m_state; }
const Node* Node::getParent() const { return m_parent; }
const Action Node::getAction() const { return m_action; }
