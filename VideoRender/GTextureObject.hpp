#pragma once

#include "GRenderObject.hpp"
#include <glad/glad.h>

class GTextureObject : public GRenderObject
{
public:
    GTextureObject(uint8_t* texture_buffer, int width, int height, GLuint texture_handle)
    {
        glBindTexture(GL_TEXTURE_2D, texture_handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);
    }

    ~GTextureObject() {}

    void render(GLuint shader, GLuint VAO, GLuint VBO = 0) override
    {
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};