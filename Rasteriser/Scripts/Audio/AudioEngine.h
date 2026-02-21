#pragma once
#include "sokol_audio.h"
#include "SoundLoader.h"
#include "../Core/InputManager.h"

#include <vector>
#include <atomic>

static const int NUMVOICES = 8;

struct Voice
{
	std::atomic<bool> isActive = false;
	std::atomic<bool> isLooping = false;
	std::atomic<SoundData*> soundData = nullptr;
	std::atomic<int> writtenFrameCount = 0;
};

struct AudioData
{
	Voice voices[NUMVOICES];
};

class AudioEngine
{
    public:
    ~AudioEngine();
    int AudioInit(InputManager* inputMan);
    void AudioInputs();

    private:
    static void RenderVoiceToBuffer(float* buffer, Voice* voice, int frame);
    static void audioCallback(float *buffer, int numFrames, int numChannels,	void *userData);
    
    SoundLoader loader;
    AudioData audioData;
    InputManager* inputManager;

};
