#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
using namespace FMOD;

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    std::vector<Sound*> sounds;

    //bool PlaySound(const std::string& filePath);
    bool LoadAudio(const char* filePath, bool loop);
    bool OnCreate();
    void OnDestroy();
    void Play(int trackIndex, float volume);
    void Update();

private:
   
    bool succeededOrWarn(const std::string& message, FMOD_RESULT result);
    FMOD::System* system;
    FMOD::ChannelGroup* channelGroup;
};
#endif // !AUDIO_SYSTEM_H