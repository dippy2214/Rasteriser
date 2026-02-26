#include "MixerManager.h"

MixerManager::MixerManager()
{
    Mixer* defaultMix = AddMixer("default");
}

Mixer* MixerManager::AddMixer(std::string name)
{
    mixers.insert({name, Mixer()});
    return &mixers.at(name);
}

Mixer* MixerManager::GetMixer(std::string name)
{
    return &mixers.at(name);
}