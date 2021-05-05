#ifndef AV_VIDEOFILE
#define AV_VIDEOFILE

#include "DataStream.h"

typedef struct CVideoFile
{

    AVFormatContext* av_format_ctx;
    AVPacket* av_packet;

    bool hwdecoding_video;
    bool hwdecoding_audio;

    CDataStream* vstream;
    CDataStream* astream;

} CVideoFile;

CVideoFile* video_file_alloc(void);

bool video_file_open(CVideoFile**, const char*);

bool video_file_read_frame(CVideoFile**);

bool video_file_allow_hwdecoding_video(CVideoFile**);
bool video_file_allow_hwdecoding_audio(CVideoFile**);

bool video_find_hwdecoder();
bool video_activate_hwdecoder();

bool video_file_close(CVideoFile**);

#endif