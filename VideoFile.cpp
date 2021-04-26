#include "VideoFile.hpp"

VideoFile::VideoFile()
{
    vstream = new VideoStream();
    astream = new AudioStream();
}

VideoFile::~VideoFile()
{
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_packet_free(&av_packet);
    delete vstream;
    delete astream;
}

bool VideoFile::open(std::string filename)
{
    av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx)
    {
        printf("Couldn't created AVFormatContext\n");
        return false;
    }

    if (avformat_open_input(&av_format_ctx, filename.c_str(), NULL, NULL) != 0)
    {
        printf("Couldn't open video file\n");
        return false;
    }

    if(!vstream->open(av_format_ctx))
    {
        printf("Couldn't open video stream\n");
    }

    if(!astream->open(av_format_ctx))
    {
        printf("Couldn't open audio stream\n");
    }

    av_packet = av_packet_alloc();
    if (!av_packet)
    {
        printf("Couldn't allocate AVPacket\n");
        return false;
    }

    return true;
}

bool VideoFile::read_frame()
{
    int response;
    while (av_read_frame(av_format_ctx, av_packet) >= 0)
    {
        if (av_packet->stream_index == vstream->get_stream_index())
        {
            if(!vstream->decode(av_format_ctx, av_packet))
            {
                av_packet_unref(av_packet);
                continue;
            }
            video_fr++;
        }
        else if(av_packet->stream_index == astream->get_stream_index())
        {
            if(!astream->decode(av_format_ctx, av_packet))
            {
                av_packet_unref(av_packet);
                continue;
            }
            audio_fr++;
        }
        else
        {
            av_packet_unref(av_packet);
            continue;
        }

        av_packet_unref(av_packet);
        break;
    }

    return true;
}

bool VideoFile::seek_frame()
{
    /*int seek_target = vstream-> + (_seek_seconds / av_q2d(av_format_ctx->streams[vstream->get_stream_index()]->time_base));

    seek_target = std::max(seek_target, 0);

    av_free_packet(av_packet);
    av_freep(av_packet);

    av_seek_frame(av_format_ctx, vstream->get_stream_index(), seek_target, AVSEEK_FLAG_BACKWARD);*/
    return false;
}