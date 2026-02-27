#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "Mixer.h"

const int maxMixers = 16;

class MixerManager
{
    private:
    std::unordered_map<std::string, int> mixerIndexes;
    std::vector<Mixer> mixers;

    int maxSamples;

    public:
    MixerManager();
    void SetMaxSamples(int maxSamp);
    void ZeroAllBuffers();
    void ApplyAllMixerEffects();
    void AddAllMixersIntoBuffer(float* buf, int numFrames);
    Mixer* AddMixer(std::string name);
    Mixer* GetMixer(std::string name);
};