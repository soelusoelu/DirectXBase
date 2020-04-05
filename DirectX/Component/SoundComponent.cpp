#include "SoundComponent.h"
#include "../Device/AssetsManager.h"
#include "../Device/Sound.h"

SoundComponent::SoundComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "SoundComponent") {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::playOneShot(const std::string& fileName, float volumeScale) {
    auto sound = Singleton<AssetsManager>::instance().createSE(fileName);

    sound->setVolume(volumeScale);
    sound->play();
}
