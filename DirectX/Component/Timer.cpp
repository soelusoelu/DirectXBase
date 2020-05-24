#include "Timer.h"
#include "../Device/Time.h"
#include "../Utility/LevelLoader.h"

Timer::Timer() :
    Component(50),
    mCurrentTime(0.f),
    mLimitTime(0.f),
    mIsOverLimit(false) {
}

Timer::~Timer() = default;

void Timer::update() {
    mCurrentTime += Time::deltaTime;

    if (mCurrentTime >= mLimitTime) {
        mIsOverLimit = true;
    }
}

void Timer::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "limit", &mLimitTime);
}

void Timer::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setFloat(alloc, inObj, "limit", mLimitTime);
}

void Timer::drawDebugInfo(DebugInfoList* inspect) const {
    Component::drawDebugInfo(inspect);

    inspect->emplace_back("CurrentTime", mCurrentTime);
    inspect->emplace_back("LimitTime", mLimitTime);
    inspect->emplace_back("IsOverLimit", mIsOverLimit);
}

void Timer::reset() {
    mCurrentTime = 0.f;
    mIsOverLimit = false;
}

bool Timer::isTime() const {
    return mIsOverLimit;
}

void Timer::setLimitTime(float sec) {
    mLimitTime = sec;
}

float Timer::currentUpTime() const {
    return mCurrentTime;
}

float Timer::currentDownTime() const {
    return mLimitTime - mCurrentTime;
}

float Timer::limitTime() const {
    return mLimitTime;
}

float Timer::rate() const {
    return mCurrentTime / mLimitTime;
}
