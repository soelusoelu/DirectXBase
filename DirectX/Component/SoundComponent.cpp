#include "SoundComponent.h"
#include "../Device/AssetsManager.h"
#include "../Device/Sound.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

SoundComponent::SoundComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "SoundComponent"),
    mFileName(""),
    mVolume(1.f) {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getFloat(inObj, "volume", &mVolume);
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
}

void SoundComponent::play() {
    playOneShot(mFileName, mVolume);
}

void SoundComponent::playOneShot(const std::string & fileName, float volumeScale) {
    auto sound = Singleton<AssetsManager>::instance().createSE(fileName);

    sound->setVolume(volumeScale);
    sound->play();
}
