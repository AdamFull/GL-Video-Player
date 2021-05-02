#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
}
#endif

class AudioStream
{
public:
    AudioStream();

    bool open(AVFormatContext* av_format_ctx);
    bool decode(AVFormatContext* av_format_ctx, AVPacket* av_packet);

    int get_stream_index() { return audio_stream_index; }
    uint8_t* get_sample() { return frame_buffer; }
    int get_sample_rate() { return av_frame->sample_rate; }
    int get_sample_size() { return buffer_size; }
    bool is_decoded() { return decoded; }
    void set_decoded(bool value) { decoded = value; }

private:
    uint8_t *frame_buffer = NULL;
    AVCodecContext* av_codec_ctx = NULL;
    AVFrame* av_frame = NULL;
    int64_t pts;
    AVRational time_base;
    bool decoded = false;
    int buffer_size;
    int audio_stream_index;
    struct SwrContext *swr_ctx = NULL;
};