#include "AudioSystem.h"
#include <iostream>

AudioManager::AudioManager():system(nullptr),channelGroup(nullptr)
{
    FMOD_RESULT result;
    // Create the main system object.
    result = FMOD::System_Create(&system);
    succeededOrWarn("FMOD: Failed to create system object", result);

    // Initialize FMOD.
    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    succeededOrWarn("FMOD: Failed to initialise system object", result);

    // Create the channel group.
    FMOD::ChannelGroup* channelGroup = nullptr;
    result = system->createChannelGroup("inGameSoundEffects", &channelGroup);
    succeededOrWarn("FMOD: Failed to create in-game sound effects channel group", result);
        
}

AudioManager::~AudioManager()
{
    channelGroup->release();
    system->release();
}

bool AudioManager::PlaySound(const std::string& filePath)
{
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
    if (!succeededOrWarn("FMOD: Failed to create sound", result))
        return false;

    FMOD::Channel* channel = nullptr;
    result = system->playSound(sound, nullptr, false, &channel);
    if (!succeededOrWarn("FMOD: Failed to play sound", result)) {
        sound->release(); // Cleanup
        return false;
    }

    if (channelGroup) {
        result = channel->setChannelGroup(channelGroup);
        if (!succeededOrWarn("FMOD: Failed to set channel group", result))
            return false;
    }

    return true;
}

bool AudioManager::succeededOrWarn(const std::string& message, FMOD_RESULT result)
{
    if (result != FMOD_OK) {
        std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}


void AudioManager::Update() {
    system->update();
}