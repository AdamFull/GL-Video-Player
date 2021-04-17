#ifndef __SE_MAIN__
#define __SE_MAIN__

#include "stdint.h"

#define GLEW_STATIC

#include "ShaderLoader.hpp"
//#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "SE_GFX.h"

#include <chrono>

typedef void(*GLKeyboardCallback)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*GLMouseCallback)(GLFWwindow* window, double xpos, double ypos);
typedef void(*GLMouseButtonCallback)(GLFWwindow* window, int button, int action, int mods);

typedef struct{
    GLKeyboardCallback glKbCb;
    GLMouseCallback glMCb;
    GLMouseButtonCallback glMbCb;
}GLInputCallbacks;

class SE_MAIN{
public:
    SE_MAIN();
    SE_MAIN(uint32_t sWidth, uint32_t sHeight);
    SE_MAIN(uint32_t sWidth, uint32_t sHeight, GLKeyboardCallback GL_Keyboard_Callback,
            GLMouseCallback GL_Mouse_Callback, GLMouseButtonCallback GL_Mouse_Button_Callback);
    SE_MAIN(uint32_t sWidth, uint32_t sHeight, GLInputCallbacks *glInputCallbacks);
    ~SE_MAIN();

    void setScreenSize(uint32_t width, uint32_t height);

    void addShaderFolder(std::string newPath) { shaderLoader.addShaderFolder(newPath); }

    bool init();
    bool init(std::string shaderFolderPath);

    void initVertexes();

    void setColor(Pixel &pixel);
    void display();
    void clear();

    bool getWindowWorker();

    double getFrameTime() { return frameTime; }

public:
    SE_GFX gfx;

/**********************************GL private variables**************************************/
private:
    GLFWwindow* window;
    GLuint VBO, VAO;
    ShaderLoader shaderLoader;

/**********************************Emulator variables**************************************/
private:
    uint64_t pixelCounter;

    double frameTime;
    std::chrono::steady_clock::time_point prvTime;
    std::chrono::steady_clock::time_point curTime;

private:
    void setPixel(Pixel pixel);
    void frameTimeStart();
    void frameTimeStop();

//Callbacks
private:
    GLInputCallbacks glInputCallbacks;
    CreateLogTracer *glLogTracer;

public:
    void createPixel(float x, float y, Color color);
};


#endif