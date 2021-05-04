#include "GLRender.hpp"

#include "stdio.h"
#include "stdlib.h"
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLRender *pData;

/*****************************************************************************************/
/****************************************CALLBACKS****************************************/
/*****************************************************************************************/
extern "C" void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/*****************************************************************************************/
/**************************************CONSTRUCTORS***************************************/
/*****************************************************************************************/
GLRender::GLRender()
{
}

/*****************************************************************************************/
GLRender::GLRender(int sWidth, int sHeight)
{
    width = sWidth;
    height = sHeight;
}

/*****************************************************************************************/
GLRender::GLRender(int sWidth, int sHeight, GLKeyboardCallback GL_Keyboard_Callback,
                 GLMouseCallback GL_Mouse_Callback, GLMouseButtonCallback GL_Mouse_Button_Callback)
{
    width = sWidth;
    height = sHeight;
    glInputCallbacks.glKbCb = GL_Keyboard_Callback;
    glInputCallbacks.glMCb = GL_Mouse_Callback;
    glInputCallbacks.glMbCb = GL_Mouse_Button_Callback;
}

/*****************************************************************************************/
GLRender::GLRender(int sWidth, int sHeight, GLInputCallbacks *glInputCallbacks)
{
    width = sWidth;
    height = sHeight;
    this->glInputCallbacks = *glInputCallbacks;
}

/*****************************************************************************************/
GLRender::~GLRender()
{
    glfwTerminate();
}

/*****************************************************************************************/
/**************************************INITIALISATION*************************************/
/*****************************************************************************************/
bool GLRender::initialize()
{
    return initialize("");
}
bool GLRender::initialize(std::string shaderFolderPath)
{
    std::cout << "Initialisation started." << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    window = glfwCreateWindow(width, height, "video player", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetFramebufferSizeCallback(window, window_size_callback);
    /*if(glInputCallbacks.glKbCb)
        glfwSetKeyCallback(window, glInputCallbacks.glKbCb);
    if(glInputCallbacks.glMCb)
        glfwSetCursorPosCallback(window, glInputCallbacks.glMCb);
    if(glInputCallbacks.glMbCb)
        glfwSetMouseButtonCallback(window, glInputCallbacks.glMbCb);*/

    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Shaders initialisation
    shaderLoader.addShaderFolder(shaderFolderPath);
    shaderLoader.loadShaders();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    shaderLoader.getShader("default").execute();
    shaderLoader.getShader("default").setInt("image", 0);
    shaderLoader.getShader("default").SetMatrix4("projection", projection);

    /*float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // bottom right
         1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // top right
        -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // top left 
        -1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // bottom left
    };*/
    /*unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };*/

    /*glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/

#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << std::string("Supported GLSL version is ") + (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
    std::cout << "Using GLEW version " + std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << std::endl;

    pData = this;
    return true;
}

void GLRender::add(std::shared_ptr<GRenderObject> robj)
{
    objects_to_render.emplace(robj);
}

/*****************************************************************************************/
void GLRender::display()
{
    process_input_callback(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    while (!objects_to_render.empty())
    {
        std::shared_ptr<GRenderObject> obj = objects_to_render.front();
        obj->render(shaderLoader.getShader("default"));
        objects_to_render.pop();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

/*****************************************************************************************/
void GLRender::clear()
{
    //glDeleteBuffers();
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * width*height, 0, GL_DYNAMIC_DRAW);
    pixelCounter = 0;
}

/*****************************************************************************************/
bool GLRender::is_running()
{
    return !glfwWindowShouldClose(window);
}

/*****************************************************************************************/
double GLRender::frameStart()
{
    prvTime = std::chrono::steady_clock::now();
    curTime = std::chrono::steady_clock::now();
    prvTime = curTime;
    return 0.0;
}

/*****************************************************************************************/
double GLRender::frameStop()
{
    curTime = std::chrono::steady_clock::now();
    frameTime = std::chrono::duration<double>(curTime - prvTime).count();
    return frameTime;
}