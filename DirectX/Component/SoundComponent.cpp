#include "SoundComponent.h"
#include "../Device/AssetsManager.h"
#include "../Device/Sound.h"
#include "../Utility/LevelLoader.h"

SoundComponent::SoundComponent() :
    Component(),
    mSound(nullptr),
    mFileName(""),
    mVolume(1.f),
    mIsFirstPlay(false) {
}

SoundComponent::~SoundComponent() {
    if (mSound) {
        mSound->stop();
    }
}

void SoundComponent::start() {
    if (mIsFirstPlay) {
        playBGM();
    }
}

void SoundComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getFloat(inObj, "volume", &mVolume);
    JsonHelper::getBool(inObj, "isFirstPlay", &mIsFirstPlay);
}

void SoundComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Volume", mVolume);
    inspect->emplace_back("IsFirstPlay", mIsFirstPlay);
}

void SoundComponent::playBGM() {
    playBGM(mFileName, mVolume);
}

void SoundComponent::playBGM(const std::string& fileName, float volumeScale) {
    mSound = Singleton<AssetsManager>::instance().createSound(fileName);

    mSound->setVolume(volumeScale);
    mSound->play(true);
}

void SoundComponent::playSE() {
    playSE(mFileName, mVolume);
}

void SoundComponent::playSE(const std::string& fileName, float volumeScale) {
    auto sound = Singleton<AssetsManager>::instance().createSound(fileName);

    sound->setVolume(volumeScale);
    sound->play();
}
