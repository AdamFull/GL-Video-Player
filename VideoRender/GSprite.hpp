#pragma once
#include <memory>
#include "GRenderObject.hpp"
#include "GTexture2D.hpp"
#include <glm/glm.hpp>

class GSprite : public GRenderObject
{
public:
    GSprite();
    GSprite(std::shared_ptr<GTexture2D> texture, glm::vec2 position, glm::vec2 scale, float rotate);
    ~GSprite();

    void init();
    void update(std::shared_ptr<GTexture2D>, glm::vec2 position, glm::vec2 scale, float rotate);
    void render(GShader shader) override;
protected:
    std::shared_ptr<GTexture2D> texture;
    GLuint VBO, VAO, EBO;
    glm::vec2 position, scale;
    float rotate;
};