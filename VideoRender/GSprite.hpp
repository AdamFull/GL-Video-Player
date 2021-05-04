#pragma once
#include "GRenderObject.hpp"
#include "GTexture2D.hpp"
#include <glm/glm.hpp>

class GSprite : public GRenderObject
{
public:
    GSprite();
    GSprite(GTexture2D &texture, glm::vec2 position, glm::vec2 scale, float rotate);
    ~GSprite();

    void init();
    void update(GTexture2D &texture, glm::vec2 position, glm::vec2 scale, float rotate);
    void render(GShader shader) override;
private:
    GTexture2D texture;
    GLuint VBO, VAO;
    glm::vec2 position, scale;
    float rotate;
};