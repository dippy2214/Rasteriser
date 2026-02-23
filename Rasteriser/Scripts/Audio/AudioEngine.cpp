//...and we're using the sokol_audio single-header library to stream audio data
//to our soundcard.
#define SOKOL_IMPL
#define DR_WAV_IMPLEMENTATION

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "AudioEngine.h"
#include "LockFreeQueue.h"

//------------------------------------------------------------------------------
///	This struct will get passed in to audioCallback().
/*!
	Use it to store any data that you want to remain persistent between calls to
	audioCallback().
 */

AudioEngine::~AudioEngine()
{
	//Close the soundcard, clean up after ourselves.
	saudio_shutdown();
}

void AudioEngine::RenderVoiceToBuffer(float* buffer, Voice* voice, int frame, Transform* listener)
{
	if (voice->isActive)
	{
		float leftSample = 0;
		float rightSample = 0;
		if (voice->soundData->channels == 2) //handle stereo files
		{
			leftSample += (voice->soundData->rawData[voice->writtenFrameCount]);
			rightSample += (voice->soundData->rawData[voice->writtenFrameCount + 1]);
			voice->writtenFrameCount++;
		}
		else //handle mono files
		{
			leftSample += (voice->soundData->rawData[voice->writtenFrameCount]);
			rightSample += (voice->soundData->rawData[voice->writtenFrameCount]);
		}

		if (voice->audioSource != nullptr && listener != nullptr)
		{
			AudioSource* audioSource = voice->audioSource;
			float distance = (audioSource->position - listener->position).Length();
			float gain = 0;
			
			if (distance < audioSource->minDistance)
			{
				distance = audioSource->minDistance;
			}
			if (distance <= audioSource->maxDistance)
			{
				gain = audioSource->minDistance / (audioSource->minDistance + audioSource->rolloff * (distance - audioSource->minDistance));
			}

			float3 toSource = (audioSource->position - listener->position).Normalised();
			float pan = toSource.Dot(listener->basisVectors.ihat);
			pan = std::clamp(pan, -1.0f, 1.0f);

			float angle = (pan + 1.0f) * 0.25f * M_PI;
			float leftGain = std::cos(pan) * gain;
			float rightGain = std::sin(pan) * gain;

			leftSample *= leftGain;
			rightSample *= rightGain;
		}

		buffer[(2 * frame)] = leftSample;
		buffer[(2 * frame) + 1] = rightSample;

		voice->writtenFrameCount++;
		if (voice->writtenFrameCount >= voice->soundData->numFrames)
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
void AudioEngine::audioCallback(float *buffer,	//A buffer of float audio samples for us to fill.
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
				RenderVoiceToBuffer(buffer, &data->voices[v], i, data->listenerTransform);
			}
			
		}
	}
}

//------------------------------------------------------------------------------
int AudioEngine::AudioInit(InputManager* inputMan)
{
	inputManager = inputMan;

	//We store any data we might use in our audio callback in this struct.

	//--------------------------------------------------------------------------
	// Load sound file data here.
	//--------------------------------------------------------------------------
	loader.LoadSound("loop", "Assets/Audio/Loop.wav");
	loader.LoadSound("stereo", "Assets/Audio/Stereo.wav");
	
	//Create a sokol_audio audio descriptor, zero its members.
	saudio_desc audioDescriptor = {};

	//Request stereo audio.
	audioDescriptor.num_channels = 2;
	//Pass in our audio callback.
	audioDescriptor.stream_userdata_cb = AudioEngine::audioCallback;
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

	// <at this point, audio is now being processed>

	return 0;
}

void AudioEngine::AudioInputs()
{
	if (inputManager->IsKeyPressed('1')) { audioData.voices[0].isActive = !audioData.voices[0].isActive; }
	if (inputManager->IsKeyPressed('2')) { audioData.voices[1].isActive = !audioData.voices[1].isActive; }
	if (inputManager->IsKeyPressed('3')) { audioData.voices[2].isActive = !audioData.voices[2].isActive; }
	if (inputManager->IsKeyPressed('4')) { audioData.voices[3].isActive = !audioData.voices[3].isActive; }
	if (inputManager->IsKeyPressed('5')) { audioData.voices[4].isActive = !audioData.voices[4].isActive; }
	if (inputManager->IsKeyPressed('6')) { audioData.voices[5].isActive = !audioData.voices[5].isActive; }
	if (inputManager->IsKeyPressed('7')) { audioData.voices[6].isActive = !audioData.voices[6].isActive; }
	if (inputManager->IsKeyPressed('8')) { audioData.voices[7].isActive = !audioData.voices[7].isActive; }
}

void AudioEngine::SetActiveListener(Transform* trans)
{
	audioData.listenerTransform = trans;
}

void AudioEngine::AddSourceToVoice(AudioSource* audioSource, int voiceIndex)
{
	audioData.voices[voiceIndex].audioSource = audioSource;
}