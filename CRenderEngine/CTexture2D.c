#include "CTexture2D.h"

CTexture2D* texture2d_alloc()
{
    CTexture2D* texture2d = NULL;
    texture2d = (CTexture2D*)malloc(sizeof(CTexture2D));

    glGenTextures(GL_TEXTURE_2D, &texture2d->ID);

    return texture2d;
}

bool texture2d_load(CTexture2D** texture2d, GLuint width, GLuint height, uint8_t** data)
{
    (*texture2d)->twidth = width;
    (*texture2d)->theight = height;

    glBindTexture(GL_TEXTURE_2D, (*texture2d)->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool texture2d_bind(CTexture2D** texture2d)
{
    glBindTexture(GL_TEXTURE_2D, (*texture2d)->ID);
}

void texture2d_delete(CTexture2D** texture2d)
{
    free(*texture2d);
}