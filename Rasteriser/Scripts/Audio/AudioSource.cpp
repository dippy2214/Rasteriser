#include "AudioSource.h"

    void AudioSource::SetPosition(float3 pos)
    {
        this->position = pos;
        is3D = true;
    }
