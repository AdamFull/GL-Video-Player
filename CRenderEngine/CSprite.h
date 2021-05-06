#ifndef CSPRITE
#define CSPRITE

#include "GShader.h"

typedef struct CSprite
{

    GLuint VBO, VAO, EBO;
    GShader* shader;

} CSprite;

CSprite* sprite_alloc(void);



#endif