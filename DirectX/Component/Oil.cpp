#include "Oil.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Oil::Oil(std::shared_ptr<GameObject> owner) :
    Component(owner, "Oil", 130),
    mFlowSpeed(0.f) {
}

Oil::~Oil() = default;

void Oil::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "flowSpeed", &mFlowSpeed);
}

void Oil::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "FlowSpeed";
    info.second = StringUtil::floatToString(mFlowSpeed);
    inspect->emplace_back(info);
}

void Oil::flow(const GameObjectPtr & gameObject) {
    auto t = gameObject->transform();
    auto pos = t->getPosition();
    if (pos.z < -3.f) {
        t->translate(Vector3::left * Time::deltaTime * mFlowSpeed);
    } else if (pos.z > 3.f) {
        t->translate(Vector3::right * Time::deltaTime * mFlowSpeed);
    }
    if (pos.x < -3.f) {
        t->translate(Vector3::forward * Time::deltaTime * mFlowSpeed);
    } else if (pos.x > 3.f) {
        t->translate(Vector3::back * Time::deltaTime * mFlowSpeed);
    }
}
