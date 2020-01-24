#include "Time.h"

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

float Time::currentTime() const {
    return mCurrentTime;
}

float Time::limitTime() const {
    return mLimitTime;
}

float Time::rate() const {
    return mCurrentTime / mLimitTime;
}

float Time::deltaTime = 0.01666f;
