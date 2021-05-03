#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TypeManager
{
public:
    TypeManager(/* args */);
    ~TypeManager();

    bool load(std::string font_name);
    Character get_character(GLchar character) { return Characters[character]; }

private:
    FT_Library ft;
    FT_Face face;

    std::map<GLchar, Character> Characters;
};
