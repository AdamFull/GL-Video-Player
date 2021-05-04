#pragma once

#include <glad/glad.h>
#include <stdint.h>

class GTexture2D
{
public:
    GTexture2D();
    GTexture2D(GLuint width, GLuint height, uint8_t* data);
    ~GTexture2D();

    void load(GLuint width, GLuint height, uint8_t* data);
    void bind();
private:
    GLuint ID;

    GLuint width, height;
};