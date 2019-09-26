#pragma once

/* Abstract class representing all the pacman game instantiable objects */
class GameObject
{
public:
    GameObject() {}
    virtual ~GameObject() {}

    virtual void init() = 0;            //> Initialzie all variables
    virtual void update();              //> Called every game frame
    virtual void render() const = 0;    //> Render object contents using OpenGL
    virtual void destroy() = 0;         //> Dealocate variables from memory
};
