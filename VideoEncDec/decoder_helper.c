#include "decoder_helper.h"

enum AVPixelFormat correct_for_deprecated_pixel_format(enum AVPixelFormat pix_fmt)
{
    // Fix swscaler deprecated pixel format warning
    // (YUVJ has been deprecated, change pixel format to regular YUV)
    switch (pix_fmt)
    {
    case AV_PIX_FMT_YUVJ420P:
        return AV_PIX_FMT_YUV420P;
    case AV_PIX_FMT_YUVJ422P:
        return AV_PIX_FMT_YUV422P;
    case AV_PIX_FMT_YUVJ444P:
        return AV_PIX_FMT_YUV444P;
    case AV_PIX_FMT_YUVJ440P:
        return AV_PIX_FMT_YUV440P;
    default:
        return pix_fmt;
    }
}

#if defined(OLD_DECODER_FORMAT)
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

#else

enum AVPixelFormat get_hw_format(AVCodecContext *av_codec_ctx, const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;
    int ret;

    for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++)
    {
        if (*p == hw_pix_fmt)
        {
            AVBufferRef*        hw_decoder_ctx = av_codec_ctx->hw_device_ctx;
            AVHWFramesContext*  hw_frames_ctx;

            av_codec_ctx->hw_frames_ctx = av_hwframe_ctx_alloc(hw_decoder_ctx);
            if (!av_codec_ctx->hw_frames_ctx)
            {
                fprintf(stderr, "Failed to get HW surface format.\n");
                return AV_PIX_FMT_NONE;
            }

            hw_frames_ctx = (AVHWFramesContext*)av_codec_ctx->hw_frames_ctx->data;

            hw_frames_ctx->format            = hw_pix_fmt;
            hw_frames_ctx->sw_format         = av_codec_ctx->sw_pix_fmt;
            hw_frames_ctx->width             = FFALIGN(av_codec_ctx->coded_width,  32);
            hw_frames_ctx->height            = FFALIGN(av_codec_ctx->coded_height, 32);
            hw_frames_ctx->initial_pool_size = 32;

            ret = av_hwframe_ctx_init(av_codec_ctx->hw_frames_ctx);
            if (ret < 0)
            {
                fprintf(stderr, "Failed to get HW surface format.\n");
                return AV_PIX_FMT_NONE;
            }

            return *p;
        }
    }
}
#endif