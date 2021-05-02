#include "AudioStream.hpp"

AudioStream::AudioStream()
{

}

bool AudioStream::open(AVFormatContext* av_format_ctx)
{
    audio_stream_index = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    audio_stream_index = av_find_best_stream(av_format_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &av_codec, 0);

    if (audio_stream_index < 0)
    {
        printf("Couldn't find valid audio stream inside file\n");
        return false;
    }

    av_codec_params = av_format_ctx->streams[audio_stream_index]->codecpar;
    time_base = av_format_ctx->streams[audio_stream_index]->time_base;

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

    av_frame = av_frame_alloc();

    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0)
    {
        printf("Couldn't open codec\n");
        return false;
    }

    swr_ctx = swr_alloc_set_opts(NULL, av_codec_ctx->channel_layout,
                  AV_SAMPLE_FMT_S16, av_codec_ctx->sample_rate,
                  av_codec_ctx->channel_layout, av_codec_ctx->sample_fmt,
                  av_codec_ctx->sample_rate, 0, NULL);

    if(!swr_ctx)
    {
        printf("Couldn't allocate swr context\n");
        return false;
    }

	if(swr_init(swr_ctx) < 0)
    {
        printf("Couldn't initialize swr context\n");
        return false;
    }
    return true;
}   

bool AudioStream::decode(AVFormatContext* av_format_ctx, AVPacket* av_packet)
{
    int response, gotFrame = 0;

    //response = avcodec_decode_audio4(av_codec_ctx, av_frame, &gotFrame, av_packet);
    
    response = avcodec_send_packet(av_codec_ctx, av_packet);
    if (response < 0)
    {
        printf("Couldn't send audio frame to decoder.\n");
        return false;
    }

    response = avcodec_receive_frame(av_codec_ctx, av_frame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
    {
        printf("Couldn't receive audio frame from decoder. End of file or eagain.\n");
        return false;
    }
    else if (response < 0)
    {
        printf("Couldn't receive audio frame from decoder.\n");
        return false;
    }

    pts = av_frame->pts;

    int out_linesize;
    av_samples_alloc(&frame_buffer, &out_linesize, av_frame->channels, av_frame->sample_rate, AV_SAMPLE_FMT_FLT, 0);

    const uint8_t **input = (const uint8_t **)av_frame->extended_data;
    response = swr_convert(swr_ctx, &frame_buffer, av_frame->sample_rate, input, av_frame->nb_samples);
    buffer_size = av_get_bytes_per_sample(AV_SAMPLE_FMT_FLT) * av_frame->channels * response;

    if (response < 0)
    {
        printf("Couldn't convert audio frame.\n");
        return false;
    }

    decoded = true;
    return true;
}