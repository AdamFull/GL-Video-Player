#include "GShader.hpp"
#include <glm/gtc/type_ptr.hpp>

GShader::GShader()
{

}

GShader::GShader(GLuint program_ID)
{
    this->ID = program_ID;
}

GShader::~GShader()
{
}

void GShader::execute()
{
    glUseProgram(this->ID);
}

void GShader::setBool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void GShader::setInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void GShader::setFloat(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void GShader::SetVector2f(const std::string &name, float x, float y)
{
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}

void GShader::SetVector2f(const std::string &name, const glm::vec2 &value)
{
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void GShader::SetVector3f(const std::string &name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void GShader::SetVector3f(const std::string &name, const glm::vec3 &value)
{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void GShader::SetVector4f(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void GShader::SetVector4f(const std::string &name, const glm::vec4 &value)
{
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void GShader::SetMatrix4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, glm::value_ptr(matrix));
}