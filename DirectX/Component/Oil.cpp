#include "Oil.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Oil::Oil(std::shared_ptr<GameObject> owner) :
    Component(owner, "Oil", 130),
    mFlowSpeed(0.f),
    mFlowRangeX(0.f),
    mFlowRangeZFore(0.f),
    mFlowRangeZBack(0.f) {
}

Oil::~Oil() = default;

void Oil::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "flowSpeed", &mFlowSpeed);
    JsonHelper::getFloat(inObj, "flowRangeX", &mFlowRangeX);
    JsonHelper::getFloat(inObj, "flowRangeZFore", &mFlowRangeZFore);
    JsonHelper::getFloat(inObj, "flowRangeZBack", &mFlowRangeZBack);
}

void Oil::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "FlowSpeed";
    info.second = StringUtil::floatToString(mFlowSpeed);
    inspect->emplace_back(info);
    info.first = "flowRangeX";
    info.second = StringUtil::floatToString(mFlowRangeX);
    inspect->emplace_back(info);
    info.first = "flowRangeZFore";
    info.second = StringUtil::floatToString(mFlowRangeZFore);
    inspect->emplace_back(info);
    info.first = "flowRangeZBack";
    info.second = StringUtil::floatToString(mFlowRangeZBack);
    inspect->emplace_back(info);
}

void Oil::flow(const GameObjectPtr & gameObject) {
    auto t = gameObject->transform();
    auto pos = t->getPosition();
    auto moveDir = Vector3::zero;
    if (pos.z < -mFlowRangeZFore) {
        moveDir += Vector3::left;
    } else if (pos.z > mFlowRangeZBack) {
        moveDir += Vector3::right;
    }
    if (pos.x < -mFlowRangeX) {
        moveDir += Vector3::forward;
    } else if (pos.x > mFlowRangeX) {
        moveDir += Vector3::back;
    }
    t->translate(moveDir * Time::deltaTime * mFlowSpeed);
}
