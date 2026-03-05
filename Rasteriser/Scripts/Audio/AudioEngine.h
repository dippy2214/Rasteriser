#pragma once
#include "sokol_audio.h"
#include "SoundLoader.h"
#include "AudioSource.h"
#include "Mixer.h"
#include "MixerManager.h"
#include "AudioConstants.h"
#include "../Core/InputManager.h"
#include "../Core/Transform.h"


#include <vector>
#include <atomic>


struct Voice
{
	std::atomic<bool> isActive = false;
	std::atomic<bool> isLooping = false;
	SoundData* soundData = nullptr;
    AudioSource* audioSource = nullptr;
    Mixer* mixer;
	std::atomic<int> writtenFrameCount = 0;
};

struct AudioData
{
    MixerManager mixerManager;
    Transform* listenerTransform;
	Voice voices[NUMVOICES];
};

class AudioEngine
{
    public:
    ~AudioEngine();
    //all initialisation happens in here
    int AudioInit(InputManager* inputMan);
    //simple input checks for toggling voices in testing
    void AudioInputs();
    //set active listener for entire audio engine
    void SetActiveListener(Transform* trans);
    //attach an audio source to a voice
    void AddSourceToVoice(AudioSource* audioSource, int voiceIndex);

    
    private:
    //used in audio thread to apply spatial effects to audio with spatial data 
    //(source + valid listener) called from RenderVoiceToBuffer
    static void ApplySpatialEffectsToStereoSamples(AudioSource* audioSource, Transform* listener, float* leftSample, float*  rightSample);
    //per voice rendering called from audio thread
    static void RenderVoiceToBuffer(std::vector<float>* buffer, Voice* voice, int numFrames, Transform* listener);
    //our audio callback. all audio processing happens here
    //data passed to soundcard when function returns
    static void audioCallback(float *buffer, int numFrames, int numChannels, void *userData);
    
    SoundLoader loader;
    AudioData audioData;
    InputManager* inputManager;
};
