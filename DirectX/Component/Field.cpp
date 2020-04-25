#include "Field.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Math/Math.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Field::Field(std::shared_ptr<GameObject> owner) :
    Component(owner, "Field"),
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
    Component::loadProperties(inObj);

    JsonHelper::getBool(inObj, "isRoll", &mIsRoll);
    JsonHelper::getFloat(inObj, "rollSpeed", &mRollSpeed);
}

void Field::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "IsRoll";
    info.second = StringUtil::boolToString(mIsRoll);
    inspect->emplace_back(info);
    info.first = "RollSpeed";
    info.second = StringUtil::floatToString(mRollSpeed);
    inspect->emplace_back(info);
}
