#include "AudioSystem.h"
#include <iostream>

AudioManager::AudioManager():system(nullptr),channelGroup(nullptr)
{
    OnCreate();
}

bool AudioManager::OnCreate()
{
    FMOD_RESULT result = System_Create(&system);
    if (result != FMOD_OK) {
        std::cerr << "FMOD system creation failed: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    result = system->init(32, FMOD_INIT_NORMAL, nullptr);  // Initialize FMOD with 32 channels
    if (result != FMOD_OK) {
        std::cerr << "FMOD system initialization failed: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    //LOAD ALL THE AUDIO HERE

    //0
    if (LoadAudio("ost/UI_sounds/Zipper_open.mp3", false) == false) {
        return false;
    }
    //1
    if (LoadAudio("ost/UI_sounds/Zipper_close.wav", false) == false) {
        return false;
    }
    //2
    if (LoadAudio("ost/UI_sounds/Nature_13_4.wav", false) == false) {
        return false;
    }
    //3
    if (LoadAudio("ost/UI_sounds/Abstract1.mp3", false) == false) {
        return false;
    }
    //4
    if (LoadAudio("ost/UI_sounds/Nature_13_6.wav", false) == false) {
        return false;
    }
    //5
    if (LoadAudio("ost/1.mp3", false) == false) {
        return false;
    }
    //6
    if (LoadAudio("ost/bedRoom.mp3", true) == false){ 
        return false;
    }
    //7
    if (LoadAudio("ost/Overworld.mp3", true) == false) {
        return false;
    }
    //8
    if (LoadAudio("ost/Underwater.mp3", true) == false) {
        return false;
    }
    //9
    if (LoadAudio("ost/UI_sounds/Retro8.wav", false) == false) {
        return false;
    }


    return true;
}

AudioManager::~AudioManager()
{
    channelGroup->release();

    for (auto& sound : sounds) {
        sound->release();
    }
    system->close();
    system->release();
}

bool AudioManager::LoadAudio(const char* filePath, bool loop)
{
    Sound* sound{};
    FMOD_MODE mode = FMOD_DEFAULT;
    if (loop) {
        mode |= FMOD_LOOP_NORMAL;  // Enable looping
    }
    FMOD_RESULT result = system->createStream(filePath, mode, nullptr, &sound);
    if (result != FMOD_OK) {
        std::cerr << "Failed to load sound: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    sounds.push_back(sound);
    return true;
 
}


void AudioManager::Play(int trackIndex, float volume) {
    if (trackIndex >= 0 && trackIndex < sounds.size()) {
        Channel* channel = nullptr;

        // Play the sound first
        FMOD_RESULT result = system->playSound(sounds[trackIndex], nullptr, false, &channel);
        if (result != FMOD_OK) {
            std::cerr << "Failed to play sound: " << FMOD_ErrorString(result) << std::endl;
            return;
        }

        // Check if channel is valid
        if (channel == nullptr) {
            std::cerr << "Channel is null after playSound" << std::endl;
            return;
        }

        // Set volume and check result
        result = channel->setVolume(volume);
        if (result != FMOD_OK) {
            std::cerr << "Failed to set volume: " << FMOD_ErrorString(result) << std::endl;
        }

        // Verify the volume was set correctly
        float actualVolume = -1.0f;
        result = channel->getVolume(&actualVolume);
        if (result != FMOD_OK) {
            std::cerr << "Failed to get volume: " << FMOD_ErrorString(result) << std::endl;
        }

        std::cout << "Audio: " << trackIndex << " Volume: " << actualVolume << std::endl;
    }
    else {
        std::cerr << "No such audio track: " << trackIndex << std::endl;
    }
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

void AudioManager::PlayBGmusic(int trackIndex, float volume)
{
    if (trackIndex >= 0 && trackIndex < sounds.size()) {
        if (trackIndex == currentTrackIndex) {
            return; // Don't restart same track
        }

        // Stop current music if playing
        if (bgMusicChannel) {
            bool isPlaying = false;
            bgMusicChannel->isPlaying(&isPlaying);
            if (isPlaying) {
                bgMusicChannel->stop();
            }
            bgMusicChannel = nullptr;
        }

        // Play new music
        FMOD_RESULT result = system->playSound(sounds[trackIndex], nullptr, false, &bgMusicChannel);
        if (!succeededOrWarn("Failed to play music", result)) return;

        result = bgMusicChannel->setVolume(volume);
        succeededOrWarn("Failed to set music volume", result);

        currentTrackIndex = trackIndex;

        std::cout << "Now playing music track: " << trackIndex << std::endl;
    }
    else {
        std::cerr << "Invalid music track index: " << trackIndex << std::endl;
    }
}
