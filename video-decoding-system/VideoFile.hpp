#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

#include <string>

class VideoFile
{
public:
    VideoFile();

    bool open(std::string filepath);
    bool read_frame(uint8_t* frame_buffer, int64_t* pts);
    bool seek_frame(int64_t ts);
    bool close();
private:
    size_t width, height;
    AVRational time_base;

    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int video_stream_index;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
};