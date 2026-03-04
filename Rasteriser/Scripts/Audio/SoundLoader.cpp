#include "SoundLoader.h"
#include "sokol_audio.h"

SoundLoader::~SoundLoader()
{
    //clean up sound rawData pointers created by drwav here
    for (auto i = sounds.begin(); i != sounds.end(); ++i)
    {
        drwav_free(i->second.rawData, NULL);
    }
}

int SoundLoader::LoadSound(const std::string& soundName, const std::string& fileName) {
    
    SoundData soundData;
    
    //drwav allocates internally with malloc
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

    //avoid reallocating memory and let drwav handle memory allocation and deallocation
    soundData.rawData = data;

    //resample audio on load to audio card sample rate
    Resample(&soundData, saudio_sample_rate());

    sounds.insert({ soundName, soundData });
    return 0;
}

SoundData* SoundLoader::GetSound(const std::string& key)
{
    if (sounds.count(key) == 0)
    {
        std::cout << "Could not find sound with this key!\n";
        return nullptr;
    }

    return &sounds.at(key);
}

void SoundLoader::Resample(SoundData* target, int targetSampleRate)
{
    if (target->sampleRate == targetSampleRate) return;

    //lots of static casting around here to prevent rounding errors
    float ratio = static_cast<float>(targetSampleRate) / static_cast<float>(target->sampleRate);
    int outputFrames = static_cast<int>(target->numFrames * ratio);

    //allocate memory here with malloc so that drwav_free can clean up in destructors later
    //to avoid memory leaks
    float* outputData = static_cast<float*>(malloc(outputFrames * sizeof(float) * target->channels));

    //I am informed that making this a float can cause some weird rounding errors
    //this results in phase drift in longer files - so double it is
    double phase = 0.0f;
    double phaseIncrement = static_cast<double>(target->sampleRate) / static_cast<double>(targetSampleRate);
    for (int f = 0; f < outputFrames; ++f)
    {
        int i = static_cast<int>(phase);
        double fraction = phase - i;

        //so many indexing issues came out of this area so beware changing
        int outputIndex = f * target->channels;
        int inputIndex = i * target->channels;
        
        for (int c = 0; c < target->channels; ++c)
        {
            //dont cancel if we reach the end of lerpable territory
            //fill buffer to avoid garbage data by repeating last sample
            if (i + 1 >= target->numFrames)
            {
                outputData[outputIndex+c] = target->rawData[((target->numFrames - 1) * target->channels) + c];
            }
            else
            {
                float sample1 = target->rawData[inputIndex+c];
                float sample2 = target->rawData[(inputIndex + 1)+c];
                outputData[outputIndex+c] = sample1 + (sample2 - sample1) * fraction;
            }
        }
        phase += phaseIncrement;
    }
    
    //clean up memory allocated by drwav to replace with new buffer
    drwav_free(target->rawData, NULL);
    target->sampleRate = targetSampleRate;
    target->numFrames = outputFrames;
    target->rawData = outputData;
}