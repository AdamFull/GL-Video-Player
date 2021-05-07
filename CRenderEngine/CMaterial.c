#include "CMaterial.h"

CMaterial* material_alloc()
{
    CMaterial* material = NULL;
    material = (CMaterial*)malloc(sizeof(CMaterial));
    material->mtextures = (CMaterialTextures*)malloc(sizeof(CMaterialTextures));

    material->mtextures->diffuse = NULL;
    material->mtextures->alpha = NULL;
    material->mtextures->normalmap = NULL;
    material->mtextures->specular = NULL;
    material->mtextures->ambient = NULL;
    material->mtextures->emissive = NULL;
    material->mtextures->metalic = NULL;
    material->mtextures->roughness = NULL;
    material->mtextures->displacement = NULL;

    return material;
}

void material_load_texture(CTexture2D** texture, GLuint width, GLuint height, uint8_t** data)
{
    CTexture2D* texture2d = *texture;
    if(!texture2d)
        texture2d = texture2d_alloc();
    
    texture2d_load(&texture2d, width, height, data);
}

void material_load_diffuse(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->diffuse, width, height, data);
}

void material_load_alpha(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->alpha, width, height, data);
}

void material_load_normalmap(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->normalmap, width, height, data);
}

void material_load_specularmap(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->specular, width, height, data);
}

void material_load_ao(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->ambient, width, height, data);
}

void material_load_emissive(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->emissive, width, height, data);
}

void material_load_metalic(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->metalic, width, height, data);
}

void material_load_roughness(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->roughness, width, height, data);
}

void material_load_displacement(CMaterial** material, GLuint width, GLuint height, uint8_t** data)
{
    CMaterial* cmaterial = *material;
    material_load_texture(&cmaterial->mtextures->displacement, width, height, data);
}

void material_bind(CMaterial** material)
{
    CMaterial* cmaterial = *material;

    for(int i = 0; i < MAT_TEXTURES_SIZE; i++)
    {
        CTexture2D* texture = cmaterial->atextures[i];
        if(texture)
            texture2d_bind(&texture);
    }
    
}

void material_delete(CMaterial** material)
{
    CMaterial* cmaterial = *material;
    free(cmaterial->mtextures);
    free(cmaterial);
}