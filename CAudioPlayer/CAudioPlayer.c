#include "CAudioPlayer.h"
#include <string.h>
#include <stdio.h>

CAudioPlayer* audio_player_alloc()
{
    CAudioPlayer* aplayer = NULL;
    aplayer = (CAudioPlayer*)malloc(sizeof(CAudioPlayer));

    ALfloat orient[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    memcpy(aplayer->listenerOri, orient, sizeof(orient));
    aplayer->devices = NULL;
    aplayer->defaultDeviceName = NULL;
    aplayer->context = NULL;
    aplayer->data_buffer = sample_alloc();
    aplayer->loop = AL_FALSE;
    aplayer->error = 0;

    return aplayer;
}

void audio_player_init(CAudioPlayer** player_ptr)
{
    CAudioPlayer* player = *player_ptr;
    player->enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (player->enumeration == AL_FALSE)
    {
        printf("enumeration extension not available\n");
    }

    if (!player->defaultDeviceName)
    {
        player->defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    }

    player->device = alcOpenDevice(NULL);
    if (!player->device)
    {
        printf("unable to open default device\n");
    }

    player->error = alcGetError(player->device);

    player->context = alcCreateContext(player->device, NULL);
    if (!alcMakeContextCurrent(player->context))
    {
        printf("failed to make default context\n");
    }

    player->error = alcGetError(player->device);

    /* set orientation */
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, player->listenerOri);

    alGenSources((ALuint)1, &player->source);

    alSourcef(player->source, AL_PITCH, 1);
    alSourcef(player->source, AL_GAIN, 1);
    alSource3f(player->source, AL_POSITION, 0, 0, 0);
    alSource3f(player->source, AL_VELOCITY, 0, 0, 0);
    alSourcei(player->source, AL_LOOPING, AL_FALSE);

    player->error = alGetError();
}

bool audio_player_set_sample(CAudioPlayer** player_ptr, uint8_t** data_ptr, int32_t sample_size, int32_t channels)
{
    CAudioPlayer* player = *player_ptr;

    sample_set(&player->data_buffer, data_ptr, sample_size, channels);
}

void audio_player_play(CAudioPlayer** player_ptr)
{
    CAudioPlayer* player = *player_ptr;

    player->error = alGetError();
    alGenBuffers(1, &player->buffer);
    player->error = alGetError();
    alBufferData(player->buffer, AL_FORMAT_STEREO16, player->data_buffer->sample_data, player->data_buffer->sample_size, player->data_buffer->channels);
    player->error = alGetError();
    alSourcei(player->source, AL_BUFFER, player->buffer);
    player->error = alGetError();
    alSourcePlay(player->source);
    player->error = alGetError();

    alGetSourcei(player->source, AL_SOURCE_STATE, &player->source_state);
    while (player->source_state == AL_PLAYING)
    {
        alGetSourcei(player->source, AL_SOURCE_STATE, &player->source_state);
    }
}