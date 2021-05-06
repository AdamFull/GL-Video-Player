#ifndef AV_DATASTREAM
#define AV_DATASTREAM

#include "HWAccelerator.h"

typedef bool (*data_stream_get_sw_data_t)(struct CDataStream**);

typedef struct CDataStream
{

    CHardwareAccelerator*             hwdecoder;

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

    data_stream_get_sw_data_t data_stream_get_sw_data_ptr;

}CDataStream;

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
CDataStream* data_stream_alloc(void);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_initialize_decode(CDataStream**, AVFormatContext*, enum AVMediaType, bool);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_initialize_encode(CDataStream**, enum AVCodecID, AVCodecParameters*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_decode(CDataStream**, AVFormatContext*, AVPacket*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_encode(CDataStream**, AVFormatContext*, AVPacket*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
double data_stream_get_pt_seconds(CDataStream**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
void data_stream_set_frame_size(CDataStream**, int32_t, int32_t);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_get_sw_data_audio(CDataStream**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_get_sw_data_video(CDataStream**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool data_stream_close(CDataStream**);


#endif