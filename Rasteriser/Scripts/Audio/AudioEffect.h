#pragma once
#include <algorithm>

class AudioEffect
{
    public:
    virtual void ApplyEffect(float* sample) = 0; 
};

class HardClip : public AudioEffect
{
    public:
    HardClip(float limit) { threshold = limit; };
    void ApplyEffect(float* sample) override { *sample = std::clamp(*sample, -threshold, threshold); };
    
    private:
    float threshold;
};

class SoftClip : public AudioEffect
{
    public:
    SoftClip(float clipperDrive, float gain) { drive = clipperDrive; outputGain = gain; };
    void ApplyEffect(float* sample) override { *sample = std::tanh(*sample * drive) * outputGain; };

    private:
    float drive;
    float outputGain;
};

