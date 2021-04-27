#pragma once

extern "C" 
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

#include "HWDecoder.hpp"
#include <string>
#include <vector>

void print_error(int errcode);

class VideoStream
{
public:
    VideoStream();

    bool open(AVFormatContext* av_format_ctx);
    bool decode(AVFormatContext* av_format_ctx, AVPacket* av_packet);
    //bool seek_frame(int64_t ts);

    size_t get_width() { return width; }
    size_t get_height() { return height; }

    int get_stream_index() { return video_stream_index; }

    double get_seconds() { return pts * (double)time_base.num / (double)time_base.den; }
    uint8_t* get_frame() { return frame_buffer; }

private:
    HWDecoder hwdecoder;
    size_t width, height;
    AVRational time_base;

    uint8_t *frame_buffer = NULL;
    int64_t pts = 0;

    int video_stream_index;
    AVCodecContext* av_codec_ctx = NULL;
    AVFrame* av_frame = NULL;
    SwsContext* sws_scaler_ctx = NULL;
};