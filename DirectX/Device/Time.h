#pragma once

class Time {
public:
    Time(float sec);
    ~Time();
    void update();
    void reset();
    bool isTime() const;
    void setLimitTime(float sec);
    int getLimitTime() const;
    int currentTimer() const;

public:
    static float deltaTime;

private:
    int mCurrentTime;
    int mLimitTime;
    bool mIsOverLimit;
};
