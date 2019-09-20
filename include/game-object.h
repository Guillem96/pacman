#pragma once

class GameObject
{
public:
    GameObject() {}
    virtual ~GameObject() {}

    virtual void init() = 0;
    virtual void render() const = 0;
    virtual void destroy() = 0;
};
