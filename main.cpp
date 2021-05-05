#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "VideoEncDec/VideoFile.hpp"

#define AVG_BUFFER_SIZE 10
bool is_file_not_end = true;
float lastTime = 0;
float fps = 0, fps_max = 0, fps_min = 0, fps_avg = 0;
float avg_fps_buffer[AVG_BUFFER_SIZE] = {0};
int avg_buffer_counter = 0;

int main()
{
    VideoFile vfile;
    #ifdef _WIN32
    vfile.open("../../resources/samples/videoplayback1.mp4");
    #else
    vfile.open("../resources/samples/videoplayback3.mp4");
    #endif

    VideoStream* vstream = vfile.GetVideoStream();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Video Player");
    window.setVerticalSyncEnabled(true);
    //vstream->set_rescale_size(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

    sf::Font font;
    #ifdef _WIN32
    font.loadFromFile("../../resources/fonts/OpenSans-Bold.ttf");
    #else
    font.loadFromFile("../resources/fonts/OpenSans-Bold.ttf");
    #endif
    sf::Text cur_fps, max_fps, min_fps;
    cur_fps.setFont(font);
    max_fps.setFont(font);
    max_fps.setPosition(0, 40);
    min_fps.setFont(font);
    min_fps.setPosition(0, 80);

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock clock;
    sf::Clock video_timer;

    video_timer.restart();
    texture.create(vstream->get_width(), vstream->get_width());
    while(is_file_not_end)
    {
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

        double pt_in_seconds = vfile.GetVideoStream()->get_seconds();
        while (pt_in_seconds > video_timer.getElapsedTime().asSeconds());

        sf::Vector2u wind_size = window.getSize();
        vstream->set_rescale_size(wind_size.x, wind_size.y);
        
        window.clear();
        is_file_not_end = vfile.read_frame();

        texture.update(vstream->get_frame(), wind_size.x, wind_size.y, 0, 0);
        sprite.setTexture(texture);
        cur_fps.setString("AVG: " + std::to_string(fps_avg));
        max_fps.setString("MAX: " + std::to_string(fps_max));
        min_fps.setString("MIN: " + std::to_string(fps_min));

        window.draw(sprite);
        window.draw(cur_fps);
        window.draw(max_fps);
        window.draw(min_fps);
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

    window.close();
}