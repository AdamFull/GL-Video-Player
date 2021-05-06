#ifndef CTEXTURE2D
#define CTEXTURE2D

#include <glad/glad.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct CTexture2D
{

    GLuint ID;

    GLuint twidth, theight;

} CTexture2D;

CTexture2D* texture2d_alloc(void);

bool texture2d_load(CTexture2D** texture2d, GLuint width, GLuint height, uint8_t** data);

bool texture2d_bind(CTexture2D** texture2d);

void texture2d_delete(CTexture2D** texture2d);

#endif