#include "VideoStream.hpp"

static AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt)
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

VideoStream::VideoStream()
{
    sws_freeContext(sws_scaler_ctx);
    av_frame_free(&av_frame);
    avcodec_free_context(&av_codec_ctx);
}

bool VideoStream::open(AVFormatContext* av_format_ctx)
{
    // Find the first valid video stream inside the file
    video_stream_index = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    for (int i = 0; i < av_format_ctx->nb_streams; ++i)
    {
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if (!av_codec)
            continue;
        if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            width = av_codec_params->width;
            height = av_codec_params->height;
            time_base = av_format_ctx->streams[i]->time_base;
            break;
        }
    }

    if (video_stream_index == -1)
    {
        printf("Couldn't find valid video stream inside file\n");
        return false;
    }

    // Set up a codec context for the decoder
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx)
    {
        printf("Couldn't create AVCodecContext\n");
        return false;
    }

    if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
    {
        printf("Couldn't initialize AVCodecContext\n");
        return false;
    }

    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0)
    {
        printf("Couldn't open codec\n");
        return false;
    }

    av_frame = av_frame_alloc();
    if (!av_frame)
    {
        printf("Couldn't allocate AVFrame\n");
        return false;
    }

    if (posix_memalign((void**)&frame_buffer, 128, width * height * 4) != 0) 
    {
        printf("Couldn't allocate frame buffer\n");
        return false;
    }

    return true;
}

bool VideoStream::decode(AVFormatContext* av_format_ctx, AVPacket* av_packet)
{
    int response;

    response = avcodec_send_packet(av_codec_ctx, av_packet);
    if (response < 0)
    {
        printf("Couldn't send video frame to decoder.\n");
        av_packet_unref(av_packet);
        return false;
    }

    response = avcodec_receive_frame(av_codec_ctx, av_frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
    {
        printf("Couldn't receive video frame from decoder. End of file or eagain.\n");
        av_packet_unref(av_packet);
        return false;
    }
    else if (response < 0)
    {
        printf("Couldn't receive video frame from decoder.\n");
        return false;
    }

    pts = av_frame->pts;

    // Set up sws scaler
    if (!sws_scaler_ctx)
    {
        auto source_pix_fmt = correct_for_deprecated_pixel_format(av_codec_ctx->pix_fmt);
        sws_scaler_ctx = sws_getContext(width, height, source_pix_fmt,
                                        width, height, AV_PIX_FMT_RGB0,
                                        SWS_BILINEAR, NULL, NULL, NULL);
    }

    if (!sws_scaler_ctx)
    {
        printf("Couldn't create sws scaler\n");
        return false;
    }

    uint8_t *dest[4] = {frame_buffer, NULL, NULL, NULL};
    int dest_linesize[4] = {width * 4, 0, 0, 0};
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);

    return true;
}