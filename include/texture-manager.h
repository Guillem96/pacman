#pragma once

#include <map>
#include <GL/glut.h>

class Texture;

class TextureManager
{
private:
    std::map<std::string, Texture*> m_textures;
    GLuint m_id = 0;

public:
    TextureManager();
    ~TextureManager();

    void loadTexture(std::string fileName, 
                     std::string name,
                     int w);

    const Texture* operator[](std::string name);

    void destroy();
};
