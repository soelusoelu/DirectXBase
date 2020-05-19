#include "BirdOrbit.h"
#include "ComponentManager.h"
#include "Sprite3D.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

BirdOrbit::BirdOrbit(std::shared_ptr<GameObject> owner) :
    Component(owner, "BirdOrbit"),
    mTimer(0.f),
    mSprite(nullptr) {
}

BirdOrbit::~BirdOrbit() = default;

void BirdOrbit::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();
    mSprite->transform()->rotate(Vector3::right, 90.f);
    mSprite->setActive(false);
}

void BirdOrbit::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "secondsAgo", &mTimer);
}

void BirdOrbit::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "SecondsAgo";
    info.second = mTimer;
    inspect->emplace_back(info);
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
