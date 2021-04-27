#pragma once

extern "C" 
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

static enum AVPixelFormat hw_pix_fmt;

class HWDecoder
{
private:
    /* data */
public:
    HWDecoder(/* args */);
    ~HWDecoder();

    bool initialize(AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params);
    bool decode(AVPacket* av_packet, AVFrame** av_frame);

    bool is_initialized() { return b_is_initialized; }

private:
    AVFrame*            sw_frame = NULL;
    AVCodecHWConfig*    hw_codec_config;
    AVBufferRef*        hw_device_ctx;
    AVPixelFormat       hw_pix_fmt_supported;
    bool                b_is_initialized = false;
};