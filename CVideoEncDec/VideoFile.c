#include "VideoFile.h"
#include "helpers.h"

CVideoFile* video_file_alloc()
{
    CVideoFile* vfile = NULL;
    
    vfile = (CVideoFile*)malloc(sizeof(CVideoFile));
    vfile->vstream = data_stream_alloc();
    vfile->astream = data_stream_alloc();

    vfile->av_format_ctx = NULL;
    vfile->av_packet = NULL;
    vfile->hwdecoding_video = false;
    vfile->hwdecoding_audio = false;

    av_register_all();
    #ifdef VENC_DEBUG
    av_log_set_level(AV_LOG_DEBUG);
    #endif

    return vfile;
}

bool video_file_open_decode(CVideoFile** vfile_ptr, const char* filepath)
{
    CVideoFile* vfile = *vfile_ptr;

    vfile->av_format_ctx = avformat_alloc_context();
    if (!vfile->av_format_ctx)
    {
        printf("Couldn't created AVFormatContext\n");
        return false;
    }

    if (avformat_open_input(&vfile->av_format_ctx, filepath, NULL, NULL) != 0)
    {
        printf("Couldn't open video file\n");
        return false;
    }

    if(!data_stream_initialize_decode(&vfile->vstream, vfile->av_format_ctx, AVMEDIA_TYPE_VIDEO, vfile->hwdecoding_video))
    {
        printf("Couldn't open video stream\n");
    }

    if(!data_stream_initialize_decode(&vfile->astream, vfile->av_format_ctx, AVMEDIA_TYPE_AUDIO, vfile->hwdecoding_audio))
    {
        printf("Couldn't open audio stream\n");
    }

    vfile->av_packet = av_packet_alloc();
    if (!vfile->av_packet)
    {
        printf("Couldn't allocate AVPacket\n");
        return false;
    }

    return true;
}

bool video_file_open_encode(CVideoFile** vfile_ptr, const char* filepath)
{

}

bool video_file_read_frame(CVideoFile** vfile_ptr)
{
    int response;
    CVideoFile* vfile = *vfile_ptr;

    while ((response = av_read_frame(vfile->av_format_ctx, vfile->av_packet)) >= 0)
    {
        if (vfile->av_packet->stream_index == vfile->vstream->data_stream_index)
        {
            if(!data_stream_decode(&vfile->vstream, vfile->av_format_ctx, vfile->av_packet))
            {
                av_packet_unref(vfile->av_packet);
                continue;
            }
        }
        else if(vfile->av_packet->stream_index == vfile->astream->data_stream_index)
        {
            if(!data_stream_decode(&vfile->astream, vfile->av_format_ctx, vfile->av_packet))
            {
                av_packet_unref(vfile->av_packet);
                continue;
            }
        }
        else
        {
            av_packet_unref(vfile->av_packet);
            continue;
        }

        av_packet_unref(vfile->av_packet);
        break;
    }

    if(response < 0)
    {
        print_error(response);
        return false;
    }

    return true;
} 

bool video_file_write_frame(CVideoFile** vfile_ptr)
{

}

bool video_file_allow_hwdecoding_video(CVideoFile** vfile_ptr)
{
    CVideoFile* vfile = *vfile_ptr;
    vfile->hwdecoding_video = true;
    return true;
}

bool video_file_allow_hwdecoding_audio(CVideoFile** vfile_ptr)
{
    CVideoFile* vfile = *vfile_ptr;
    vfile->hwdecoding_audio = true;
    return true;
}

bool video_find_hwdecoder()
{
    return false;
}

bool video_activate_hwdecoder()
{
    return false;
}

bool video_file_close(CVideoFile** vfile_ptr)
{
    avformat_close_input(&(*vfile_ptr)->av_format_ctx);
    avformat_free_context((*vfile_ptr)->av_format_ctx);
    av_packet_free(&(*vfile_ptr)->av_packet);
    data_stream_close(&(*vfile_ptr)->vstream);
    data_stream_close(&(*vfile_ptr)->astream);
}