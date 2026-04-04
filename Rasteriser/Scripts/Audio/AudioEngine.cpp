//...and we're using the sokol_audio single-header library to stream audio data
//to our soundcard.
#define SOKOL_IMPL
#define DR_WAV_IMPLEMENTATION

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "AudioEngine.h"

//------------------------------------------------------------------------------
///	This struct will get passed in to audioCallback().
/*!
	Use it to store any data that you want to remain persistent between calls to
	audioCallback().
 */

AudioEngine::AudioEngine()
{
	// IMPORTANT - initialise sokol audio here as other parts of the system
	// 			   depend on it for information like sample rate of sound 
	//			   card and sound buffer size
	
	saudio_desc audioDescriptor = {};
	audioDescriptor.num_channels = OUTPUTCHANNELS; //playback is stereo
	audioDescriptor.stream_userdata_cb = AudioEngine::audioCallback;
	audioDescriptor.user_data = (void *)&audioData;
	
	//Initialise sokol_audio, and start the soundcard processing audio.
	saudio_setup(&audioDescriptor);

	//Check that we were able to initialise sokol_audio.
	if(!saudio_isvalid())
	{
		std::cout << "Could not initialise sokol_audio. Exiting." << std::endl;
	}
	mixerManager.SetMaxSamples(saudio_buffer_frames()*2);
}

AudioEngine::~AudioEngine()
{
	//Close the soundcard, clean up after ourselves.
	saudio_shutdown();
}

void AudioEngine::ApplySpatialEffectsToStereoSamples(AudioSource* audioSource, Transform* listener, float* leftSample, float*  rightSample)
{
	//distance based attenuation
	float distance = (audioSource->position - listener->position).Length();
	float gain = 0;
	
	distance = std::max(distance, audioSource->minDistance);
	if (distance <= audioSource->maxDistance)
	{
		gain = audioSource->minDistance / (audioSource->minDistance + audioSource->rolloff * (distance - audioSource->minDistance));
	}

	//directional panning (interaural level difference)
	float3 toSource = (audioSource->position - listener->position).Normalised();
	float pan = toSource.Dot(listener->basisVectors.ihat);
	pan = std::clamp(pan, -1.0f, 1.0f);

	float angle = (pan + 1.0f) * 0.25f * M_PI;

	//delayed playback between ears (interaural time difference)
	const float headRadius = 0.09f; //meters
    const float speedOfSound = 343.f; //meters per second
    const float maxInterauralTimeDifference = headRadius/speedOfSound; //seconds

	float sampleRate = saudio_sample_rate();
	float delaySamples = pan * sampleRate * maxInterauralTimeDifference;

	//use abs to account for negative or positive direction from pan
	int intDelay = static_cast<int>(std::abs(delaySamples));

	if (delaySamples > 0) { 
		audioSource->leftITDBuffer.write(*leftSample); 
		*leftSample = audioSource->leftITDBuffer.readRelative(intDelay);
		
		audioSource->rightITDBuffer.write(*rightSample);
	}
	else {
		audioSource->rightITDBuffer.write(*rightSample); 
		*rightSample = audioSource->rightITDBuffer.readRelative(intDelay);
		
		audioSource->leftITDBuffer.write(*leftSample);
	}

	//distance based attenuation gain applied here with interaural level difference
	//after sample changes from interaural time difference
	float leftGain = std::sin(angle) * gain;
	float rightGain = std::cos(angle) * gain;

	*leftSample *= leftGain;
	*rightSample *= rightGain;
}

void AudioEngine::RenderVoiceToBuffer(std::vector<float>* buffer, Voice* voice, int numFrames, Transform* listener)
{
	//we already have checked at this point that the audiosource exists
	if (!voice->audioSource->isActive) return;

	for(int i=0;i<numFrames;++i)
	{	
		float leftSample = 0;
		float rightSample = 0;

		int frameIndex = voice->writtenFrameCount;

		//internally audio is processed as stereo and output as stereo
		if (voice->audioSource->soundData->channels == 2) //handle stereo files
		{
			int sampleIndex = frameIndex * 2;
			leftSample += (voice->audioSource->soundData->rawData[sampleIndex]);
			rightSample += (voice->audioSource->soundData->rawData[sampleIndex + 1]);
		}
		else //handle mono files
		{
			leftSample += (voice->audioSource->soundData->rawData[voice->writtenFrameCount]);
			rightSample += (voice->audioSource->soundData->rawData[voice->writtenFrameCount]);
		}

		//check for spatialisation and listener and apply spatial effects if found
		if (voice->audioSource->is3D && listener != nullptr)
		{
			ApplySpatialEffectsToStereoSamples(voice->audioSource, listener, &leftSample, &rightSample);
		}

		(*buffer)[2 * i] += leftSample;
		(*buffer)[(2 * i) + 1] += rightSample;

		voice->writtenFrameCount++;
		if (voice->writtenFrameCount >= voice->audioSource->soundData->numFrames)
		{
			if (!voice->audioSource->isLooping)
			{
				voice->audioSource->isActive = false;
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
	if(!data) return;

	data->mixerManager->ZeroAllBuffers();

	//render all voices to their relevant mixer's internal buffer
	for (int v = 0; v < NUMVOICES; ++v)
	{
		if (!data->voices[v].audioSource) { continue; }
		RenderVoiceToBuffer(&data->voices[v].audioSource->mixer->buffer, &data->voices[v], numFrames, data->listenerTransform);
	}

	//make sure to zero buffer
	std::fill(buffer, buffer + numFrames * numChannels, 0.0f);

	//apply mixer effects and add them to final output buffer
	data->mixerManager->ApplyAllMixerEffects();
	data->mixerManager->AddAllMixersIntoBuffer(buffer, numFrames);
}

//------------------------------------------------------------------------------

void AudioEngine::SetActiveListener(Transform* trans)
{
	audioData.listenerTransform = trans;
}

void AudioEngine::AddSourceToVoice(AudioSource* audioSource, int voiceIndex)
{
	audioData.voices[voiceIndex].audioSource = audioSource;
}

AudioSource* AudioEngine::CreateAudioSource()
{
	if (currentVoiceIndex >= NUMVOICES) { return nullptr; }
	AudioSource* output = new AudioSource;
	output->mixer = mixerManager.GetMixer("default");
	AddSourceToVoice(output, currentVoiceIndex);
	currentVoiceIndex++;
	return output;
}

Mixer* AudioEngine::AddMixer(const std::string& name)
{
	return mixerManager.AddMixer(name);
}

Mixer* AudioEngine::GetMixer(const std::string& name)
{
	return mixerManager.GetMixer(name);
}

int AudioEngine::LoadSound(const std::string& soundName, const std::string& fileName)
{
	return loader.LoadSound(soundName, fileName);
}

SoundData* AudioEngine::GetSound(const std::string& soundName)
{
	return loader.GetSound(soundName);
}