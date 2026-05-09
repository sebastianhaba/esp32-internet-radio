#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <Arduino.h>
#include <Audio.h>

class AudioSystem {
public:
    enum class State { Idle, Connecting, Connected };

    void begin(uint8_t BCLK, uint8_t LRC, uint8_t DOUT);
    void loop();
    void play(const char* url);
    void stop();

    void volumeUp();
    void volumeDown();

    int getVolume();
    int getVolumePercent();
    const String& getStreamTitle() const;
    bool titleChanged();
    State state() const;
    uint16_t getVUlevel();

private:
    Audio _audio;
    String _streamTitle;
    bool _titleChanged = false;
    State _state = State::Idle;

    static AudioSystem* _instance;
    static void onAudioInfo(Audio::msg_t m);
    void handleAudioInfo(Audio::msg_t m);

    static constexpr int VOLUME_MAX     = 21;
    static constexpr int VOLUME_DEFAULT = 12;
};

#endif
