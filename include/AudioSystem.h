#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <Arduino.h>
#include <Audio.h>

class AudioSystem {
public:
    void begin(uint8_t BCLK, uint8_t LRC, uint8_t DOUT);
    void loop();

    void volumeUp();
    void volumeDown();

    int getVolume();
    int getVolumePercent();
    const String& getStreamTitle() const;
    bool titleChanged();

private:
    Audio _audio;
    String _streamTitle;
    bool _titleChanged = false;

    static AudioSystem* _instance;
    static void onAudioInfo(Audio::msg_t m);
    void handleAudioInfo(Audio::msg_t m);

    static constexpr int VOLUME_MAX     = 21;
    static constexpr int VOLUME_DEFAULT = 12;
    static constexpr const char* STREAM_URL = "http://stream.nowyswiat.online/mp3";
};

#endif
