#include "ShaderLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

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
void ShaderLoader::ReadDirrectory(const fs::path& srSearchPath, const std::vector<std::string>& filelinks)
{
    for (const auto& entry : fs::directory_iterator(srSearchPath)) 
    {
        if (entry.is_directory()) 
            ReadDirrectory(entry, filelinks);
        else if (entry.is_regular_file())
        {
            std::string temp = entry.filename().string();
            if (temp.find(".glsl") != std::string::npos || temp.find(".vs") != std::string::npos || temp.find(".fs") != std::string::npos)
            {
                filelinks.emplace_back(temp);
                glLogTracer->SendLog("Founded file " + filelinks.back());
            }
        }
        else
            glLogTracer->SendLog("INFO: " + "Unknown file" + " [?]" + entry.path());
    }
}

/*****************************************************************************************/
std::vector<std::string> ShaderLoader::getFilenames()
{
    std::vector<std::string> foundedFiles;

    for (std::string cpath : shaderPaths)
    {
        ReadDirrectory(cpath, foundedFiles);
        glLogTracer->SendLog("Looking for shaders in dirrectory " + cpath);
        if (foundedFiles.size() > 0)
        {
            glLogTracer->SendLog("Founded " + std::to_string(foundedFiles.size()) + " shaders in dirrectory " + cpath);
            pathPrefix = cpath;
            break;
        }
        else
        {
            glLogTracer->SendLog("Can't found shaders in dirrectory " + cpath);
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

    programm_ID = glCreateProgram();

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
            glLogTracer->SendLog("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        }

        const char *shaderCode = readedShaderCode.c_str();

        if (cfpath.find("vertex") != std::string::npos || cfpath.find(".vs") != std::string::npos)
        {
            temporaryShaderId = glCreateShader(GL_VERTEX_SHADER);
            glLogTracer->SendLog("Compiling vertex shader...");
            okFlag = true;
        }
        else if (cfpath.find("fragment") != std::string::npos || cfpath.find(".fs") != std::string::npos)
        {
            temporaryShaderId = glCreateShader(GL_FRAGMENT_SHADER);
            glLogTracer->SendLog("Compiling fragment shader...");
            okFlag = true;
        }
        else
        {
            glLogTracer->SendLog("Error. Unknown shader " + pathPrefix + cfpath);
        }

        if (okFlag)
        {
            glShaderSource(temporaryShaderId, 1, &shaderCode, NULL);
            glCompileShader(temporaryShaderId);
            glGetShaderiv(temporaryShaderId, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(temporaryShaderId, 512, NULL, infoLog);
                glLogTracer->SendLog("ERROR::SHADER::VERTEX::COMPILATION_FAILED" + std::string(infoLog));
            }
            else
            {
                glAttachShader(programm_ID, temporaryShaderId);
                compiledShaders.push_back(temporaryShaderId);
            }
        }
    }

    glLogTracer->SendLog("Shaders compiled: " + std::to_string(compiledShaders.size()));
    glLinkProgram(programm_ID);

    glGetProgramiv(programm_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programm_ID, 512, NULL, infoLog);
        glLogTracer->SendLog("ERROR::SHADER::PROGRAM::LINKING_FAILED" + std::string(infoLog));
    }

    for (uint32_t i = 0; i < compiledShaders.size(); i++)
        glDeleteShader(compiledShaders[i]);
}