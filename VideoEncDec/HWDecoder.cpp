#include "HWDecoder.hpp"
#include <libswscale/swscale.h>
#include "memory_helper.h"

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
    #   error "Unknown Apple platform"
    #endif
#elif defined(__ANDROID__)
#define HW_DECODER_NAME "mediacodec"
#endif

enum AVPixelFormat get_hw_format(AVCodecContext *av_codec_ctx, const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != -1; p++)
    {
        if (*p == hw_pix_fmt)
            return *p;
    }

    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

HWDecoder::HWDecoder(/* args */)
{
    hw_pix_fmt_supported = AV_PIX_FMT_NONE;
}

HWDecoder::~HWDecoder()
{
    av_frame_free(&sw_frame);
}

bool HWDecoder::initialize(AVCodec *av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters *av_codec_params)
{
    //vaapi|vdpau|cuvid|dxva2|d3d11va|qvs|videotoolbox|drm|opencl|mediacodec|vulkan
    AVHWDeviceType devType = av_hwdevice_find_type_by_name(HW_DECODER_NAME);

    for (int i = 0;; i++)
    {
        const AVCodecHWConfig *config = avcodec_get_hw_config(av_codec, i);
        if (!config)
        {
            fprintf(stderr, "Decoder %s does not support device type %s.\n",
                    av_codec->name, av_hwdevice_get_type_name(devType));
            return false;
        }

        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && config->device_type == devType)
        {
            hw_pix_fmt = config->pix_fmt;
            break;
        }
    }

    hw_device_ctx = av_hwdevice_ctx_alloc(devType);

    *av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx)
    {
        printf("Couldn't create AVCodecContext\n");
        return false;
    }

    if (avcodec_parameters_to_context(*av_codec_ctx, av_codec_params) < 0)
    {
        printf("Couldn't initialize AVCodecContext\n");
        return false;
    }

    (*av_codec_ctx)->get_format  = get_hw_format;

    //Initialize hardware context
    int err = 0;
    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, devType, NULL, NULL, 0)) < 0)
    {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return false;
    }

    (*av_codec_ctx)->hw_device_ctx = av_buffer_ref(hw_device_ctx);

    if (avcodec_open2(*av_codec_ctx, av_codec, NULL) < 0)
    {
        printf("Couldn't open codec\n");
        return false;
    }

    b_is_initialized = true;

    return true;
}

bool HWDecoder::decode(AVPacket* av_packet, AVFrame** av_frame)
{
    int ret = 0;

    realloc_frame(&sw_frame);

    if ((*av_frame)->format == hw_pix_fmt)
    {
        if ((ret = av_hwframe_transfer_data(sw_frame, *av_frame, 0)) < 0)
        {
            fprintf(stderr, "Error transferring the data to system memory\n");
            av_frame_free(&sw_frame);
            return false;
        }
        
        sw_frame->pict_type = (*av_frame)->pict_type;
        sw_frame->pts = (*av_frame)->pts;
        sw_frame->pkt_pts = (*av_frame)->pkt_pts;
        sw_frame->pkt_dts = (*av_frame)->pkt_dts;
        sw_frame->pkt_pos = (*av_frame)->pkt_pos;
        sw_frame->pkt_duration = (*av_frame)->pkt_duration;
        sw_frame->pkt_size = (*av_frame)->pkt_size;
        sw_frame->pkt_size = (*av_frame)->pkt_size;
        sw_frame->color_range = (*av_frame)->color_range;
        sw_frame->color_primaries = (*av_frame)->color_primaries;
        sw_frame->color_trc = (*av_frame)->color_trc;
        sw_frame->colorspace = (*av_frame)->colorspace;
        sw_frame->chroma_location = (*av_frame)->chroma_location;
        sw_frame->best_effort_timestamp = (*av_frame)->best_effort_timestamp;
        av_frame_ref(*av_frame, sw_frame);
        av_frame_unref(sw_frame);
        return true;
    }
    return false;
}