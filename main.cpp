#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

extern "C"
{
#include "CVideoEncDec/VideoFile.h"
}

#define AVG_BUFFER_SIZE 10
bool is_file_not_end = true;
float lastTime = 0;
float fps = 0, fps_max = 0, fps_min = 0, fps_avg = 0;
float avg_fps_buffer[AVG_BUFFER_SIZE] = {0};
int avg_buffer_counter = 0;

int main()
{
    CVideoFile* vfile;
    vfile = video_file_alloc();
    video_file_allow_hwdecoding_video(&vfile);
    
    #ifdef _WIN32
    video_file_open_decode(&vfile, "../../resources/samples/videoplayback1.mp4");
    #else
    video_file_open_decode(&vfile, "../resources/samples/videoplayback3.mp4");
    #endif

    CDataStream* vstream = vfile->vstream;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Video Player");
    window.setVerticalSyncEnabled(true);
    //vstream->set_rescale_size(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

    sf::Font font;
    #ifdef _WIN32
    font.loadFromFile("../../resources/fonts/OpenSans-Bold.ttf");
    #else
    font.loadFromFile("../resources/fonts/OpenSans-Bold.ttf");
    #endif
    sf::Text cur_fps("", font, 16);
    //cur_fps.setFont(font);

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock clock;
    sf::Clock video_timer;

    video_timer.restart();
    while(is_file_not_end)
    {
        std::string fps_stats;
        if(fps > fps_max)
            fps_max = fps;
        else if(fps < fps_min)
            fps_min = fps;

        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        double pt_in_seconds = data_stream_get_pt_seconds(&vstream);
        while (pt_in_seconds > video_timer.getElapsedTime().asSeconds());

        sf::Vector2u wind_size = window.getSize();
        data_stream_set_frame_size(&vstream, wind_size.x, wind_size.y);
        
        window.clear();
        is_file_not_end = video_file_read_frame(&vfile);

        texture.create(wind_size.x, wind_size.y);
        texture.update(vstream->block_buffer, wind_size.x, wind_size.y, 0, 0);

        sprite.setTexture(texture);
        fps_stats += "FPS: " + std::to_string(fps) + "\n";
        fps_stats += "AVG: " + std::to_string(fps_avg) + "\n";
        fps_stats += "MAX: " + std::to_string(fps_max) + "\n";
        fps_stats += "MIN: " + std::to_string(fps_min) + "\n";

        cur_fps.setString(fps_stats);

        window.draw(sprite);
        window.draw(cur_fps);
        window.display();

        fps = 1.f / clock.restart().asSeconds();
        if(fps_min < 1) 
            fps_min = fps;

        if(avg_buffer_counter < AVG_BUFFER_SIZE)
        {
            avg_fps_buffer[avg_buffer_counter] = fps;
            avg_buffer_counter++;
        }
        else
        {
            fps_avg = 0;
            for(int i = 0; i < AVG_BUFFER_SIZE; i++)
                fps_avg+=fps/AVG_BUFFER_SIZE;

            avg_buffer_counter = 0;
        }
    }

    video_file_close(&vfile);

    window.close();
}