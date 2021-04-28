#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <libavcodec/avcodec.h>
#include <stdbool.h>

bool realloc_frame(AVFrame** av_frame);

#ifdef __cplusplus
}
#endif