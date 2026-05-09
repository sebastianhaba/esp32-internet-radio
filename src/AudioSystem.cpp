#include "AudioSystem.h"

AudioSystem* AudioSystem::_instance = nullptr;

void AudioSystem::begin(uint8_t BCLK, uint8_t LRC, uint8_t DOUT) {
    _instance = this;

    Serial.println("Initialize I2S...");
    Audio::audio_info_callback = onAudioInfo;
    _audio.setPinout(BCLK, LRC, DOUT);
    _audio.setVolume(VOLUME_DEFAULT);
}

void AudioSystem::loop() {
    _audio.loop();
    if (_state == State::Connecting && _audio.isRunning()) {
        _state = State::Connected;
    }
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

void AudioSystem::play(const char* url) {
    _streamTitle = "";
    _titleChanged = true;
    _state = State::Connecting;
    _audio.connecttohost(url);
}

void AudioSystem::stop() {
    _audio.stopSong();
    _state = State::Idle;
}

AudioSystem::State AudioSystem::state() const {
    return _state;
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
        _state = State::Connected;
    } else if (m.e == Audio::evt_name) {
        _state = State::Connected;
    } else if (m.e == Audio::evt_image) {
        for (int i = 0; i < m.vec.size(); i += 2) {
            Serial.printf("cover image: segment %02i, pos %07lu, len %05lu\n", i / 2, m.vec[i], m.vec[i + 1]);
        }
    }
}
