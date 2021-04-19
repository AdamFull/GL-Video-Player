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

AVCodec* find_decoder()
{
    std::vector<std::string> vdecoders = {"h264_vda", "h264_vdpau", "h264_vaapi", "h264_cuvid"};
    AVCodec *av_codec;

    AVHWDeviceType devType = AV_HWDEVICE_TYPE_VDPAU;// = av_hwdevice_find_type_by_name("vdpau");
    while((devType = av_hwdevice_iterate_types(devType)) != AV_HWDEVICE_TYPE_NONE)
            fprintf(stderr, " %s", av_hwdevice_get_type_name(devType));

    for(std::string sdecoder : vdecoders)
    {
        av_codec = avcodec_find_decoder_by_name(sdecoder.c_str());
        if(av_codec)
        {
            printf("Couldn't find supported hwaccel video codec on device.\n");
            return av_codec;
        }
    }
    return nullptr;
}

bool VideoStream::open(AVFormatContext* av_format_ctx)
{
    // Find the first valid video stream inside the file
    video_stream_index = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    video_stream_index = av_find_best_stream(av_format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &av_codec, 0);
    av_codec_params = av_format_ctx->streams[video_stream_index]->codecpar;
    width = av_codec_params->width;
    height = av_codec_params->height;
    time_base = av_format_ctx->streams[video_stream_index]->time_base;

    if (video_stream_index == -1)
    {
        printf("Couldn't find valid video stream inside file\n");
        return false;
    }

    if(!hwdecoder.initialize(av_codec, &av_codec_ctx, av_codec_params))
    {
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

    if(hwdecoder.is_initialized())
    {
        if(!hwdecoder.decode(av_packet, &av_frame))
        {
            printf("Failed while decoding frame on gpu.\n");
        }
    }

    pts = av_frame->pts;

    // Set up sws scaler
    if (!sws_scaler_ctx)
    {
        auto source_pix_fmt = correct_for_deprecated_pixel_format((AVPixelFormat)av_frame->format);
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