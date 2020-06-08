#include "Time.h"
#include "../System/SystemInclude.h"

Time::Time(float sec) :
    mCurrentTime(0.f),
    mLimitTime(sec),
    mIsOverLimit(false) {
}

Time::~Time() = default;

void Time::update() {
    mCurrentTime += Time::deltaTime;

    if (mCurrentTime >= mLimitTime) {
        mIsOverLimit = true;
    }
}

void Time::reset() {
    mCurrentTime = 0.f;
    mIsOverLimit = false;
}

bool Time::isTime() const {
    return mIsOverLimit;
}

void Time::setLimitTime(float sec) {
    mLimitTime = sec;
}

void Time::setCurrentTime(float sec) {
    mCurrentTime = sec;
}

float Time::countUpTime() const {
    return mCurrentTime;
}

float Time::countDownTime() const {
    return mLimitTime - mCurrentTime;
}

float Time::limitTime() const {
    return mLimitTime;
}

float Time::rate() const {
    return mCurrentTime / mLimitTime;
}

unsigned long long Time::time() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return static_cast<unsigned long long>(time.QuadPart);
}

float Time::deltaTime = 0.01666f;
