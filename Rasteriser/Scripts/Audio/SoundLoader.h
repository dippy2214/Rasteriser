#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "dr_wav.h"

struct SoundData
{
	unsigned int channels = 0;
	unsigned int sampleRate = 0;
	drwav_uint64 numFrames = 0;
	float* rawData;
};

class SoundLoader
{
public:
	~SoundLoader();

	int LoadSound(const std::string& soundName, const std::string& fileName);
	SoundData* GetSound(const std::string& key);
	void ResampleMono(SoundData* target, int targetSampleRate);

private:
	std::unordered_map<std::string, SoundData> sounds;
};