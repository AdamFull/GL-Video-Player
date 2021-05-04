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
    ~VideoStream();

    bool open(AVFormatContext* av_format_ctx);
    bool decode(AVFormatContext* av_format_ctx, AVPacket* av_packet);
    //bool seek_frame(int64_t ts);

    int get_width() { return width; }
    int get_height() { return height; }

    int get_stream_index() { return video_stream_index; }

    void set_rescale_size(int nwidth, int nheight);

    double get_seconds() { return pts * (double)time_base.num / (double)time_base.den; }
    uint8_t* get_frame() { return frame_buffer; }
    size_t get_size() { return allocated_frame_size; }

private:
    HWDecoder hwdecoder;
    int width, height, to_width, to_height;
    bool needs_reinit_scaler;
    AVRational time_base;
    size_t allocated_frame_size;

    uint8_t *frame_buffer;
    int64_t pts;

    int video_stream_index;
    AVCodecContext* av_codec_ctx;
    AVFrame* av_frame, *sc_frame;
    SwsContext* sws_scaler_ctx;
};