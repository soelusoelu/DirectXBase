#include "Text.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Text::Text() :
    Component(500),
    mText(""),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mColor(ColorPalette::white),
    mAlpha(1.f),
    mPivot(Pivot::Pivot::LEFT_TOP),
    mIsActive(true) {
}

Text::~Text() = default;

void Text::update() {
    if (!mIsActive) {
        return;
    }

    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;
    const auto& ds = owner()->renderer()->getDrawString();

    for (const auto& s : split) {
        ds->drawString(s, pos, mScale, mColor, mAlpha, mPivot);
        pos.y += DrawString::HEIGHT * mScale.y;
    }
}

void Text::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "text", &mText);
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getVector3(inObj, "color", &mColor);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    std::string src;
    if (JsonHelper::getString(inObj, "pivot", &src)) {
        Pivot::stringToPivot(src, &mPivot);
    }
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
}

void Text::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Text", mText);
    inspect->emplace_back("Position", mPosition);
    inspect->emplace_back("Scale", mScale);
    inspect->emplace_back("Color", mColor);
    inspect->emplace_back("Alpha", mAlpha);
    inspect->emplace_back("IsActive", mIsActive);
}

void Text::setText(const std::string & text) {
    mText = text;
}

const std::string& Text::text() const {
    return mText;
}

void Text::setPosition(const Vector2 & pos) {
    mPosition = pos;
}

const Vector2& Text::getPosition() const {
    return mPosition;
}

void Text::setScale(const Vector2 & scale) {
    mScale = scale;
}

const Vector2& Text::getScale() const {
    return mScale;
}

void Text::setColor(const Vector3 & color) {
    mColor = color;
}

const Vector3& Text::getColor() const {
    return mColor;
}

void Text::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Text::getAlpha() const {
    return mAlpha;
}

void Text::setPivot(Pivot::Pivot pivot) {
    mPivot = pivot;
}

Pivot::Pivot Text::getPivot() const {
    return mPivot;
}

void Text::setActive(bool value) {
    mIsActive = value;
}

bool Text::getActive() const {
    return mIsActive;
}
