#include "Field.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"
#include "../Utility/LevelLoader.h"

Field::Field() :
    Component(),
    mIsRoll(false),
    mRollSpeed(0.f) {
}

Field::~Field() = default;

void Field::start() {
    auto t = owner()->transform();
    t->rotate(Vector3::left, 90.f);
}

void Field::update() {
    if (mIsRoll) {
        owner()->transform()->rotate(Vector3::up, mRollSpeed);
    }
}

void Field::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getBool(inObj, "isRoll", &mIsRoll);
    JsonHelper::getFloat(inObj, "rollSpeed", &mRollSpeed);
}

void Field::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("IsRoll", mIsRoll);
    inspect->emplace_back("RollSpeed", mRollSpeed);
}
