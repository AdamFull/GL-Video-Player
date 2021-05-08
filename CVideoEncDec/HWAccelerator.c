#include "HWAccelerator.h"
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

CHardwareAccelerator* hw_alloc()
{
    CHardwareAccelerator* hwdec = NULL;
    hwdec = (CHardwareAccelerator*)malloc(sizeof(CHardwareAccelerator));

    hwdec->sw_frame = NULL;
    hwdec->hw_device_ctx = NULL;
    hwdec->extradata = NULL;
    hwdec->b_is_initialized = false;

    return hwdec;
}

bool hw_initialize_decoder(CHardwareAccelerator** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params)
{
    CHardwareAccelerator* hwdec = *hwdec_ptr;

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

    av_opt_set((*av_codec_ctx)->priv_data, "preset", "fast", 0);

    (*av_codec_ctx)->get_format  = get_hw_format;

    //Initialize hardware context
    int err = 0;
    if ((err = av_hwdevice_ctx_create(&hwdec->hw_device_ctx, devType, NULL, NULL, 0)) < 0)
    {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return false;
    }

    (*av_codec_ctx)->hw_device_ctx = av_buffer_ref(hwdec->hw_device_ctx);

    /*AVBitStreamFilterContext* av_bit_filter_ctx = av_bitstream_filter_init("h264_mp4toannexb");
    av_bitstream_filter_filter(av_bit_filter_ctx, *av_codec_ctx, NULL, &hwdec->extradata, &hwdec->extradata_size, NULL, 0, 0);
    av_bitstream_filter_close(av_bit_filter_ctx);

    (*av_codec_ctx)->extradata = (uint8_t*)av_malloc(hwdec->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);*/


    if (avcodec_open2(*av_codec_ctx, av_codec, NULL) < 0)
    {
        printf("Couldn't open codec\n");
        return false;
    }

    hwdec->b_is_initialized = true;

    return true;
}

bool hw_initialize_encoder(CHardwareAccelerator** hwdec_ptr, AVCodec* av_codec, AVCodecContext** av_codec_ctx, AVCodecParameters* av_codec_params)
{

}

bool hw_is_coder_ready(CHardwareAccelerator** hwdec_ptr)
{
    CHardwareAccelerator* hwdec = *hwdec_ptr;
    return hwdec->b_is_initialized;
}

bool hw_decode(CHardwareAccelerator** hwdec_ptr, AVPacket* av_packet, AVFrame** av_frame)
{
    int ret = 0;
    CHardwareAccelerator* hwdec = *hwdec_ptr;

    if ((*av_frame)->format == hw_pix_fmt)
    {
        if ((ret = av_hwframe_transfer_data(hwdec->sw_frame, *av_frame, 0)) < 0)
        {
            fprintf(stderr, "Error transferring the data to system memory\n");
            return false;
        }
        
        memcpy((*av_frame)->data, hwdec->sw_frame->data, sizeof(hwdec->sw_frame->data));
        memcpy((*av_frame)->linesize, hwdec->sw_frame->linesize, sizeof(hwdec->sw_frame->linesize));
        (*av_frame)->format = hwdec->sw_frame->format;
        return true;
    }
    return false;
}

bool hw_encode(CHardwareAccelerator** hwdec_ptr, AVPacket* av_packet, AVFrame** av_frame)
{

}

bool hw_close(CHardwareAccelerator** hwdec_ptr)
{
    av_frame_free(&(*hwdec_ptr)->sw_frame);
    av_buffer_unref(&(*hwdec_ptr)->hw_device_ctx);
    free(*hwdec_ptr);
}