#include "ALPlayer.hpp"
#include <iostream>

void handleError(ALCenum err)
{
    std::string error_code;
    switch (err)
    {
        case AL_NO_ERROR:
            return;
        case ALC_INVALID_DEVICE:
            error_code = "ALC_INVALID_DEVICE";
            break;
        case ALC_INVALID_CONTEXT:
            error_code = "ALC_INVALID_CONTEXT";
            break;
        case ALC_INVALID_ENUM:
            error_code = "AL_INVALID_ENUM";
            break;
        case ALC_INVALID_VALUE:
            error_code = "AL_INVALID_VALUE";
            break;
        case AL_OUT_OF_MEMORY:
            error_code = "AL_OUT_OF_MEMORY";
            break;
        default:
            error_code = "UNKNOWN";
    }
    throw std::runtime_error(error_code);
}

ALPlayer::ALPlayer()
{
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
    {
        std::cout << "enumeration extension not available\n";
    }

    if (defaultDeviceName.empty())
    {
        defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    }

    device = alcOpenDevice(nullptr);
    if (!device)
    {
        //fprintf(stderr, "unable to open default device\n");
        std::cout << "ohoh\n";
    }

    error = alcGetError(device);

    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        //fprintf(stderr, "failed to make default context\n");
        std::cout << "ohoh2\n";
    }

    error = alcGetError(device);

    /* set orientation */
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
    handleError(alGetError());

    alGenSources((ALuint)1, &source);
    handleError(alGetError());

    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    error = alGetError();
}

ALPlayer::~ALPlayer()
{
    /* exit context */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void ALPlayer::play(AudioStream *stream)
{
    error = alGetError();
    alGenBuffers(1, &buffer);
    error = alGetError();
    alBufferData(buffer, AL_FORMAT_STEREO16, stream->get_sample(), stream->get_sample_size(), stream->get_sample_rate());
    error = alGetError();
    alSourcei(source, AL_BUFFER, buffer);
    error = alGetError();
    alSourcePlay(source);
    error = alGetError();

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    while (source_state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    }
}