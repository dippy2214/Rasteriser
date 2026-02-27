#pragma once
#include <vector>
#include <memory>

#include "AudioEffect.h"

class Mixer
{
public:
    void ApplyMixerEffects();

    std::vector<std::unique_ptr<AudioEffect>> effects;
    std::vector<float> buffer;
};