#pragma once

#include "VideoStream.hpp"
#include "AudioStream.hpp"
#include <string>

class VideoFile
{
public:
    VideoFile();
    ~VideoFile();

    bool open(std::string filepath);
    bool read_frame();
    bool seek_frame();

    VideoStream* GetVideoStream() { return vstream; }
    AudioStream* GetAudioStream() { return astream; }

private:
    AVFormatContext* av_format_ctx = NULL;
    AVPacket* av_packet = NULL;
    int _seek_seconds = 0;

    VideoStream* vstream;
    AudioStream* astream;
};