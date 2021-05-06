#ifndef AV_HWACCELERATOR
#define AV_HWACCELERATOR

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdbool.h>


/**
 * The structure that determines the progress of decoding a video or audio clip.
 * 
 * Stores a pointer to the decoded frame that was returned from the codec 
 * and a pointer to the accelerator context.
 */
typedef struct CHardwareAccelerator
{
    AVFrame*            sw_frame;
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
 * @param hwdec_ptr Pointer to pointer to CHardwareAccelerator structure.
 * 
 * @param avcodec Name of the codec implementation.
 * 
 * @param av_codec_ctx main external of ffmpeg API structure.
 * 
 * @param av_codec_params This struct describes the properties of an encoded stream.
 *
 * @return Returns true if all initialization got well.
 */
bool hw_initialize_decoder(CHardwareAccelerator** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr Pointer to pointer to CHardwareAccelerator structure.
 * 
 * @param avcodec Name of the codec implementation.
 * 
 * @param av_codec_ctx main external of ffmpeg API structure.
 * 
 * @param av_codec_params This struct describes the properties of an encoded stream.
 *
 * @return Returns true if all initialization got well.
 */
bool hw_initialize_encoder(CHardwareAccelerator** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr Pointer to pointer to CHardwareAccelerator structure.
 *
 * @return Returns true if all initialization got well.
 */
bool hw_is_coder_ready(CHardwareAccelerator** hwdec_ptr);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr Pointer to pointer to CHardwareAccelerator structure.
 * 
 * @param av_packet
 * 
 * @param av_frame
 *
 * @return Returns true if all initialization got well.
 */
bool hw_decode(CHardwareAccelerator** hwdec_ptr, AVPacket* av_packet, AVFrame** av_frame);

/**
 * Release all allocated memory for CHardwareAccelerator structure.
 *
 * @param hwdec_ptr Pointer to pointer to CHardwareAccelerator structure.
 *
 * @return Returns true if all initialization got well.
 */
bool hw_close(CHardwareAccelerator** hwdec_ptr);

#endif