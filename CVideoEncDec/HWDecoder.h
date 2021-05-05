#ifndef AV_HWDECODER
#define AV_HWDECODER

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdbool.h>

typedef struct CHWDecoder
{
    AVFrame*            sw_frame;
    AVCodecHWConfig*    hw_codec_config;
    AVBufferRef*        hw_device_ctx;
    bool                b_is_initialized;
} CHWDecoder;

CHWDecoder* hwdecoder_alloc(void);

bool hwdecoder_initialize(CHWDecoder**, AVCodec*, AVCodecContext**, AVCodecParameters*);
bool hwdecoder_is_ready(CHWDecoder**);
bool hwdecoder_decode(CHWDecoder**, AVPacket*, AVFrame**);

bool hwdecoder_close(CHWDecoder**);

#endif