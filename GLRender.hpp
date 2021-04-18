#ifndef __SE_MAIN__
#define __SE_MAIN__

#include "stdint.h"

#define GLEW_STATIC

#include "ShaderLoader.hpp"
#include "VideoFile.hpp"
#include "ALPlayer.hpp"
#include <GLFW/glfw3.h>

#include <chrono>

typedef void(*GLKeyboardCallback)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*GLMouseCallback)(GLFWwindow* window, double xpos, double ypos);
typedef void(*GLMouseButtonCallback)(GLFWwindow* window, int button, int action, int mods);

typedef struct{
    GLKeyboardCallback glKbCb;
    GLMouseCallback glMCb;
    GLMouseButtonCallback glMbCb;
}GLInputCallbacks;

class GLRender{
public:
    GLRender();
    GLRender(VideoFile* vf);
    GLRender(int sWidth, int sHeight);
    GLRender(int sWidth, int sHeight, GLKeyboardCallback GL_Keyboard_Callback,
            GLMouseCallback GL_Mouse_Callback, GLMouseButtonCallback GL_Mouse_Button_Callback);
    GLRender(int sWidth, int sHeight, GLInputCallbacks *glInputCallbacks);
    ~GLRender();

    void screen_size(int width, int height);
    void add_shader_folder_path(std::string newPath) { shaderLoader.addShaderFolder(newPath); }

    bool initialize();
    bool initialize(std::string shaderFolderPath);

    void display();
    void clear();

    bool is_running();

    double get_frame_time() { return frameTime; }

/**********************************GL private variables**************************************/
private:
    GLFWwindow* window;
    GLuint VBO, VAO, EBO;
    GLuint tex_handle;
    ShaderLoader shaderLoader;

/**********************************Emulator variables**************************************/
private:
    uint64_t pixelCounter;
    int width, height;

    double frameTime;
    std::chrono::steady_clock::time_point prvTime;
    std::chrono::steady_clock::time_point curTime;

private:
    void frameStart();
    void frameStop();

//Callbacks
private:
    GLInputCallbacks glInputCallbacks;
    VideoFile* vfile;
    ALPlayer* aplayer;
};


#endif