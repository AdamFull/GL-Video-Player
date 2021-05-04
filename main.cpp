#include <iostream>
#include "VideoRender/GLRender.hpp"
#include "VideoRender/GTexture2D.hpp"
#include "VideoRender/GSprite.hpp"
//#include "VideoRender/GTypeObject.hpp"
#include "VideoEncDec/VideoFile.hpp"
//#include "AudioPlayer/ALPlayer.hpp"


std::shared_ptr<GSprite> movie_screen;
std::shared_ptr<GTexture2D> movie_texture;
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

    //ALPlayer* aplayer = new ALPlayer();

    VideoStream* vstream = vfile.GetVideoStream();
    GLRender renderer(vstream->get_width(), vstream->get_height());
    //TypeManager typeMgr;

    #ifdef _WIN32
    renderer.initialize("../../resources/shaders/");
    //typeMgr.load("../../resources/fonts/OpenSans-Bold.ttf"); 
    #else
    renderer.initialize("../resources/shaders/");
    //typeMgr.load("../resources/fonts/OpenSans-Bold.ttf"); 
    #endif

    movie_screen = std::make_shared<GSprite>();
    movie_screen->init();
    movie_texture = std::make_shared<GTexture2D>();
    movie_texture->init();

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

        movie_texture->load(vstream->get_width(), vstream->get_height(), vstream->get_frame());
        movie_screen->update(movie_texture, glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), 0.f);
        renderer.add(movie_screen);
        //renderer.add(std::make_shared<GTypeObject>(&typeMgr, std::to_string(fps), 10, 20, 1));
        renderer.display();

        if(vfile.GetAudioStream()->is_decoded())
        {
            //aplayer->play(vfile.GetAudioStream());
            vfile.GetAudioStream()->set_decoded(false);
        }

        fps = 1.0/renderer.frameStop();
    }

    return 0;
}
