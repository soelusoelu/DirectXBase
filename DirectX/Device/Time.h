#pragma once

class Time {
public:
    Time(float sec);
    ~Time();
    void update();
    void reset();
    bool isTime() const;
    void setLimitTime(float sec);
    float currentTime() const;
    float limitTime() const;
    float rate() const;

public:
    static float deltaTime;

private:
    float mCurrentTime;
    float mLimitTime;
    bool mIsOverLimit;
};
