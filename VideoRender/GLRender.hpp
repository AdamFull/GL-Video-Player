#ifndef __SE_MAIN__
#define __SE_MAIN__

#include "stdint.h"

#define GLEW_STATIC

#include "ShaderLoader.hpp"
#include "GRenderObject.hpp"
#include <GLFW/glfw3.h>
#include <queue>

#include <chrono>

using GLKeyboardCallback = void(*)(GLFWwindow* window, int key, int scancode, int action, int mods);
using GLMouseCallback = void(*)(GLFWwindow* window, double xpos, double ypos);
using GLMouseButtonCallback = void(*)(GLFWwindow* window, int button, int action, int mods);

using GLInputCallbacks = 
struct
{
    GLKeyboardCallback glKbCb;
    GLMouseCallback glMCb;
    GLMouseButtonCallback glMbCb;
};

class GLRender{
public:
    GLRender();
    GLRender(int sWidth, int sHeight);
    GLRender(int sWidth, int sHeight, GLKeyboardCallback GL_Keyboard_Callback,
            GLMouseCallback GL_Mouse_Callback, GLMouseButtonCallback GL_Mouse_Button_Callback);
    GLRender(int sWidth, int sHeight, GLInputCallbacks *glInputCallbacks);
    ~GLRender();

    void screen_size(int width, int height);
    void add_shader_folder_path(std::string newPath) { shaderLoader.addShaderFolder(newPath); }

    void add(std::shared_ptr<GRenderObject> robj);

    GLuint get_texture() { return tex_handle; }

    bool initialize();
    bool initialize(std::string shaderFolderPath);

    void display();
    void clear();

    bool is_running();

    double get_frame_time() { return frameTime; }

    double frameStart();
    double frameStop();

/**********************************GL private variables**************************************/
private:
    GLFWwindow* window;
    GLuint VBO, VAO, EBO;
    GLuint tex_handle;
    ShaderLoader shaderLoader;
    std::queue<std::shared_ptr<GRenderObject>> objects_to_render;

/**********************************Emulator variables**************************************/
private:
    uint64_t pixelCounter;
    int width, height;

    double frameTime;
    std::chrono::steady_clock::time_point prvTime;
    std::chrono::steady_clock::time_point curTime;

//Callbacks
private:
    GLInputCallbacks glInputCallbacks;
};


#endif