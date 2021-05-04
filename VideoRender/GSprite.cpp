#include "GSprite.hpp"
#include <glm/gtc/matrix_transform.hpp>

GSprite::GSprite()
{
}

GSprite::GSprite(std::shared_ptr<GTexture2D> texture, glm::vec2 position, glm::vec2 scale, float rotate)
{
    init();
    update(texture, position, scale, rotate);
}

void GSprite::init()
{
    float vertices[] = 
    { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GSprite::update(std::shared_ptr<GTexture2D> texture, glm::vec2 position, glm::vec2 scale, float rotate)
{
    this->texture = texture;
    this->position = position;
    this->scale = scale;
    this->rotate = rotate;
}

GSprite::~GSprite()
{
    glDeleteVertexArrays(1, &VAO);
}

void GSprite::render(GShader shader)
{
    shader.execute();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(scale, 1.0f)); // last scale

    shader.SetMatrix4("model", model);

    // render textured quad
    glm::vec3 color(1.f);
    shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}