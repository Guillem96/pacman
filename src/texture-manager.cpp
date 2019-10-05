#include "texture-manager.h"
#include "texture.h"

#include <GL/glut.h>
#include <jpeglib.h>

typedef unsigned char u8;

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

static void readJpeg(const char *filename,
                     u8 **image,
                     int *width, int *height)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    u8 **buffer;
    int i, j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((infile = fopen(filename, "rb")) == NULL)
    {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, true);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;

    *image = new u8[cinfo.output_width * cinfo.output_height * cinfo.output_components];

    buffer = new u8*;
    buffer[0] = new u8[cinfo.output_width * cinfo.output_components];

    i = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for (j = 0; j < cinfo.output_width * cinfo.output_components; j++)
        {
            (*image)[i] = buffer[0][j];
            i++;
        }
    }

    delete buffer;
    jpeg_finish_decompress(&cinfo);
}

void TextureManager::loadTexture(std::string fileName,
                                 std::string name,
                                 int dim)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    u8 *buffer;
    u8 *buffer2;
    int width, height;
    long i, j;
    long k, h;


    readJpeg(fileName.c_str(), &buffer, &width, &height);

    buffer2 = new u8[dim * dim * 3];

    //-- The texture pattern is subsampled so that its dimensions become dim x dim --
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
            k = i * height / dim;
            h = j * width / dim;

            buffer2[3 * (i * dim + j)] = buffer[3 * (k * width + h)];
            buffer2[3 * (i * dim + j) + 1] = buffer[3 * (k * width + h) + 1];
            buffer2[3 * (i * dim + j) + 2] = buffer[3 * (k * width + h) + 2];
        }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim, dim, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer2);
    
    m_textures[name] = new Texture(m_id, buffer2, dim, dim);
    delete buffer;
}

const Texture* TextureManager::operator[](std::string name)
{
    return m_textures[name];
}

void TextureManager::destroy()
{
    for (const auto& texture : m_textures) {
        auto t = texture.second;
        t->destroy();
        delete t;
    }
}
