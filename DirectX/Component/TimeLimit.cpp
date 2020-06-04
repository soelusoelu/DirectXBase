#include "TimeLimit.h"
#include "ComponentManager.h"
#include "Text.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TimeLimit::TimeLimit() :
    Component(),
    mText(nullptr),
    mTimer(std::make_unique<Time>()),
    mDecimalDigit(0) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mText = owner()->componentManager()->getComponent<Text>();
}

void TimeLimit::update() {
    mTimer->update();
    auto time = mTimer->countDownTime();
    if (time < 0.f) {
        time = 0.f;
    }
    mText->setText(StringUtil::floatToString(time, mDecimalDigit));
}

void TimeLimit::loadProperties(const rapidjson::Value& inObj) {
    float timer = 0.f;
    if (JsonHelper::getFloat(inObj, "timer", &timer)) {
        mTimer->setLimitTime(timer);
    }
    JsonHelper::getInt(inObj, "decimalDigit", &mDecimalDigit);
}

bool TimeLimit::isTime() const {
    return mTimer->isTime();
}
