#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#define OLD_DECODER_FORMAT

#if defined(_WIN32)
    #define HW_DECODER_NAME "none"
#elif defined(__linux__)
    #define HW_DECODER_NAME "cuda"
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

enum AVPixelFormat correct_for_deprecated_pixel_format(enum AVPixelFormat pix_fmt);

#ifdef __cplusplus
}
#endif