#pragma once

#include <vector>
#include <iostream>
#include <stack>
#include "cell.h"
#include "util.h"

enum Action {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    NONE
};

class Successor {
    public:
        Vector2 state;
        Action action;

    Successor(Vector2 s, Action a): state(s), action(a) {}
} ;

class Node
{
    private:
        Node* m_parent;
        Vector2 m_state;
        Action m_action;

    public:
        Node(Vector2 state, Node* parent, Action action);

        Vector2 getState() const;
        const Node* getParent() const;
        const Action getAction() const;

};

class DFS
{
private:
    Cell** m_maze;
    std::stack<Node> m_fringe;

    int m_width;
    int m_height;

    void m_drawEdges(std::vector<std::pair<Vector2, Vector2>>);
    bool m_end();
    std::vector<Action> m_validDirections(const Vector2& pos);
    std::vector<Successor> m_getChildren(const Node& n);

public:
    DFS(int height, int width);

    Cell** generate();
    int getHeight();
    int getWidth();
    Cell** getMaze();

    void visit(const Node& n);
    Vector2 getStartState() const;
    void m_visitThroug(const Node& n);
};

