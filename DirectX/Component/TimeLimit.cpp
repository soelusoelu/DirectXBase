#include "TimeLimit.h"
#include "ComponentManager.h"
#include "Text.h"
#include "Timer.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TimeLimit::TimeLimit() :
    Component(),
    mText(nullptr),
    mTimer(nullptr),
    mDecimalDigit(0) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mText = owner()->componentManager()->getComponent<Text>();
    mTimer = owner()->componentManager()->getComponent<Timer>();
}

void TimeLimit::update() {
    auto time = mTimer->currentDownTime();
    if (time < 0.f) {
        time = 0.f;
    }
    mText->setText(StringUtil::floatToString(time, mDecimalDigit));
}

void TimeLimit::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "decimalDigit", &mDecimalDigit);
}
