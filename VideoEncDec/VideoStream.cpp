#include "VideoStream.hpp"
#include <iostream>
#include "memory_helper.h"
#include "decoder_helper.h"
#include <libavutil/error.h>

void print_error(int errcode)
{
    std::string serror;
    serror.resize(64);
    av_make_error_string(serror.data(), 64, errcode);
    std::cout << serror << std::endl;
}

VideoStream::VideoStream()
{
    width = to_width = 0;
    height = to_height = 0;
    needs_reinit_scaler = false;

    frame_buffer = NULL;
    pts = 0;

    video_stream_index = -1;
    av_codec_ctx = NULL;
    av_frame = NULL;
    sws_scaler_ctx = NULL;
}

VideoStream::~VideoStream()
{
    sws_freeContext(sws_scaler_ctx);
    av_frame_free(&av_frame);
    av_frame_free(&sc_frame);
    avcodec_free_context(&av_codec_ctx);
    av_free(frame_buffer);
    //std::free(frame_buffer);
}

bool VideoStream::open(AVFormatContext* av_format_ctx)
{
    // Find the first valid video stream inside the file
    video_stream_index = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    video_stream_index = av_find_best_stream(av_format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &av_codec, 0);

    if (video_stream_index == -1)
    {
        printf("Couldn't find valid video stream inside file\n");
        print_error(video_stream_index);
        return false;
    }

    av_codec_params = av_format_ctx->streams[video_stream_index]->codecpar;
    width = av_codec_params->width;
    height = av_codec_params->height;
    time_base = av_format_ctx->streams[video_stream_index]->time_base;

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

    if (!(sc_frame = av_frame_alloc()))
    {
        fprintf(stderr, "Can not alloc frame\n");
        return false;
    }

    /*frame_buffer = (uint8_t*)std::aligned_alloc(32, width * height * 4);
    if (!frame_buffer) 
    {
        printf("Couldn't allocate frame buffer\n");
        return false;
    }*/

    return true;
}

void VideoStream::set_rescale_size(int nwidth, int nheight)
{
    if(nwidth != to_width && nheight != to_height)
    {
        to_width = nwidth;
        to_height = nheight;
        needs_reinit_scaler = true;

        /*if (frame_buffer)
            std::free(frame_buffer);

        frame_buffer = (uint8_t*)std::aligned_alloc(32, width * height * 4);
        if (!frame_buffer) 
            printf("Couldn't allocate frame buffer\n");*/
    }
}

bool VideoStream::decode(AVFormatContext* av_format_ctx, AVPacket* av_packet)
{
    int response;

    response = avcodec_send_packet(av_codec_ctx, av_packet);
    if (response < 0)
    {
        printf("Couldn't send video frame to decoder.\n");
        print_error(response);
        return false;
    }

    realloc_frame(&av_frame);

    response = avcodec_receive_frame(av_codec_ctx, av_frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
    {
        av_frame_free(&av_frame);
        return false;
    }
    else if (response < 0)
    {
        print_error(response);
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
    if (!sws_scaler_ctx || needs_reinit_scaler)
    {
        if(needs_reinit_scaler && sws_scaler_ctx)
            sws_freeContext(sws_scaler_ctx);

        auto source_pix_fmt = correct_for_deprecated_pixel_format((AVPixelFormat)av_frame->format);
        //Send here frame params
        //BEST QUALITY/PERFOMANCE: SWS_BICUBLIN, SWS_AREA
        sws_scaler_ctx = sws_getContext(width, height, source_pix_fmt,
                                        to_width, to_height, AV_PIX_FMT_RGB0,
                                        SWS_BICUBLIN, NULL, NULL, NULL);

        if(needs_reinit_scaler && frame_buffer)
            av_free(frame_buffer);

        int num_bytes = avpicture_get_size(AV_PIX_FMT_RGB0, to_width, to_height);
        frame_buffer = (uint8_t *)av_malloc(num_bytes*sizeof(uint8_t));
        needs_reinit_scaler = false;
    }

    if (!sws_scaler_ctx)
    {
        printf("Couldn't create sws scaler\n");
        return false;
    }

    avpicture_fill((AVPicture*)sc_frame, frame_buffer, AV_PIX_FMT_RGB0, to_width, to_height);
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, height, sc_frame->data, sc_frame->linesize);

    return true;
}