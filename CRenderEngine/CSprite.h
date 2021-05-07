#ifndef CSPRITE
#define CSPRITE

#include "GShader.h"
#include "CMaterial.h"

typedef struct CSprite
{
    GLuint VBO, VAO, EBO;
    GShader* shader;
    CMaterial* material;

} CSprite;

CSprite* sprite_alloc(void);



#endif