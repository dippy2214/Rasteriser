#pragma once
#include <cmath>
#include <algorithm>
#include "../Core/RingBuffer.h"
#include "AudioConstants.h"

//simple audioeffect interface used for mixer effects
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
        alpha = 1.0f - std::exp(-2.0f * PI * cutoff / sampleRate);
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

//one pole high pass filter
class HighPassFilter : public AudioEffect
{
public:
    HighPassFilter(float cutoff, float sampRate)
    {
        sampleRate = sampRate;
        SetCutoff(cutoff);
    }

    void SetCutoff(float cutoff)
    {
        alpha = 1.0f - std::exp(-2.0f * PI * cutoff / sampleRate);
    }

    void ApplyEffect(float* leftSample, float* rightSample) override
    {
        // Left
        prevLowL += alpha * (*leftSample - prevLowL);
        *leftSample = *leftSample - prevLowL;

        // Right
        prevLowR += alpha * (*rightSample - prevLowR);
        *rightSample = *rightSample - prevLowR;
    }

private:
    float sampleRate;
    float alpha;
    float prevLowL = 0.0f;
    float prevLowR = 0.0f;
};

//two pole(?) band pass filter
class BandPassFilter : public AudioEffect
{
public:
    BandPassFilter(float lowCut, float highCut, float sampRate) 
        : hp(lowCut, sampRate), lp(highCut, sampRate) {}

    void ApplyEffect(float* leftSample, float* rightSample) override
    {
        hp.ApplyEffect(leftSample, rightSample);
        lp.ApplyEffect(leftSample, rightSample);
    }

private:
    HighPassFilter hp;
    LowPassFilter  lp;
};

//simple delay effect
class Delay : public AudioEffect
{
public:
    Delay(float seconds, int sampleRate)
    {
        size_t delaySamples = static_cast<size_t>(sampleRate * seconds);

        leftBuffer = RingBuffer<float>(delaySamples);
        rightBuffer = RingBuffer<float>(delaySamples);

        frameDelay = delaySamples - 1; // ensure valid index
    }

    void ApplyEffect(float* leftSample, float* rightSample) override
    {
        //avoid off by one sample errors by storing these first
        float delayedL = leftBuffer.readRelative(frameDelay);
        float delayedR = rightBuffer.readRelative(frameDelay);

        leftBuffer.write(*leftSample);
        rightBuffer.write(*rightSample);

        *leftSample += delayedL;
        *rightSample += delayedR;
    }

private:
    RingBuffer<float> leftBuffer;
    RingBuffer<float> rightBuffer;
    //stored as size_t to work best with ringbuffer implementation
    size_t frameDelay;
};