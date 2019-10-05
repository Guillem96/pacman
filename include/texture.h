#pragma once

#include <GL/glut.h>

typedef unsigned char u8;

class Texture
{
private:
    GLuint m_id;
    u8* m_buffer;
    int m_w;
    int m_h;

public:
    Texture(GLuint id, u8* buffer, int width, int height);
    ~Texture();

    void active() const;
    
    void destroy();
};
