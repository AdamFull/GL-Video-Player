#include "CSample.h"
#include <stdlib.h>
#include <string.h>

CSample* sample_alloc()
{
    CSample* sample = NULL;
    sample = (CSample*)malloc(sizeof(CSample));

    sample->sample_data = NULL;
    sample->sample_size = 0;
    sample->channels = 0;

    return sample;
}

void sample_set(CSample** sample_ptr, uint8_t** data_ptr, int32_t sample_size, int32_t channels)
{
    CSample* sample = *sample_ptr;
    
    sample->sample_data = *data_ptr;
    sample->sample_size = sample_size;
    sample->channels = channels;
}