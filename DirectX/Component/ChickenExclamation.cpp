#include "ChickenExclamation.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

ChickenExclamation::ChickenExclamation() :
    Component(),
    mSprite(nullptr),
    mBurntSecondsAgo(0.f),
    mOffsetPosition(Vector3::zero),
    mMeshRadius(0.f) {
}

ChickenExclamation::~ChickenExclamation() = default;

void ChickenExclamation::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();

    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    mMeshRadius = mesh->getRadius();
}

void ChickenExclamation::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "burntSecondsAgo", &mBurntSecondsAgo);
    JsonHelper::getVector3(inObj, "offsetPosition", &mOffsetPosition);
}

void ChickenExclamation::setActive(bool value) {
    mSprite->setActive(value);
}

void ChickenExclamation::setPosition(const Vector3 & pos) {
    float offsetY = mMeshRadius * owner()->transform()->getScale().y;
    mSprite->transform()->setPosition(mOffsetPosition + pos + Vector3::up * offsetY);
}

float ChickenExclamation::getTime() const {
    return mBurntSecondsAgo;
}
