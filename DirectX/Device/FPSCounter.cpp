#include "FPSCounter.h"
#include "DrawString.h"
#include "Log.h"
#include "Renderer.h"
#include "Time.h"
#include "../Utility/Debug.h"

FPSCounter::FPSCounter(std::shared_ptr<Renderer> renderer, float fixedFrame) :
    mDrawString(renderer->getDrawString()),
    mDrawUpdateTimer(std::make_unique<Time>(0.5f)),
    mFixedFrame(fixedFrame),
    mCurrentFPS(fixedFrame),
    mFrequency(),
    mPreviousTime() {
}

FPSCounter::~FPSCounter() = default;

void FPSCounter::fixedFrame() {
    float time = 0.f;
    float fix = 1000.f / mFixedFrame;

    while (time < fix) {
        QueryPerformanceFrequency(&mFrequency);

        time = static_cast<float>(Time::time() - mPreviousTime);
        time *= 1000.f / static_cast<float>(mFrequency.QuadPart);
    }
    mPreviousTime = Time::time();

    float deltaTime = static_cast<float>(time / 1000.f);
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    Time::deltaTime = deltaTime;

    drawFPS(time);
}

void FPSCounter::setFixedFrame(float fixedFrame) {
    mFixedFrame = fixedFrame;
}

void FPSCounter::drawFPS(float time) {
    mDrawUpdateTimer->update();
    if (mDrawUpdateTimer->isTime()) {
        mDrawUpdateTimer->reset();
        mCurrentFPS = 1000.f / time;
    }
    Debug::log()->drawFPS(mCurrentFPS);
}
