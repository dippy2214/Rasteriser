#include "Mixer.h"

void Mixer::ApplyMixerEffects()
{
    for (int s = 0; s < buffer.size(); s+=2)
    {
        for (int i = 0; i < effects.size(); ++i)
        {
            effects[i].get()->ApplyEffect(&buffer[s], &buffer[s+1]);
        }
    }
}