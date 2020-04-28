#include "BirdOrbit.h"
#include "ComponentManager.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

BirdOrbit::BirdOrbit(std::shared_ptr<GameObject> owner) :
    Component(owner, "BirdOrbit"),
    mSprite(nullptr) {
}

BirdOrbit::~BirdOrbit() = default;

void BirdOrbit::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();

    if (mSprite) {
        mSprite->transform()->rotate(Vector3::right, 90.f);
        mSprite->setActive(false);
    }
}

void BirdOrbit::setActive(bool value) {
    if (mSprite) {
        mSprite->setActive(value);
    }
}

void BirdOrbit::setPositionZ(float z) {
    if (mSprite) {
        mSprite->transform()->setPosition(Vector3(0.f, 0.f, z));
    }
}
