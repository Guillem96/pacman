#include "map-generator.h"
#include "util.h"
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <chrono>


Node::Node(Vector2<> state, Node* parent, Vector2<> action): m_state(state), m_parent(parent), m_action(action)
{

}

std::vector<Vector2<>> DFS::m_validDirections(const Vector2<>& pos)
{
    bool canGoLeft = pos.getY() - 2 >= 0;
    bool canGoRight =  pos.getY() + 2 < m_width;
    bool canGoUp = pos.getX() - 2 >= 0;
    bool canGoDown = pos.getX() + 2 < m_height;

    std::vector<Vector2<>> valid;
    if (canGoLeft)
        valid.push_back(Vector2<>::left * 2);

    if (canGoRight)
        valid.push_back(Vector2<>::right * 2);

    if (canGoUp)
        valid.push_back(Vector2<>::up * 2);

    if (canGoDown)
        valid.push_back(Vector2<>::down * 2);

    return valid;
}

std::vector<Successor> DFS::m_getChildren(const Node& n)
{
    auto possibleActions = m_validDirections(n.getState());
    std::vector<Successor> s;
    for (int i = 0; i < possibleActions.size(); i++)
    {
        Vector2<> direction = possibleActions[i];
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

Vector2<> DFS::getStartState() const
{
    int* col = range(1, m_width, 2);
    int* row = range(1, m_height, 2);
    shuffle(col, m_width / 2);
    shuffle(row, m_height / 2);

    return Vector2<>(row[0], col[0]);
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
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();

    for (int i = 0; i < m_height * m_width; i++)
        visited[i] = m_maze[i]->isWall();

    fringe.push(Node(getStartState(), nullptr, Vector2<>()));

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
                /* Visit the new node */
                visit(ns);
               
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

    if (a == Vector2<>())
        return;
    
    /* Clear the wall that is between the two points */
    Vector2<> dir = a / 2;
    auto toClear = n.getState() - dir;
    m_maze[m_width * toClear.getX() + toClear.getY()]->setType(CellType::Path);
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

Vector2<> Node::getState() const { return m_state; }
const Node* Node::getParent() const { return m_parent; }
const Vector2<> Node::getAction() const { return m_action; }
