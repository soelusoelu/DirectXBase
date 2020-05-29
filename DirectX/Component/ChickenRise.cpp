#include "ChickenRise.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"
#include "../Utility/LevelLoader.h"

ChickenRise::ChickenRise() :
    Component(),
    mUpSpeed(0.f),
    mCurrentRotation(0.f),
    mRotateSpeed(0.f),
    mRotateRange(0.f),
    mDefaultRotateRange(0.f),
    mRotateRateOfUp(0.f) {
}

ChickenRise::~ChickenRise() = default;

void ChickenRise::start() {
    mDefaultRotateRange = mRotateRange;
}

void ChickenRise::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "upSpeed", &mUpSpeed);
    JsonHelper::getFloat(inObj, "rotateSpeed", &mRotateSpeed);
    JsonHelper::getFloat(inObj, "rotateRateOfUp", &mRotateRateOfUp);
}

void ChickenRise::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    inspect->emplace_back("UpSpeed", mUpSpeed);
    inspect->emplace_back("RotateSpeed", mRotateSpeed);
    inspect->emplace_back("RotateRateOfUp", mRotateRateOfUp);
}

void ChickenRise::initialize() {
    mCurrentRotation = 0.f;
    mRotateRange = mDefaultRotateRange;
}

void ChickenRise::rise() {
    auto t = owner()->transform();
    t->translate(Vector3::up * mUpSpeed * Time::deltaTime);

    mCurrentRotation += mRotateSpeed * Time::deltaTime;
    mRotateRange += mRotateRateOfUp * Time::deltaTime;
    t->translate(Vector3(Math::cos(mCurrentRotation), 0.f, Math::sin(mCurrentRotation)) * mRotateRange);
}

bool ChickenRise::isEnd() {
    auto posY = owner()->transform()->getPosition().y;
    return (posY > 6.f);
}
