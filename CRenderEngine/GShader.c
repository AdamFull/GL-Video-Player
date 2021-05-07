#include "GShader.h"
#include "stdlib.h"
#include "string.h"

GShader* shader_alloc()
{
    GShader* shader = NULL;
    shader = (GShader*)malloc(sizeof(GShader));

    shader->program_id = glCreateProgram();
    shader->shaders_loaded = 0;

    return shader;
}

bool shader_load(GShader** shader, const char* dpath)
{
    char symbol;
    GShader* gshader = *shader;

    gshader->shader_file = fopen(dpath, "rb");

    fseek(gshader->shader_file, 0L, SEEK_END);
    gshader->shader_data = (const char*)malloc(sizeof(const char)*ftell(gshader->shader_file));
    fseek(gshader->shader_file, 0L, SEEK_SET);

    if(gshader->shader_file != NULL)
    {
        while((symbol = getc(gshader->shader_file)) != EOF)
        {
            strcat(gshader->shader_data, &symbol);
        }
        fclose(gshader->shader_file);
    }
}

bool shader_compile(GShader** shader, int32_t shader_type)
{
    int result;
    char infoLog[512];
    GShader* gshader = *shader;

    gshader->shaders[gshader->shaders_loaded] = glCreateShader(shader_type);
    GLuint shader_id = gshader->shaders[gshader->shaders_loaded];

    glShaderSource(shader_id, 1, &gshader->shader_data, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED, %s\n", infoLog);
        return false;
    }
    else
        glAttachShader(gshader->program_id, shader_id);
    
    gshader->shaders_loaded++;
    free(gshader->shader_data);
    return true;
}

void shader_load_and_compile(GShader** shader, const char* dpath, int32_t shader_type)
{
    shader_load(shader, dpath);
    shader_link_program(shader, shader_type);
}

bool shader_link_program(GShader** shader)
{
    int result;
    char infoLog[512];
    GShader* gshader = *shader;

    glLinkProgram(gshader->program_id);
    glGetProgramiv(gshader->program_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(gshader->program_id, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
        return false;
    }

    for(int32_t i = 0; i < gshader->shaders_loaded; i++)
        glDeleteShader(gshader->shaders[i]);
    
    return true;
}

void shader_use(GShader** shader)
{
    glUseProgram((*shader)->program_id);
}

void shader_delete(GShader** shader)
{
    free(*shader);
}

void shader_set_bool(GShader** shader, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation((*shader)->program_id, name), (int)value);
}

void shader_set_int(GShader** shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation((*shader)->program_id, name), value);
}

void shader_set_float(GShader** shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation((*shader)->program_id, name), value);
}

void shader_set_vector2f(GShader** shader, const char* name, float x, float y)
{
    glUniform2f(glGetUniformLocation((*shader)->program_id, name), x, y);
}

void shader_set_vector3f(GShader** shader, const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation((*shader)->program_id, name), x, y, z);
}

void shader_set_vector4f(GShader** shader, const char* name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation((*shader)->program_id, name), x, y, z, w);
}

void shader_set_matrix4(GShader** shader, const char* name, const float* matrix)
{
    glUniformMatrix4fv(glGetUniformLocation((*shader)->program_id, name), 1, false, matrix);
}
