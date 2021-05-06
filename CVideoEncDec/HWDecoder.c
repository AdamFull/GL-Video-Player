#include "HWDecoder.h"
#include "helpers.h"

#include <libswscale/swscale.h>

enum AVPixelFormat hw_pix_fmt = AV_PIX_FMT_NONE;

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

CHWDecoder* hw_alloc()
{
    CHWDecoder* hwdec = NULL;
    hwdec = (CHWDecoder*)malloc(sizeof(CHWDecoder));

    hwdec->sw_frame = NULL;
    hwdec->hw_device_ctx = NULL;
    hwdec->b_is_initialized = false;

    return hwdec;
}

bool hw_initialize_decoder(CHWDecoder** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params)
{
    CHWDecoder* hwdec = *hwdec_ptr;

    //vaapi|vdpau|cuvid|dxva2|d3d11va|qvs|videotoolbox|drm|opencl|mediacodec|vulkan
    enum AVHWDeviceType devType = av_hwdevice_find_type_by_name(HW_DECODER_NAME);

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

    hwdec->hw_device_ctx = av_hwdevice_ctx_alloc(devType);

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

    (*av_codec_ctx)->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    (*av_codec_ctx)->get_format  = get_hw_format;

    //Initialize hardware context
    int err = 0;
    if ((err = av_hwdevice_ctx_create(&hwdec->hw_device_ctx, devType, NULL, NULL, 0)) < 0)
    {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return false;
    }

    (*av_codec_ctx)->hw_device_ctx = av_buffer_ref(hwdec->hw_device_ctx);

    if (avcodec_open2(*av_codec_ctx, av_codec, NULL) < 0)
    {
        printf("Couldn't open codec\n");
        return false;
    }

    hwdec->b_is_initialized = true;

    return true;
}

bool hw_initialize_encoder(CHWDecoder** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params)
{

}

bool hw_is_coder_ready(CHWDecoder** hwdec_ptr)
{
    CHWDecoder* hwdec = *hwdec_ptr;
    return hwdec->b_is_initialized;
}

bool hw_decode(CHWDecoder** hwdec_ptr, AVPacket* av_packet, AVFrame** av_frame)
{
    int ret = 0;
    CHWDecoder* hwdec = *hwdec_ptr;

    realloc_frame(&hwdec->sw_frame);

    if ((*av_frame)->format == hw_pix_fmt)
    {
        if ((ret = av_hwframe_transfer_data(hwdec->sw_frame, *av_frame, 0)) < 0)
        {
            fprintf(stderr, "Error transferring the data to system memory\n");
            av_frame_free(&hwdec->sw_frame);
            return false;
        }
        
        hwdec->sw_frame->pict_type = (*av_frame)->pict_type;
        hwdec->sw_frame->pts = (*av_frame)->pts;
        hwdec->sw_frame->pkt_pts = (*av_frame)->pkt_pts;
        hwdec->sw_frame->pkt_dts = (*av_frame)->pkt_dts;
        hwdec->sw_frame->pkt_pos = (*av_frame)->pkt_pos;
        hwdec->sw_frame->pkt_duration = (*av_frame)->pkt_duration;
        hwdec->sw_frame->pkt_size = (*av_frame)->pkt_size;
        hwdec->sw_frame->pkt_size = (*av_frame)->pkt_size;
        hwdec->sw_frame->color_range = (*av_frame)->color_range;
        hwdec->sw_frame->color_primaries = (*av_frame)->color_primaries;
        hwdec->sw_frame->color_trc = (*av_frame)->color_trc;
        hwdec->sw_frame->colorspace = (*av_frame)->colorspace;
        hwdec->sw_frame->chroma_location = (*av_frame)->chroma_location;
        hwdec->sw_frame->best_effort_timestamp = (*av_frame)->best_effort_timestamp;
        av_frame_ref(*av_frame, hwdec->sw_frame);
        av_frame_unref(hwdec->sw_frame);
        return true;
    }
    return false;
}

bool hw_encode(CHWDecoder** hwdec_ptr, AVPacket* av_packet, AVFrame** av_frame)
{

}

bool hw_close(CHWDecoder** hwdec_ptr)
{
    av_frame_free(&(*hwdec_ptr)->sw_frame);
    free(*hwdec_ptr);
}