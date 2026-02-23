#pragma once
#include "../Core/Transform.h"

class AudioSource : public Transform
{
    public:
    float minDistance = 10;
    float maxDistance = 30;
    float rolloff = 5;    
};