#pragma once

#include <XAudio2.h>
#include <memory>
#include <string>

class Sound;
class Actor;

class SoundBase {
public:
    SoundBase();
    ~SoundBase();
    void load(const std::string& fileName, std::shared_ptr<Sound>* sound);
    void createSourceVoice(std::shared_ptr<Sound>* sound);

private:
    IXAudio2* mXAudio2;
    IXAudio2MasteringVoice* mMasteringVoice;
};

class Sound {
    friend class SoundBase;
public:
    Sound();
    ~Sound();
    void play(bool isLoop = false);
    void stop();
    void setVolume(float volume);

private:
    IXAudio2SourceVoice* mSourceVoice;
    BYTE* mWavBuffer; //波形データ(フォーマット等を含まない、純粋に波形データのみ)
    DWORD mWavSize; //波形データのサイズ
    WAVEFORMATEX* mWavFormat;
};
