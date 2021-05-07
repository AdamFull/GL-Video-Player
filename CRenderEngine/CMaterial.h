#ifndef CMATERIAL
#define CMATERIAL

#include "CTexture2D.h"

#define MAT_TEXTURES_SIZE 9

typedef struct CMaterialTextures
{

    CTexture2D* diffuse;
    CTexture2D* alpha;
    CTexture2D* normalmap;
    CTexture2D* specular;
    CTexture2D* ambient;
    CTexture2D* emissive;
    CTexture2D* metalic;
    CTexture2D* roughness;
    CTexture2D* displacement;

} CMaterialTextures;

typedef union CMaterial 
{
    CTexture2D* atextures[MAT_TEXTURES_SIZE];
    CMaterialTextures* mtextures;
} CMaterial;

CMaterial* material_alloc(void);

void material_load_texture(CTexture2D** texture, GLuint width, GLuint height, uint8_t** data);

void material_load_diffuse(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_alpha(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_normalmap(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_specularmap(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_ao(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_emissive(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_metalic(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_roughness(CMaterial** material, GLuint width, GLuint height, uint8_t** data);
void material_load_displacement(CMaterial** material, GLuint width, GLuint height, uint8_t** data);

void material_bind(CMaterial** material);

void material_delete(CMaterial** material);

#endif