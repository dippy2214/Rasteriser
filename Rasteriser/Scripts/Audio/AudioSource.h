#pragma once
#include "../Core/Transform.h"
#include "../Core/RingBuffer.h"
#include <vector>


class AudioSource : public Transform
{
    public:
    float minDistance = 10;
    float maxDistance = 30;
    float rolloff = 5;    

    //buffers for interaural time difference spatialisation
    RingBuffer<float> leftITDBuffer = RingBuffer<float>(64);
    RingBuffer<float> rightITDBuffer = RingBuffer<float>(64);
};