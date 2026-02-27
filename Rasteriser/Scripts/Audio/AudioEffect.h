#pragma once
#include <cmath>
#include <algorithm>



class AudioEffect
{
    public:
    virtual void ApplyEffect(float* leftSample, float* rightSample) = 0; 
};

//basic hard clipping effect
class HardClip : public AudioEffect
{
    public:
    HardClip(float limit) { threshold = limit; };
    void ApplyEffect(float* leftSample, float* rightSample) override { 
        *leftSample = std::clamp(*leftSample, -threshold, threshold); 
        *rightSample = std::clamp(*rightSample, -threshold, threshold); 
    };
    
    private:
    float threshold;
};

//tanh based soft clipper
class SoftClip : public AudioEffect
{
    public:
    SoftClip(float clipperDrive, float gain) { drive = clipperDrive; outputGain = gain; };
    void ApplyEffect(float* leftSample, float* rightSample) override { 
        *leftSample = std::tanh(*leftSample * drive) * outputGain; 
        *rightSample = std::tanh(*rightSample * drive) * outputGain; 
    
    };

    private:
    float drive;
    float outputGain;
};

//one pole low pass filter design
class LowPassFilter : public AudioEffect
{
    public:
    LowPassFilter(float cutoff, float sampRate) { sampleRate = sampRate; SetCutoff(cutoff); };
    void SetCutoff(float cutoff)
    {
        alpha = 1.0f - std::exp(-2.0f * 3.1415f * cutoff / sampleRate);
    }
    void ApplyEffect(float* leftSample, float* rightSample) override
    {
        prevSampleL += alpha * (*leftSample - prevSampleL);
        *leftSample = prevSampleL;
        prevSampleR += alpha * (*rightSample - prevSampleR);
        *rightSample = prevSampleR;
    }

    private:
    float sampleRate;
    float alpha;

    float prevSampleL = 0;
    float prevSampleR = 0;
};