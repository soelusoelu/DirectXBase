#include "Text.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Utility/StringUtil.h"

Text::Text(std::shared_ptr<Renderer> renderer) :
    UI(renderer, "Text"),
    mText(""),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mColor(ColorPalette::white),
    mAlpha(1.f),
    mPivot(Pivot::LEFT_TOP),
    mIsRightJustified(false) {
}

Text::~Text() = default;

void Text::updateUI() {
    //重いかなぁ
    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;

    if (mIsRightJustified) {
        for (const auto& s : split) {
            mRenderer->getDrawString()->drawStringRightJustified(s, pos, mScale, mColor, mAlpha, mPivot);
            pos.y += DrawString::HEIGHT * mScale.y;
        }
    } else {
        for (const auto& s : split) {
            mRenderer->getDrawString()->drawString(s, pos, mScale, mColor, mAlpha, mPivot);
            pos.y += DrawString::HEIGHT * mScale.y;
        }
    }
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
