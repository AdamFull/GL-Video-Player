#include "GTexture2D.hpp"

GTexture2D::GTexture2D()
{
    glGenTextures(GL_TEXTURE_2D, &ID);
}

GTexture2D::GTexture2D(GLuint width, GLuint height, uint8_t* data)
{
    glGenTextures(GL_TEXTURE_2D, &ID);
    this->load(width, height, data);
}

GTexture2D::~GTexture2D()
{
}

void GTexture2D::load(GLuint width, GLuint height, uint8_t* data)
{
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GTexture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}