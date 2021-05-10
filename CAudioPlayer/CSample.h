#ifndef CSAMPLE
#define CSAMPLE

#include <stdint.h>

typedef struct CSample
{

    uint8_t* sample_data;
    int32_t sample_size;
    int32_t channels;

} CSample;

CSample* sample_alloc(void);

void sample_set(CSample** sample_ptr, uint8_t** data_ptr, int32_t sample_size, int32_t channels);

#endif