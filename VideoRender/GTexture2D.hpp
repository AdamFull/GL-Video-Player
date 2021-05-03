#pragma once

#include <glad/glad.h>

class GTexture2D
{
public:
    GTexture2D();
    ~GTexture2D();
private:
    GLuint ID;

    GLuint width, height;
};