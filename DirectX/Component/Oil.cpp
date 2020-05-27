#include "Oil.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

Oil::Oil() :
    Component(130),
    mFlowSpeed(0.f),
    mFlowRangeX(0.f),
    mFlowRangeZFore(0.f),
    mFlowRangeZBack(0.f) {
}

Oil::~Oil() = default;

void Oil::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "flowSpeed", &mFlowSpeed);
    JsonHelper::getFloat(inObj, "flowRangeX", &mFlowRangeX);
    JsonHelper::getFloat(inObj, "flowRangeZFore", &mFlowRangeZFore);
    JsonHelper::getFloat(inObj, "flowRangeZBack", &mFlowRangeZBack);
}

void Oil::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    inspect->emplace_back("FlowSpeed", mFlowSpeed);
    inspect->emplace_back("flowRangeX", mFlowRangeX);
    inspect->emplace_back("flowRangeZFore", mFlowRangeZFore);
    inspect->emplace_back("flowRangeZBack", mFlowRangeZBack);
}

void Oil::flow(const GameObjectPtr& gameObject) {
    const auto& t = gameObject->transform();
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
