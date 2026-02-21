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
	std::vector<float> rawData;
};

class SoundLoader
{
	std::unordered_map<std::string, SoundData> sounds;

public:
	int LoadSound(const std::string soundName, const char* fileName) {
		SoundData soundData;
		
		std::unique_ptr<float> data(drwav_open_file_and_read_pcm_frames_f32(fileName,
			&soundData.channels,
			&soundData.sampleRate,
			&soundData.numFrames,
			NULL));

		if (!data)
		{
			std::cout << "Could not open audio file and read data!\n";
			return 1;
		}

		soundData.rawData = std::vector<float>(data.get(), data.get() + (soundData.numFrames * soundData.channels));

		sounds.insert({ soundName, soundData });
        return 0;
	}

	SoundData* GetSound(std::string key)
	{
		if (sounds.count(key) == 0)
		{
			std::cout << "Could not find sound with this key!\n";
			return nullptr;
		}

		return &sounds.at(key);
	}
};