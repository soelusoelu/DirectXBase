#include "Time.h"

Time::Time(float sec) :
    mCurrentTime(0),
    mLimitTime(sec * 60.f + 0.5f),
    mIsOverLimit(false) {
}

Time::~Time() = default;

void Time::update() {
    mCurrentTime++;

    if (mCurrentTime >= mLimitTime) {
        mIsOverLimit = true;
    }
}

void Time::reset() {
    mCurrentTime = 0;
    mIsOverLimit = false;
}

bool Time::isTime() const {
    return mIsOverLimit;
}

void Time::setLimitTime(float sec) {
    mLimitTime = sec * 60.f + 0.5f;
}

int Time::getLimitTime() const {
    return mLimitTime;
}

int Time::currentTimer() const {
    return mCurrentTime / 60;
}

float Time::deltaTime = 0.01666f;
