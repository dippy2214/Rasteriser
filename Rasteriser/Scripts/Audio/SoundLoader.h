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
private:
	std::unordered_map<std::string, SoundData> sounds;

public:
	~SoundLoader()
	{
		//clean up sound rawData pointers created by drwav here
		for (auto i = sounds.begin(); i != sounds.end(); ++i)
		{
			drwav_free(i->second.rawData, NULL);
		}
	}

	int LoadSound(const std::string& soundName, const std::string& fileName) {
		SoundData soundData;
		
		float* data(drwav_open_file_and_read_pcm_frames_f32(fileName.c_str(),
			&soundData.channels,
			&soundData.sampleRate,
			&soundData.numFrames,
			NULL));

		if (!data)
		{
			std::cout << "Could not open audio file and read data!\n";
			return 1;
		}
		//technically speaking this copies memory
		//sub optimal but I ran into out of scope issues without doing this
		//and it is convinient for audio data to be structured as a vector
		//soundData.rawData = std::vector<float>(data, data + (soundData.numFrames * soundData.channels));

		soundData.rawData = data;

		Resample(&soundData);

		sounds.insert({ soundName, soundData });
		//drwav_free(data, NULL);
        return 0;
	}

	SoundData* GetSound(const std::string& key)
	{
		if (sounds.count(key) == 0)
		{
			std::cout << "Could not find sound with this key!\n";
			return nullptr;
		}

		return &sounds.at(key);
	}

	void Resample(SoundData* target)
	{

	}
};