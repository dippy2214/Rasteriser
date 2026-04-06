#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "Mixer.h"
#include "AudioConstants.h"

class MixerManager
{
public:
    //buffer initialisations
    MixerManager();
    //set a maximum sample count for mixer buffers
    void SetMaxSamples(int maxSamp);
    //zero all mixer buffers
    void ZeroAllBuffers();
    //apply all mixer effects to internal buffer of mixers
    void ApplyAllMixerEffects();
    //add all contents of all mixer buffers into an output buffer
    void AddAllMixersIntoBuffer(float* buf, int numFrames);
    //register new mixer to manager
    Mixer* AddMixer(std::string name);
    //get mixer from manager map
    Mixer* GetMixer(std::string name);
private:
    std::vector<Mixer> mixers;
    std::unordered_map<std::string, int> mixerIndexes;
    int maxSamples;
};