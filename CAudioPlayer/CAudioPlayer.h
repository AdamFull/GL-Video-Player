#ifndef AUDIOPLAYER
#define AUDIOPLAYER

#include <AL/al.h>
#include <AL/alc.h>
#include "CSample.h"
#include <stdbool.h>

typedef struct CAudioPlayer
{
    ALboolean enumeration;
	const ALCchar *devices;
	const char* defaultDeviceName;
    CSample* data_buffer;
	ALCdevice *device;
	ALCcontext *context;
	ALuint buffer, source;
	ALfloat listenerOri[6];
	ALboolean loop;
	ALCenum error;
	ALint source_state;

} CAudioPlayer;

CAudioPlayer* audio_player_alloc(void);

void audio_player_init(CAudioPlayer** player_ptr);

bool audio_player_set_sample(CAudioPlayer** player_ptr, uint8_t** data_ptr, int32_t sample_size, int32_t channels);

void audio_player_play(CAudioPlayer** player_ptr);

#endif