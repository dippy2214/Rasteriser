#pragma once
#include <vector>
#include <memory>

#include "AudioEffect.h"

class Mixer
{
public:
    //Apply all listed mixer effects to the mixer's internal audio buffer
    void ApplyMixerEffects();

    std::vector<std::unique_ptr<AudioEffect>> effects;
    std::vector<float> buffer;
};