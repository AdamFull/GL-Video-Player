#include <iostream>
#include "GLRender.hpp"

int main(int, char**) 
{
    VideoFile vfile;
    #ifdef _WIN32
    vfile.open("../../videoplayback1.mp4");
    #else
    vfile.open("../videoplayback.mp4");
    #endif

    GLRender renderer(&vfile);
    #ifdef _WIN32
    renderer.initialize("../../shaders/");
    #else
    renderer.initialize("../shaders/");
    #endif
    renderer.display();

    return 0;
}
