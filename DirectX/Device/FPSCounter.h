#pragma once

#include "../System/DirectXIncLib.h"
#include <rapidjson/document.h>
#include <memory>

class DrawString;
class Renderer;
class Time;

class FPSCounter {
public:
    FPSCounter(std::shared_ptr<Renderer> renderer);
    ~FPSCounter();
    void loadProperties(const rapidjson::Value& inObj);
    void fixedFrame();
    void setFixedFrame(float fixedFrame);

private:
    void drawFPS(float time);

private:
    std::shared_ptr<DrawString> mDrawString;
    std::unique_ptr<Time> mDrawUpdateTimer;
    float mFixedFrame;
    float mCurrentFPS;
    LARGE_INTEGER mFrequency;
    unsigned long long mPreviousTime;
};
