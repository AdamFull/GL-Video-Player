#ifndef AV_HELPERS
#define AV_HELPERS

#include <stdbool.h>
#include <libavcodec/avcodec.h>

#if defined(_WIN32)
    #define HW_DECODER_NAME "d3d11va"
#elif defined(__linux__)
    #define HW_DECODER_NAME "vdpau"
#elif defined(__APPLE__)
    #if TARGET_IPHONE_SIMULATOR
        #define HW_DECODER_NAME "videotoolbox"
    #elif TARGET_OS_IPHONE
        #define HW_DECODER_NAME "videotoolbox"
    #elif TARGET_OS_MAC
        #define HW_DECODER_NAME "vdpau"
    #else
        #error "Unknown Apple platform"
    #endif
#elif defined(__ANDROID__)
    #define HW_DECODER_NAME "mediacodec"
#endif

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool realloc_frame(AVFrame**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
void print_error(int);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
enum AVPixelFormat correct_for_deprecated_pixel_format(enum AVPixelFormat pix_fmt);

#endif