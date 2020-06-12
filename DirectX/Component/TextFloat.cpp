#include "TextFloat.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../Utility/LevelLoader.h"

TextFloat::TextFloat() :
    Component(),
    mNumber(0.f),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mAlpha(1.f),
    mDecimalDigits(1),
    mPivot(Pivot::LEFT_TOP),
    mIsActive(true) {
}

TextFloat::~TextFloat() = default;

void TextFloat::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    owner()->renderer()->getDrawString()->drawNumber(mNumber, mPosition, mScale, mDecimalDigits, mPivot);
}

void TextFloat::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "number", &mNumber);
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    JsonHelper::getInt(inObj, "decimalDigits", &mDecimalDigits);
    std::string src;
    if (JsonHelper::getString(inObj, "pivot", &src)) {
        PivotFunc::stringToPivot(src, &mPivot);
    }
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
}

void TextFloat::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Number", mNumber);
    inspect->emplace_back("Position", mPosition);
    inspect->emplace_back("Scale", mScale);
    inspect->emplace_back("Alpha", mAlpha);
    inspect->emplace_back("DecimalDigits", mDecimalDigits);
    inspect->emplace_back("IsActive", mIsActive);
}

void TextFloat::setNumber(float number) {
    mNumber = number;
}

float TextFloat::number() const {
    return mNumber;
}

void TextFloat::setPosition(const Vector2& pos) {
    mPosition = pos;
}

const Vector2& TextFloat::getPosition() const {
    return mPosition;
}

void TextFloat::setScale(const Vector2& scale) {
    mScale = scale;
}

const Vector2& TextFloat::getScale() const {
    return mScale;
}

void TextFloat::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TextFloat::getAlpha() const {
    return mAlpha;
}

void TextFloat::setDecimalDigits(int decimalDigits) {
    mDecimalDigits = decimalDigits;
}

int TextFloat::getDecimalDigits() const {
    return mDecimalDigits;
}

void TextFloat::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot TextFloat::getPivot() const {
    return mPivot;
}

void TextFloat::setActive(bool value) {
    mIsActive = value;
}

bool TextFloat::getActive() const {
    return mIsActive;
}
