#pragma once

#include "GRenderObject.hpp"
#include "TypeManager.hpp"
#include <string>
#include <map>
#include <glad/glad.h>

class GTypeObject : public GRenderObject
{
public:
    GTypeObject(TypeManager* tmgr, std::string text, float x, float y, float scale)
    {
        this->tmgr = tmgr;
        this->text = text;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    ~GTypeObject() {}

    void render(GLuint shader, GLuint VAO, GLuint VBO = 0) override
    {
        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for(auto c = text.begin(); c != text.end(); c++)
        {
            Character ch = tmgr->get_character(*c);

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos, ypos, 0.0f, 1.0f},
                {xpos + w, ypos, 1.0f, 1.0f},

                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos, 1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}};
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    std::string text;
    float x, y, scale;
    TypeManager* tmgr;
};