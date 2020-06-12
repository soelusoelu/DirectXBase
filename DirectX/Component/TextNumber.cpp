#include "TextNumber.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../Utility/LevelLoader.h"

TextNumber::TextNumber() :
    Component(),
    mNumber(0),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mAlpha(1.f),
    mPivot(Pivot::LEFT_TOP),
    mIsActive(true) {
}

TextNumber::~TextNumber() = default;

void TextNumber::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    owner()->renderer()->getDrawString()->drawNumber(mNumber, mPosition, mScale, mPivot);
}

void TextNumber::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "number", &mNumber);
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    std::string src;
    if (JsonHelper::getString(inObj, "pivot", &src)) {
        PivotFunc::stringToPivot(src, &mPivot);
    }
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
}

void TextNumber::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Number", mNumber);
    inspect->emplace_back("Position", mPosition);
    inspect->emplace_back("Scale", mScale);
    inspect->emplace_back("Alpha", mAlpha);
    inspect->emplace_back("IsActive", mIsActive);
}

void TextNumber::setNumber(int number) {
    mNumber = number;
}

int TextNumber::number() const {
    return mNumber;
}

void TextNumber::setPosition(const Vector2& pos) {
    mPosition = pos;
}

const Vector2& TextNumber::getPosition() const {
    return mPosition;
}

void TextNumber::setScale(const Vector2& scale) {
    mScale = scale;
}

const Vector2& TextNumber::getScale() const {
    return mScale;
}

void TextNumber::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TextNumber::getAlpha() const {
    return mAlpha;
}

void TextNumber::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot TextNumber::getPivot() const {
    return mPivot;
}

void TextNumber::setActive(bool value) {
    mIsActive = value;
}

bool TextNumber::getActive() const {
    return mIsActive;
}
