#include <iostream>
#include "GLRender.hpp"

int main(int, char**) 
{
    VideoFile vfile;
    vfile.open("../videoplayback.mp4");

    GLRender renderer(&vfile);
    renderer.initialize("../shaders/");
    renderer.display();

    return 0;
}
