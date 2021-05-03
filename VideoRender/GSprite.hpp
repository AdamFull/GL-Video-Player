#pragma once
#include "GRenderObject.hpp"
#include "GTexture2D.h"
#include <glad/glad.h>

class GSprite : GRenderObject
{
public:
    GSprite(GTexture2D &texture, glm::vec2 position, glm::vec2 size);
    ~GSprite();

    void render(GLuint program) override;
private:
    GLuint VBO, VAO;
};