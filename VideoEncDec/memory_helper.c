#include "memory_helper.h"

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
}