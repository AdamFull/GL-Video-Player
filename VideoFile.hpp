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
    bool read_frame();
    bool seek_frame(int64_t ts);
    bool close();

    size_t get_width() { return width; }
    size_t get_height() { return height; }

    double get_seconds() { return pts * (double)time_base.num / (double)time_base.den; }
    uint8_t* get_frame() { return frame_buffer; }
private:
    size_t width, height;
    AVRational time_base;

    uint8_t *frame_buffer = NULL;
    int64_t pts;

    AVFormatContext* av_format_ctx = NULL;
    AVCodecContext* av_codec_ctx = NULL;
    int video_stream_index;
    AVFrame* av_frame = NULL;
    AVPacket* av_packet = NULL;
    SwsContext* sws_scaler_ctx = NULL;
};