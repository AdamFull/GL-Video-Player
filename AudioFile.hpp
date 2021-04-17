#pragma once
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

class AudioFile
{
public:
    AudioFile();

    bool open(AVFormatContext* av_format_ctx);

private:
    AVCodecContext* av_codec_ctx = NULL;
    AVRational time_base;
};