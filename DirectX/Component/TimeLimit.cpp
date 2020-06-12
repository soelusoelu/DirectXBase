#include "TimeLimit.h"
#include "ComponentManager.h"
#include "TextFloat.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"

TimeLimit::TimeLimit() :
    Component(),
    mText(nullptr),
    mTimer(std::make_unique<Time>()) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mText = owner()->componentManager()->getComponent<TextFloat>();
}

void TimeLimit::update() {
    mTimer->update();
    auto time = mTimer->countDownTime();
    if (time < 0.f) {
        time = 0.f;
    }
    mText->setNumber(time);
}

void TimeLimit::loadProperties(const rapidjson::Value& inObj) {
    float timer = 0.f;
    if (JsonHelper::getFloat(inObj, "timer", &timer)) {
        mTimer->setLimitTime(timer);
    }
}

bool TimeLimit::isTime() const {
    return mTimer->isTime();
}
