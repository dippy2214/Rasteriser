#pragma once

class AudioEffect
{
    virtual void ApplyEffect(float* sample) = 0; 
};

class HardClip : public AudioEffect
{
    float threshold = 0.8;
    void ApplyEffect(float* sample) override { if(*sample > 0.8f) *sample = 0.8f; };
};