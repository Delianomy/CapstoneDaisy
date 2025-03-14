#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H


#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool PlaySound(const std::string& filePath);
    void Update();

private:
    bool succeededOrWarn(const std::string& message, FMOD_RESULT result);
    FMOD::System* system;
    FMOD::ChannelGroup* channelGroup;
};
#endif // !AUDIO_SYSTEM_H