#ifndef __SHADERLOADER__
#define __SHADERLOADER__

#include <glad/glad.h>

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class ShaderLoader{
public:
    ShaderLoader();
    ShaderLoader(std::string  shaderFolder);
    ~ShaderLoader();

    void addShaderFolder(std::string newPath) { shaderPaths.push_back(newPath); }

    std::vector<std::string> getFilenames();

    void loadShaders();
    void ReadDirrectory(const fs::path&, std::vector<std::string>*);
    GLuint getProgram() { return programm_ID; }

    void setBool(const std::string &name, bool value) const{         
        glUniform1i(glGetUniformLocation(programm_ID, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const{ 
        glUniform1i(glGetUniformLocation(programm_ID, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const{ 
        glUniform1f(glGetUniformLocation(programm_ID, name.c_str()), value); 
    } 
private:
    std::vector<GLuint> compiledShaders;
    std::vector<std::string> shaderPaths;
    std::string pathPrefix;
    GLuint programm_ID;
};


#endif