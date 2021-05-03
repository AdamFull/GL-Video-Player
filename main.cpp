#include <iostream>
#include "VideoRender/GLRender.hpp"
#include "VideoRender/GTextureObject.hpp"
#include "VideoRender/GTypeObject.hpp"
#include "VideoEncDec/VideoFile.hpp"
#include "AudioPlayer/ALPlayer.hpp"

bool is_file_not_end = true;
double fps = 0.0;

int main(int, char**) 
{
    VideoFile vfile;
    #ifdef _WIN32
    vfile.open("../../resources/samples/videoplayback1.mp4");
    #else
    vfile.open("../resources/samples/videoplayback1.mp4");
    #endif

    ALPlayer* aplayer = new ALPlayer();

    VideoStream* vstream = vfile.GetVideoStream();
    GLRender renderer(vstream->get_width(), vstream->get_height());
    TypeManager typeMgr;

    #ifdef _WIN32
    renderer.initialize("../../resources/shaders/");
    typeMgr.load("../../resources/fonts/OpenSans-Bold.ttf"); 
    #else
    renderer.initialize("../resources/shaders/");
    typeMgr.load("../resources/fonts/OpenSans-Bold.ttf"); 
    #endif

    renderer.create_texture();

    glfwSetTime(0.0);
    while(is_file_not_end)
    {
        renderer.frameStart();
        is_file_not_end = vfile.read_frame();
        double pt_in_seconds = vfile.GetVideoStream()->get_seconds();
        while (pt_in_seconds > glfwGetTime()) 
        {
            glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
        }

        renderer.add(std::make_shared<GTextureObject>(vstream->get_frame(), vstream->get_width(), vstream->get_height(), renderer.get_texture()));
        renderer.add(std::make_shared<GTypeObject>(&typeMgr, std::to_string(fps), 10, 20, 1));
        renderer.display();

        if(vfile.GetAudioStream()->is_decoded())
        {
            aplayer->play(vfile.GetAudioStream());
            vfile.GetAudioStream()->set_decoded(false);
        }

        fps = 1.0/renderer.frameStop();
    }

    return 0;
}
