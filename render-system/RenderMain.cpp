#include "RenderMain.hpp"

#include "stdio.h"
#include "stdlib.h"
#include <cstring>

static SE_MAIN* pData;

/*****************************************************************************************/
/****************************************CALLBACKS****************************************/
/*****************************************************************************************/
extern "C"
void window_size_callback(GLFWwindow* window, int width, int height) {
    pData->gfx.setWidth(width);
    pData->gfx.setHeight(height);
	glViewport(0, 0, width, height);		// Draw into entire window
}

/*****************************************************************************************/
void createPixelCallback(float x, float y, Color color){
    pData->createPixel(x, y, color);
}

/*****************************************************************************************/
/**************************************CONSTRUCTORS***************************************/
/*****************************************************************************************/
SE_MAIN::SE_MAIN(){
    gfx = SE_GFX(240, 320);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;
    glLogTracer = new GetConsoleLog();
    shaderLoader.setLogTracer(glLogTracer);
}

/*****************************************************************************************/
SE_MAIN::SE_MAIN(uint32_t sWidth, uint32_t sHeight){
    gfx = SE_GFX(sWidth, sHeight);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;
    glLogTracer = new GetConsoleLog();
    shaderLoader.setLogTracer(glLogTracer);
}

/*****************************************************************************************/
SE_MAIN::SE_MAIN(uint32_t sWidth, uint32_t sHeight, GLKeyboardCallback GL_Keyboard_Callback, 
    GLMouseCallback GL_Mouse_Callback, GLMouseButtonCallback GL_Mouse_Button_Callback){
    gfx = SE_GFX(sWidth, sHeight);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;

    glInputCallbacks.glKbCb = GL_Keyboard_Callback;
    glInputCallbacks.glMCb = GL_Mouse_Callback;
    glInputCallbacks.glMbCb = GL_Mouse_Button_Callback;
    glLogTracer = new GetConsoleLog();
    shaderLoader.setLogTracer(glLogTracer);
}

/*****************************************************************************************/
SE_MAIN::SE_MAIN(uint32_t sWidth, uint32_t sHeight, GLInputCallbacks *glInputCallbacks){
    gfx = SE_GFX(sWidth, sHeight);
    gfx.setCallback(createPixelCallback);
    pixelCounter = 0;
    this->glInputCallbacks = *glInputCallbacks;
    glLogTracer = new GetConsoleLog();
    shaderLoader.setLogTracer(glLogTracer);
}

/*****************************************************************************************/
SE_MAIN::~SE_MAIN(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete glLogTracer;
    glfwTerminate();
}

/*****************************************************************************************/
/**************************************INITIALISATION*************************************/
/*****************************************************************************************/
bool SE_MAIN::init(){
    return init("");
}
bool SE_MAIN::init(std::string shaderFolderPath){
    glLogTracer->SendLog("Initialisation started.");
    glfwInit();

    #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(gfx.getWidth(), gfx.getHeignt(), "Screen Simulator", NULL, NULL);
	if (window == NULL) {
		glLogTracer->SendLog("Failed to create GLFW window!");
		glfwTerminate();
		return false;
	}

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, glInputCallbacks.glKbCb);
    glfwSetCursorPosCallback(window, glInputCallbacks.glMCb);
    glfwSetMouseButtonCallback(window, glInputCallbacks.glMbCb);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glLogTracer->SendLog("Failed to initialize GLAD");
        return false;
    }

    //Shaders initialisation
    shaderLoader.addShaderFolder(shaderFolderPath);
    shaderLoader.loadShaders();

    //Init vertex array
    initVertexes();

    glEnable(GL_DOUBLEBUFFER);
    glDisable(GL_DEPTH_TEST);

    #ifdef GL_SHADING_LANGUAGE_VERSION
        glLogTracer->SendLog(std::string("Supported GLSL version is ") + (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    #endif
    glLogTracer->SendLog("Using GLEW version " + std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))));

    pData = this;
    return true;
}

/*****************************************************************************************/
void SE_MAIN::initVertexes(){

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, gfx.getBufferSize(), 0, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

/*****************************************************************************************/
void SE_MAIN::display(){
    frameTimeStart();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderLoader.getProgram());
    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, pixelCounter);

    glfwSwapBuffers(window);
    glfwPollEvents();
    frameTimeStop();
}

/*****************************************************************************************/
void SE_MAIN::clear(){
    //glDeleteBuffers();
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * width*height, 0, GL_DYNAMIC_DRAW);
    pixelCounter = 0;
}

/*****************************************************************************************/
bool SE_MAIN::getWindowWorker(){
    return !glfwWindowShouldClose(window);
}

/*****************************************************************************************/
void SE_MAIN::createPixel(float x, float y, Color color){
    Pixel pixel = {x, y, 0.0f, color};
    setPixel(pixel);
    pixelCounter++;
}

/*****************************************************************************************/
void SE_MAIN::setPixel(Pixel pixel){
    union dataToData{
        Pixel pix;
        float floats[6];
    } data;

    data.pix = pixel;
    glBufferSubData(GL_ARRAY_BUFFER, pixelCounter*sizeof(data.floats), sizeof(data.floats), &data.floats);
}

/*****************************************************************************************/
void SE_MAIN::frameTimeStart(){
    prvTime = std::chrono::steady_clock::now();
    curTime = std::chrono::steady_clock::now();
    prvTime = curTime;
}

/*****************************************************************************************/
void SE_MAIN::frameTimeStop(){
    curTime = std::chrono::steady_clock::now();
    frameTime = std::chrono::duration<double>(curTime - prvTime).count();
}