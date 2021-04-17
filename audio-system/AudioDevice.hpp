#pragma once

class AudioDevice()
{
public:
    AudioDevice();

private:
    ALCchar* devices;
    ALCchar* default_device;

    ALCdevice* device;
};