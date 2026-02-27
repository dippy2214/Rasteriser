#include "MixerManager.h"
#include "AudioEngine.h"
#include <vector>

MixerManager::MixerManager()
{
    mixers.reserve(maxMixers);
    Mixer* defaultMix = AddMixer("default");
}

void MixerManager::SetMaxSamples(int maxSamp)
{
    maxSamples = maxSamp;
    for (int i = 0; i < mixers.size(); ++i)
    {
        mixers[i].buffer.resize(maxSamples);
    }
}

void MixerManager::ZeroAllBuffers()
{
    for (int i = 0; i < mixers.size(); ++i)
    {
        std::fill(mixers[i].buffer.begin(), mixers[i].buffer.end(), 0.0f);
    }
}

void MixerManager::AddAllMixersIntoBuffer(float* buf, int numFrames)
{
    int numSamples = numFrames * 2;

    // Assume buf was zeroed beforehand
    for (int i = 0; i < mixers.size(); ++i)
    {
        for (int j = 0; j < numSamples; ++j)
        {
            buf[j] += mixers[i].buffer[j];
        }
    }
}

Mixer* MixerManager::AddMixer(std::string name)
{
    if (maxMixers <= mixers.size()) return nullptr;

    mixerIndexes.insert({name, mixers.size()});

    mixers.emplace_back();
    if (maxSamples > 0)
    {
        mixers.back().buffer.resize(maxSamples);
    }

    return &mixers.back();
}

Mixer* MixerManager::GetMixer(std::string name)
{
    return &mixers[mixerIndexes.at(name)];
}