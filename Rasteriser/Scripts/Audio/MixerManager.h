#pragma once
#include <unordered_map>
#include <string>

#include "Mixer.h"

class MixerManager
{
    std::unordered_map<std::string, Mixer> mixers;

    public:
    MixerManager();
    Mixer* AddMixer(std::string name);
    Mixer* GetMixer(std::string name);
};