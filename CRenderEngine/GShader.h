#ifndef GSHADER
#define GSHADER

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stdbool.h>
#include <stdint.h>

typedef struct GShader
{
    FILE* shader_file;
    const char* shader_data;

    uint8_t program_name[64];
    GLuint shaders_loaded;
    GLuint shaders[64];
    GLuint program_id;

} GShader;

GShader* shader_alloc(void);

bool shader_load(GShader** shader, const char* dpath);

bool shader_compile(GShader** shader, int32_t shader_type);

bool shader_link_program(GShader** shader);

bool shader_load_and_compile(GShader** shader, const char* dpath, int32_t shader_type);

void shader_use(GShader** shader);

void shader_delete(GShader** shader);

void shader_set_bool(GShader** shader, const char* name, bool value);
void shader_set_int(GShader** shader, const char* name, int value);
void shader_set_float(GShader** shader, const char* name, float value);
void shader_set_vector2f(GShader** shader, const char* name, float x, float y);
void shader_set_vector2f(GShader** shader, const char* name, const glm::vec2 &value);
void shader_set_vector3f(GShader** shader, const char* name, float x, float y, float z);
void shader_set_vector3f(GShader** shader, const char* name, const glm::vec3 &value);
void shader_set_vector4f(GShader** shader, const char* name, float x, float y, float z, float w);
void shader_set_vector4f(GShader** shader, const char* name, const glm::vec4 &value);
void shader_set_matrix4(GShader** shader, const char* name, const glm::mat4 &matrix);

#endif