#include "SoundComponent.h"
#include "../Actor/Actor.h"
#include "../Device/AssetsManager.h"
#include "../Device/Renderer.h"
#include "../Device/Sound.h"

SoundComponent::SoundComponent(std::shared_ptr<Actor> owner) :
    Component(owner) {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::start() {
}

void SoundComponent::update() {
}

void SoundComponent::playOneShot(const char* fileName, float volumeScale) {
    auto sound = owner()->renderer()->getAssetsManager()->createSE(fileName);

    sound->setVolume(volumeScale);
    sound->play();
}
