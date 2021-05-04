#ifndef __SHADERLOADER__
#define __SHADERLOADER__

#include "GShader.hpp"

#include <map>
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

    GShader& getShader(std::string shader_name) { return loaded_shaders.at(shader_name); }

private:
    std::vector<GLuint> compiledShaders;
    std::vector<std::string> shaderPaths;
    std::string pathPrefix;
    std::map<std::string, GShader> loaded_shaders;
};


#endif