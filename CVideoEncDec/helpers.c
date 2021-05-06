#include "helpers.h"

void print_error(int errcode)
{
    char serror[64] = {0};
    av_make_error_string(serror, 64, errcode);
    serror[strlen(serror)] = '\n';
    printf(serror + '\0');
}

bool realloc_frame(AVFrame** av_frame)
{
    if(av_frame)
    {
        //reallocating frame
        av_frame_free(av_frame);
        if (!(*av_frame = av_frame_alloc()))
        {
            fprintf(stderr, "Can not alloc frame\n");
            return false;
        }
    }
    else
    {
        if (!(*av_frame = av_frame_alloc()))
        {
            fprintf(stderr, "Can not alloc frame\n");
            return false;
        }
    }
    return false;
}

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