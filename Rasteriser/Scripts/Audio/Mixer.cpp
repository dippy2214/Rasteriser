#include "Mixer.h"

void Mixer::ApplyMixerEffects()
{
    //iterate samples externally for optimisation and 
    //in increments of two for stereo playback
    for (int s = 0; s < buffer.size(); s+=2)
    {
        for (int i = 0; i < effects.size(); ++i)
        {
            effects[i].get()->ApplyEffect(&buffer[s], &buffer[s+1]);
        }
    }
}