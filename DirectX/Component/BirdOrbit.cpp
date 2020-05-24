#include "BirdOrbit.h"
#include "ComponentManager.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

BirdOrbit::BirdOrbit() :
    Component(),
    mTimer(0.f),
    mSprite(nullptr) {
}

BirdOrbit::~BirdOrbit() = default;

void BirdOrbit::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();
}

void BirdOrbit::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "secondsAgo", &mTimer);
}

void BirdOrbit::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    inspect->emplace_back("SecondsAgo", mTimer);
}

void BirdOrbit::setActive(bool value) {
    mSprite->setActive(value);
}

void BirdOrbit::setPositionZ(float z) {
    mSprite->transform()->setPosition(Vector3(0.f, 0.f, z));
}

float BirdOrbit::getTime() const {
    return mTimer;
}
