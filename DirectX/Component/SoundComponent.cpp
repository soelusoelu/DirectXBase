#include "SoundComponent.h"
#include "../Device/AssetsManager.h"
#include "../Device/Sound.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

SoundComponent::SoundComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "SoundComponent"),
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
    Component::loadProperties(inObj);

    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getFloat(inObj, "volume", &mVolume);
    JsonHelper::getBool(inObj, "isFirstPlay", &mIsFirstPlay);
}

void SoundComponent::drawDebugInfo(DebugInfoList* inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "FileName";
    info.second = mFileName;
    inspect->emplace_back(info);
    info.first = "Volume";
    info.second = StringUtil::floatToString(mVolume);
    inspect->emplace_back(info);
    info.first = "IsFirstPlay";
    info.second = StringUtil::boolToString(mIsFirstPlay);
    inspect->emplace_back(info);
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
