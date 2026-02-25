#pragma once
#include "sokol_audio.h"
#include "SoundLoader.h"
#include "AudioSource.h"
#include "../Core/InputManager.h"
#include "../Core/Transform.h"

#include <vector>
#include <atomic>

static const int NUMVOICES = 8;

struct Voice
{
	std::atomic<bool> isActive = false;
	std::atomic<bool> isLooping = false;
	SoundData* soundData = nullptr;
    AudioSource* audioSource = nullptr;
	std::atomic<int> writtenFrameCount = 0;
};

struct AudioData
{
    Transform* listenerTransform;
	Voice voices[NUMVOICES];
};

class AudioEngine
{
    public:
    ~AudioEngine();
    int AudioInit(InputManager* inputMan);
    void AudioInputs();
    void SetActiveListener(Transform* trans);
    void AddSourceToVoice(AudioSource* audioSource, int voiceIndex);

    private:
    static void ApplySpatialEffectsToStereoSamples(AudioSource* audioSource, Transform* listener, float* leftSample, float*  rightSample);
    static void RenderVoiceToBuffer(float* buffer, Voice* voice, int frame, Transform* listener);
    static void audioCallback(float *buffer, int numFrames, int numChannels, void *userData);
    
    SoundLoader loader;
    AudioData audioData;
    InputManager* inputManager;

};
