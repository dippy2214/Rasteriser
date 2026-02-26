#pragma once
#include <vector>

#include "AudioEffect.h"

class Mixer
{
public:
    std::vector<AudioEffect*> effects;
    std::vector<float> buffer;
};