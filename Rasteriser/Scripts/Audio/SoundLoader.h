#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>


//vorbis aligned sounddata struct
struct SoundData
{
	int channels = 0;
	int sampleRate = 0;
	int numFrames = 0;
	float* rawData = nullptr;
};

class SoundLoader
{
public:
	//memory cleanup happens here 
	~SoundLoader();

	//load sound into memory from .wav file
	int LoadSound(const std::string& soundName, std::string fileName);
	//get sound by key from loaded data
	SoundData* GetSound(const std::string& key);
	
private:
	//resample a sound to a target sample rate
	//this is run when sounds are loaded to match them to the soundcard sample rate
	void Resample(SoundData* target, int targetSampleRate);

	std::unordered_map<std::string, SoundData> sounds;
};