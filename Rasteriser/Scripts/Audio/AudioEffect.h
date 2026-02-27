#pragma once

class AudioEffect
{
    public:
    virtual void ApplyEffect(float* sample) = 0; 
};

class HardClip : public AudioEffect
{
    public:
    HardClip(float limit) { threshold = limit; };
    void ApplyEffect(float* sample) override { 
        if(*sample > threshold) *sample = threshold; 
        else if (*sample < -threshold) *sample = -threshold;
    };
    
    private:
    float threshold = 0.8f;
    
};