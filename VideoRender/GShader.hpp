#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class GShader
{
public:
    GShader();
    GShader(GLuint program_ID);
    ~GShader();

    void execute();

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void SetVector2f(const std::string &name, float x, float y);
    void SetVector2f(const std::string &name, const glm::vec2 &value);
    void SetVector3f(const std::string &name, float x, float y, float z);
    void SetVector3f(const std::string &name, const glm::vec3 &value);
    void SetVector4f(const std::string &name, float x, float y, float z, float w);
    void SetVector4f(const std::string &name, const glm::vec4 &value);
    void SetMatrix4(const std::string &name, const glm::mat4 &matrix);
private:
    GLuint ID;
};