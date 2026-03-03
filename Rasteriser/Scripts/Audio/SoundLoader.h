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
	//memory cleanup happens in 
	~SoundLoader();

	//load sound into memory from .wav file
	int LoadSound(const std::string& soundName, const std::string& fileName);
	//get sound by key from loaded data
	SoundData* GetSound(const std::string& key);
	
private:
	//resample a sound to a target sample rate
	//this is run on load of sounds to match soundcard sample rate
	void Resample(SoundData* target, int targetSampleRate);

	std::unordered_map<std::string, SoundData> sounds;
};