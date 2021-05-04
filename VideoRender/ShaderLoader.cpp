#include "ShaderLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include "windows.h"
#else
#include <dirent.h>
#endif

/*****************************************************************************************/
ShaderLoader::ShaderLoader()
{
    addShaderFolder("shaders\\"); //default path
}

/*****************************************************************************************/
ShaderLoader::ShaderLoader(std::string shaderFolder)
{
    addShaderFolder("shaders\\");
    addShaderFolder(shaderFolder);
    loadShaders();
}

/*****************************************************************************************/
ShaderLoader::~ShaderLoader()
{
    compiledShaders.clear();
    shaderPaths.clear();
}

/*****************************************************************************************/
void ShaderLoader::ReadDirrectory(const fs::path& srSearchPath, std::vector<std::string>* filelinks)
{
    if(!fs::exists(srSearchPath))
    {
        std::cout << std::string("ERROR: ") + std::string("No directory found: ") + srSearchPath.string() << std::endl;
        return;
    }

    for (const auto& entry : fs::directory_iterator(srSearchPath)) 
    {
        if (entry.is_directory()) 
            ReadDirrectory(entry, filelinks);
        else if (entry.is_regular_file())
        {
            std::string temp = entry.path().filename().string();
            if (temp.find(".glsl") != std::string::npos || temp.find(".vs") != std::string::npos || temp.find(".fs") != std::string::npos)
            {
                filelinks->emplace_back(temp);
                std::cout << "Founded file " + filelinks->back() << std::endl;
            }
        }
        else
            std::cout << std::string("INFO: ") + std::string("Unknown file: ") + entry.path().string() << std::endl;
    }
}

/*****************************************************************************************/
std::vector<std::string> ShaderLoader::getFilenames()
{
    std::vector<std::string> foundedFiles;

    for (std::string cpath : shaderPaths)
    {
        std::cout << "Looking for shaders in dirrectory " + cpath << std::endl;
        ReadDirrectory(cpath, &foundedFiles);
        if (foundedFiles.size() > 0)
        {
            std::cout << "Founded " + std::to_string(foundedFiles.size()) + " shaders in dirrectory " + cpath << std::endl;
            pathPrefix = cpath;
            break;
        }
        else
        {
            std::cout << "Can't found shaders in dirrectory " + cpath << std::endl;
            continue;
        }
    }
    return foundedFiles;
}

/*****************************************************************************************/
void ShaderLoader::loadShaders()
{
    std::string readedShaderCode;
    std::ifstream shaderFile;

    std::vector<std::string> foundedFiles = getFilenames();

    int success;
    char infoLog[512];

    GLuint programm_ID = glCreateProgram();

    for (std::string cfpath : foundedFiles)
    {
        GLuint temporaryShaderId;
        bool okFlag = false;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            shaderFile.open(pathPrefix + cfpath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            readedShaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char *shaderCode = readedShaderCode.c_str();

        if (cfpath.find("vertex") != std::string::npos || cfpath.find(".vs") != std::string::npos)
        {
            temporaryShaderId = glCreateShader(GL_VERTEX_SHADER);
            std::cout << "Compiling vertex shader..." << std::endl;
            okFlag = true;
        }
        else if (cfpath.find("fragment") != std::string::npos || cfpath.find(".fs") != std::string::npos)
        {
            temporaryShaderId = glCreateShader(GL_FRAGMENT_SHADER);
            std::cout << "Compiling fragment shader..." << std::endl;
            okFlag = true;
        }
        else
        {
            std::cout << "Error. Unknown shader " + pathPrefix + cfpath << std::endl;
        }

        if (okFlag)
        {
            glShaderSource(temporaryShaderId, 1, &shaderCode, NULL);
            glCompileShader(temporaryShaderId);
            glGetShaderiv(temporaryShaderId, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(temporaryShaderId, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" + std::string(infoLog) << std::endl;
            }
            else
            {
                glAttachShader(programm_ID, temporaryShaderId);
                compiledShaders.push_back(temporaryShaderId);
            }
        }
    }

    std::cout << "Shaders compiled: " + std::to_string(compiledShaders.size()) << std::endl;
    glLinkProgram(programm_ID);

    glGetProgramiv(programm_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programm_ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" + std::string(infoLog) << std::endl;
    }

    loaded_shaders.emplace("default", GShader(programm_ID));

    for (uint32_t i = 0; i < compiledShaders.size(); i++)
        glDeleteShader(compiledShaders[i]);
}
