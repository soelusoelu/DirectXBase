#include "Text.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Text::Text(std::shared_ptr<GameObject> owner) :
    Component(owner, "Text", 500),
    mText(""),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mColor(ColorPalette::white),
    mAlpha(1.f),
    mPivot(Pivot::LEFT_TOP),
    mIsRightJustified(false) {
}

Text::~Text() = default;

void Text::update() {
    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;
    auto ds = owner()->renderer()->getDrawString();

    if (mIsRightJustified) {
        for (const auto& s : split) {
            ds->drawStringRightJustified(s, pos, mScale, mColor, mAlpha, mPivot);
            pos.y += DrawString::HEIGHT * mScale.y;
        }
    } else {
        for (const auto& s : split) {
            ds->drawString(s, pos, mScale, mColor, mAlpha, mPivot);
            pos.y += DrawString::HEIGHT * mScale.y;
        }
    }
}

void Text::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getString(inObj, "text", &mText);
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getVector3(inObj, "color", &mColor);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    JsonHelper::getBool(inObj, "rightJustified", &mIsRightJustified);
}

void Text::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setString(alloc, inObj, "text", mText);
    JsonHelper::setVector2(alloc, inObj, "position", mPosition);
    JsonHelper::setVector2(alloc, inObj, "scale", mScale);
    JsonHelper::setVector3(alloc, inObj, "color", mColor);
    JsonHelper::setFloat(alloc, inObj, "alpha", mAlpha);
    JsonHelper::setBool(alloc, inObj, "rightJustified", mIsRightJustified);
}

void Text::drawDebugInfo(debugInfoList* inspect) const {
    debugInfo info;
    info.first = "Text";
    info.second = mText;
    inspect->emplace_back(info);
    info.first = "Position";
    info.second = InspectHelper::vector2ToString(mPosition);
    inspect->emplace_back(info);
    info.first = "Scale";
    info.second = InspectHelper::vector2ToString(mScale);
    inspect->emplace_back(info);
    info.first = "Color";
    info.second = InspectHelper::vector3ToString(mColor);
    inspect->emplace_back(info);
    info.first = "Alpha";
    info.second = StringUtil::floatToString(mAlpha);
    inspect->emplace_back(info);
    info.first = "RightJustified";
    info.second = StringUtil::boolToString(mIsRightJustified);
    inspect->emplace_back(info);
}

void Text::setText(const std::string & text) {
    mText = text;
}

void Text::setText(int number) {
    mText = StringUtil::intToString(number);
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

void Text::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot Text::getPivot() const {
    return mPivot;
}

void Text::setRightJustified(bool value) {
    mIsRightJustified = value;
}

bool Text::getRightJustified() const {
    return mIsRightJustified;
}
