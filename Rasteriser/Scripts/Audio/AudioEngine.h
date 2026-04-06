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
    AudioSource* audioSource = nullptr;
	std::atomic<int> writtenFrameCount = 0;
};

struct AudioData
{
    MixerManager* mixerManager;
    Transform* listenerTransform;
	Voice voices[NUMVOICES];
};

class AudioEngine
{
    public:
    //init for sokol audio happens here
    AudioEngine();
    ~AudioEngine();
    //set active listener for entire audio engine
    void SetActiveListener(Transform* trans);
    //attach an audio source to a voice
    void AddSourceToVoice(AudioSource* audioSource, int voiceIndex);
    //create audio source with default parameters and mixer
    AudioSource* CreateAudioSource();

    Mixer* AddMixer(const std::string& name);
    Mixer* GetMixer(const std::string& name);

    int LoadSound(const std::string& soundName, std::string fileName);
    SoundData* GetSound(const std::string& name);

    
    private:
    //used in audio thread to apply spatial effects to audio with spatial data 
    //(source + valid listener) called from RenderVoiceToBuffer
    static void ApplySpatialEffectsToStereoSamples(AudioSource* audioSource, Transform* listener, float* leftSample, float*  rightSample);
    //per voice rendering called from audio thread
    static void RenderVoiceToBuffer(std::vector<float>* buffer, Voice* voice, int numFrames, Transform* listener);
    //our audio callback. all audio processing happens here
    //data passed to soundcard when function returns
    static void audioCallback(float *buffer, int numFrames, int numChannels, void *userData);
    
    int currentVoiceIndex = 0;

    MixerManager mixerManager;
    SoundLoader loader;
    AudioData audioData;
};
