#include "AudioFile.hpp"

AudioFile::AudioFile()
{

}

bool AudioFile::open(AVFormatContext* av_format_ctx)
{
    int audio_stream_index = -1;
    AVCodecParameters *av_codec_params;
    AVCodec *av_codec;
    for (int i = 0; i < av_format_ctx->nb_streams; ++i)
    {
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if (!av_codec)
            continue;
        if(av_format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audio_stream_index = i;
            time_base = av_format_ctx->streams[i]->time_base;
            break;
        }
    }

    if (audio_stream_index == -1)
    {
        printf("Couldn't find valid audio stream inside file\n");
        return false;
    }

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