#pragma once

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>

#include "VideoEncDec/AudioStream.hpp"
#include <string>

class ALPlayer
{
public:
    ALPlayer();
    ~ALPlayer();
    
    void play(AudioStream* stream);
private:
    ALboolean enumeration;
	const ALCchar *devices = NULL;
	std::string defaultDeviceName;
	ALCdevice *device = NULL;
	ALvoid *data = NULL;
	ALCcontext *context = NULL;
	ALuint buffer, source;
	ALfloat listenerOri[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error = 0;
	ALint source_state;
};