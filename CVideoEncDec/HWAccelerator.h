#ifndef AV_HWACCELERATOR
#define AV_HWACCELERATOR

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdbool.h>

typedef struct CHardwareAccelerator
{
    AVFrame*            sw_frame;
    AVCodecHWConfig*    hw_codec_config;
    AVBufferRef*        hw_device_ctx;
    bool                b_is_initialized;
} CHardwareAccelerator;

/**
 * Allocate an CHardwareAccelerator and set its fields to default values.
 *
 * @return An CHardwareAccelerator filled with default values or NULL on failure.
 */
CHardwareAccelerator* hw_alloc(void);

/**
 * Initialize an CHardwareAccelerator as decoder.
 *
 * @param hwdec_ptr
 * @param avcodec
 * @param av_codec_ctx
 * @param av_codec_params
 *
 * @return Returns true if all initialization got well.
 */
bool hw_initialize_decoder(CHardwareAccelerator**, AVCodec*, AVCodecContext**, AVCodecParameters*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 * @param avcodec
 * @param av_codec_ctx
 * @param av_codec_params
 *
 * @return Returns true if all initialization got well.
 */
bool hw_initialize_encoder(CHardwareAccelerator**, AVCodec*, AVCodecContext**, AVCodecParameters*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool hw_is_coder_ready(CHardwareAccelerator**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 * @param av_packet
 * @param av_frame
 *
 * @return Returns true if all initialization got well.
 */
bool hw_decode(CHardwareAccelerator**, AVPacket*, AVFrame**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 * @param av_packet
 * @param av_frame
 *
 * @return Returns true if all initialization got well.
 */
bool hw_encode(CHardwareAccelerator**, AVPacket*, AVFrame**);

/**
 * Release all allocated memory for CHardwareAccelerator structure.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool hw_close(CHardwareAccelerator**);

#endif