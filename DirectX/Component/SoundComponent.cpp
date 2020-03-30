#include "SoundComponent.h"
#include "../Device/AssetsManager.h"
#include "../Device/Renderer.h"
#include "../Device/Sound.h"
#include "../GameObject/GameObject.h"

SoundComponent::SoundComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "SoundComponent") {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::playOneShot(const char* fileName, float volumeScale) {
    auto sound = owner()->renderer()->getAssetsManager()->createSE(fileName);

    sound->setVolume(volumeScale);
    sound->play();
}
