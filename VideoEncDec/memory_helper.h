#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <libavcodec/avcodec.h>
#include <stdbool.h>

#ifdef _WIN32
    #define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)
#endif

bool realloc_frame(AVFrame** av_frame);

#ifdef __cplusplus
}
#endif