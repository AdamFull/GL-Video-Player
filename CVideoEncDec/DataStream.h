#ifndef AV_DATASTREAM
#define AV_DATASTREAM

#include "HWDecoder.h"

typedef struct CDataStream
{

    CHWDecoder*             hwdecoder;

    enum AVMediaType        stream_type;

    int32_t                 fwidth, fheight, swidth, sheight;

    bool                    vneed_rescaler_update;
    bool                    allow_hardware_decoding;

    AVRational              time_base;
    int32_t                 allocated_block_size;

    uint8_t*                block_buffer;
    int64_t                 pts;

    int32_t                 data_stream_index;
    AVCodecContext*         av_codec_ctx;
    AVFrame*                av_frame;
    AVFrame*                sc_frame;
    struct SwsContext*      sws_scaler_ctx;
    struct SwrContext*      swr_ctx;

}CDataStream;

CDataStream* data_stream_alloc(void);

bool data_stream_initialize(CDataStream**, AVFormatContext*, enum AVMediaType, bool);

bool data_stream_decode(CDataStream**, AVFormatContext*, AVPacket*);

double data_stream_get_pt_seconds(CDataStream**);

void data_stream_set_frame_size(CDataStream**, int32_t, int32_t);

bool data_stream_close(CDataStream**);


#endif