//...and we're using the sokol_audio single-header library to stream audio data
//to our soundcard.
#define SOKOL_IMPL
#include "sokol_audio.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <atomic>

#include "LockFreeQueue.h"


static const int NUMVOICES = 8;

//------------------------------------------------------------------------------
///	This struct will get passed in to audioCallback().
/*!
	Use it to store any data that you want to remain persistent between calls to
	audioCallback().
 */
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
	bool LoadSound(const std::string soundName, const char* fileName) {
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

struct Voice
{
	std::atomic<bool> isActive = false;
	std::atomic<bool> isLooping = false;
	std::atomic<SoundData*> soundData = nullptr;
	std::atomic<int> writtenFrameCount = 0;
};

struct AudioData
{
	Voice voices[NUMVOICES];
};

void RenderVoiceToBuffer(float* buffer, Voice* voice, int frame)
{
	if (voice->isActive)
	{
		if (voice->soundData.load()->channels == 2) //handle stereo files
		{
			buffer[(2 * frame)] += (voice->soundData.load()->rawData[voice->writtenFrameCount]);
			buffer[(2 * frame) + 1] += (voice->soundData.load()->rawData[voice->writtenFrameCount + 1]);
			voice->writtenFrameCount++;
		}
		else //handle mono files
		{
			buffer[(2 * frame)] += (voice->soundData.load()->rawData[voice->writtenFrameCount]);
			buffer[(2 * frame) + 1] += (voice->soundData.load()->rawData[voice->writtenFrameCount]);
		}
		voice->writtenFrameCount++;
		if (voice->writtenFrameCount >= voice->soundData.load()->numFrames)
		{
			if (!voice->isLooping)
			{
				voice->isActive = false;
			}
			voice->writtenFrameCount = 0;
		}
	}
}


//------------------------------------------------------------------------------
///	Our audio callback. All audio processing happens here.
/*!
	When the function returns our audio data will get passed to the soundcard.
 */
void audioCallback(float *buffer,	//A buffer of float audio samples for us to fill.
				   int numFrames,	//The number of frames in this buffer (you can think of a frame as a multi-channel sample; mono = 1 sample per frame; stereo = 2 samples per frame, etc.).
				   int numChannels,	//The number of channels this buffer represents (1: mono).
				   void *userData)	//Our AudioData struct.
{
	//Get our audio data from the passed-in userData variable.
	AudioData *data((AudioData *)userData);
	//Get the samplerate our soundcard is running at.
	const float samplerate((float)saudio_sample_rate());


	//Just in case...
	if(data)
	{
		for(int i=0;i<numFrames;++i)
		{
			buffer[(2*i)] = 0.0f;
			buffer[(2*i) + 1] = 0.0f;
			
			for (int v = 0; v < NUMVOICES; ++v)
			{
				RenderVoiceToBuffer(buffer, &data->voices[v], i);
			}
			
		}
	}
}

//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	//We store any data we might use in our audio callback in this struct.
	AudioData audioData;

	//--------------------------------------------------------------------------
	// Load sound file data here.
	//--------------------------------------------------------------------------
	SoundLoader loader;
	loader.LoadSound("loop", "Loop.wav");
	loader.LoadSound("stereo", "Stereo.wav");
	
	//Create a sokol_audio audio descriptor, zero its members.
	saudio_desc audioDescriptor = {};

	//Request stereo audio.
	audioDescriptor.num_channels = 2;
	//Pass in our audio callback.
	audioDescriptor.stream_userdata_cb = audioCallback;
	//Pass in any user data (stored in the AudioData struct) to the audio callback.
	audioDescriptor.user_data = (void *)&audioData;

	audioData.voices[0].soundData = loader.GetSound("stereo");
	if (audioData.voices[0].soundData == nullptr)
	{
		std::cout << "Failed to grab sound from loader!\n";
		return 1;
	}
	audioData.voices[0].isLooping = true;

	for (int i = 1; i < NUMVOICES; ++i)
	{
		audioData.voices[i].soundData = loader.GetSound("loop");
		if (audioData.voices[i].soundData == nullptr)
		{
			std::cout << "Failed to grab sound from loader!\n";
			return 1;
		}

		audioData.voices[i].isLooping = true;
	}
	
	//Initialise sokol_audio, and start the soundcard processing audio.
	saudio_setup(&audioDescriptor);

	//Check that we were able to initialise sokol_audio.
	if(!saudio_isvalid())
	{
		std::cout << "Could not initialise sokol_audio. Exiting." << std::endl;

		return 1;
	}
	else
		std::cout << "(type [x] to quit)" << std::endl;

	// <at this point, audio is now being processed>

	//We use cin.get() as a cheap way of blocking execution on the main thread
	//until the user decides to quit. We can also use it as a way to introduce
	//user input (e.g. trigger a sound when a particular key is entered)
	//(note that audio processing happens in a separate audio thread)


	int input = std::cin.get();
	while(input != 'x')
	{
		switch (input)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			audioData.voices[input - 49].isActive = !audioData.voices[input-49].isActive;
			break;
		default:
			break;
		}

		input = std::cin.get();
	}

	//Close the soundcard, clean up after ourselves.
	saudio_shutdown();

	return 0;
}
