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

CHWDecoder* hw_alloc(void);

bool hw_initialize_decoder(CHWDecoder**, AVCodec*, AVCodecContext**, AVCodecParameters*);
bool hw_initialize_encoder(CHWDecoder**, AVCodec*, AVCodecContext**, AVCodecParameters*);
bool hw_is_coder_ready(CHWDecoder**);
bool hw_decode(CHWDecoder**, AVPacket*, AVFrame**);
bool hw_encode(CHWDecoder**, AVPacket*, AVFrame**);

bool hw_close(CHWDecoder**);

#endif