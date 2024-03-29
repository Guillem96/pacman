#pragma once

#include <vector>
#include <iostream>
#include <stack>
#include "cell.h"
#include "util.h"


/* Class which represents the possible children of the graph node */
class Successor 
{
    public:
        Vector2<> state;
        Vector2<> action;

    Successor(Vector2<> s, Vector2<> a): state(s), action(a) {}
};

/* 
 * Represents a graph node.  
 * Note: While we generate the random map, the maze is the graph and map cells are the graph nodes 
 */
class Node
{
    private:
        Node* m_parent;
        Vector2<> m_state;
        Vector2<> m_action;

    public:
        Node(Vector2<> state, Node* parent, Vector2<> action);

        Vector2<> getState() const;
        const Node* getParent() const;
        const Vector2<> getAction() const;
};

/*
 * Implementation of depth first search in graph 
 */
class DFS
{
private:
    Cell** m_maze;

    int m_width;    //> Maze or graph width
    int m_height;   //> Maze or graph height
    
    /* Get the possible directions while being in the `pos` cell */
    std::vector<Vector2<>> m_validDirections(const Vector2<>& pos);
    /* Returns all the childrens of a specific node */
    std::vector<Successor> m_getChildren(const Node& n);

public:
    DFS(int height, int width);

    Cell** generate();          //> Generate the random maze
    void visit(const Node& n);  //> After DFS falls in a new node

    
    Vector2<> getStartState() const; //> Get DFS initial state
    
    int getHeight();
    int getWidth();
    Cell** getMaze();

    void destroy();
};

