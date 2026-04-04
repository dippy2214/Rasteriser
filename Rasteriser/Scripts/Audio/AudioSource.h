#pragma once
#include "../Core/Transform.h"
#include "../Core/RingBuffer.h"
#include "Mixer.h"
#include "SoundLoader.h"
#include <vector>


class AudioSource : public Transform
{
public:
    void SetPosition(float3 pos);

    float minDistance = 10;
    float maxDistance = 30;
    float rolloff = 5;    

    std::atomic<bool> isActive = false;
	std::atomic<bool> isLooping = false;
	SoundData* soundData = nullptr;
    std::atomic<bool> is3D = false;

    Mixer* mixer;
    //buffers for interaural time difference spatialisation
    RingBuffer<float> leftITDBuffer = RingBuffer<float>(64);
    RingBuffer<float> rightITDBuffer = RingBuffer<float>(64);
};