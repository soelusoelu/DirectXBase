#include "FixedDebugInformation.h"
#include "../Actor/Actor.h"
#include "../Device/DrawString.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

FixedDebugInformation::FixedDebugInformation(DrawString* drawString) :
    mDrawString(drawString),
    mScale(Vector2::one),
    mActorsPos(Vector2::zero),
    mNumRowsToDisplay(0),
    mFPSPos(Vector2::zero) {
}

FixedDebugInformation::~FixedDebugInformation() = default;

void FixedDebugInformation::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["fixedDebugInfo"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
    }
}

void FixedDebugInformation::initialize() {
    mFPSPos = Vector2(Window::width() / 2.f, Window::height());
    mActorsPos = mFPSPos + Vector2(0.f, DrawString::HEIGHT * mScale.y);
    mNumRowsToDisplay = (Window::debugWidth() - Window::height()) / (DrawString::HEIGHT * mScale.y);
}

void FixedDebugInformation::drawFPS(float fps) const {
#ifdef _DEBUG
    auto drawPos = mFPSPos;
    std::string str = "fps";
    mDrawString->drawString(str, drawPos, mScale);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * mScale.x;
    mDrawString->drawString(StringUtil::floatToString(fps, 2), drawPos, mScale);
#endif // _DEBUG
}

void FixedDebugInformation::drawActors(std::list<std::shared_ptr<Actor>> actors) {
#ifdef _DEBUG
    auto drawPos = mActorsPos;
    std::string str = "Actors";
    mDrawString->drawString(str, drawPos, mScale);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * mScale.x;

    for (const auto& actor : actors) {
        mDrawString->drawString(actor->tag(), drawPos, mScale);
        drawPos.y += DrawString::HEIGHT * mScale.y;
    }
#endif // _DEBUG
}
