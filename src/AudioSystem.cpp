#include "AudioSystem.h"

AudioSystem* AudioSystem::_instance = nullptr;

void AudioSystem::begin(uint8_t BCLK, uint8_t LRC, uint8_t DOUT) {
    _instance = this;

    Serial.println("Initialize I2S...");
    Audio::audio_info_callback = onAudioInfo;
    _audio.setPinout(BCLK, LRC, DOUT);
    _audio.setVolume(VOLUME_DEFAULT);

    Serial.println("Try to play stream");
    _audio.connecttohost(STREAM_URL);
}

void AudioSystem::loop() {
    _audio.loop();
}

void AudioSystem::volumeUp() {
    _audio.setVolume(min(VOLUME_MAX, _audio.getVolume() + 1));
}

void AudioSystem::volumeDown() {
    _audio.setVolume(max(0, _audio.getVolume() - 1));
}

int AudioSystem::getVolume() {
    return _audio.getVolume();
}

int AudioSystem::getVolumePercent() {
    return map(_audio.getVolume(), 0, VOLUME_MAX, 0, 100);
}

const String& AudioSystem::getStreamTitle() const {
    return _streamTitle;
}

bool AudioSystem::titleChanged() {
    bool changed = _titleChanged;
    _titleChanged = false;
    return changed;
}

void AudioSystem::onAudioInfo(Audio::msg_t m) {
    if (_instance) {
        _instance->handleAudioInfo(m);
    }
}

void AudioSystem::handleAudioInfo(Audio::msg_t m) {
    Serial.printf("%s: %s\n", m.s, m.msg);
    if (m.e == Audio::evt_streamtitle) {
        _streamTitle = String(m.msg);
        _titleChanged = true;
    } else if (m.e == Audio::evt_image) {
        for (int i = 0; i < m.vec.size(); i += 2) {
            Serial.printf("cover image: segment %02i, pos %07lu, len %05lu\n", i / 2, m.vec[i], m.vec[i + 1]);
        }
    }
}
