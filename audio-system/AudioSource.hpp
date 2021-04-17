#pragma once

class AudioSource
{
public:
    AudioSource();

    bool open();
    bool load();
    bool close();

    bool play();
    bool pause();
    bool stop();

    bool set_device();

private:
    ALvoid* data;
    ALCcontext *context;
	ALsizei size, freq;
	ALenum format;
	ALuint buffer, source;
    ALCenum error;
	ALint source_state;
};