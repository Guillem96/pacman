#include "texture.h"

#include <GL/glut.h>
#include "util.h"

Texture::Texture(GLuint id, u8* buffer, int width, int height):
    m_id(id), m_buffer(buffer), m_w(width), m_h(height) {}

Texture::~Texture() {}

void Texture::active() const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glMaterialfv(GL_FRONT_AND_BACK, 
                 GL_AMBIENT_AND_DIFFUSE, 
                 Color::white.glColorfv());
}

    
void Texture::destroy()
{
    delete m_buffer;
}