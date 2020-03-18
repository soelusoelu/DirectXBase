﻿#include "UIButton.h"

UIButton::UIButton(std::function<void()> onClick, const Vector2& pos, const Vector2& dims) :
    mOnClick(onClick),
    mPosition(pos),
    mDimensions(dims),
    mHighlighted(false) {
}

UIButton::~UIButton() = default;

void UIButton::setClickFunc(std::function<void()> onClick) {
    mOnClick = onClick;
}

const Vector2& UIButton::getPosition() const {
    return mPosition;
}

void UIButton::setHighlighted(bool set) {
    mHighlighted = set;
}

bool UIButton::getHighlighted() const {
    return mHighlighted;
}

bool UIButton::containsPoint(const Vector2 & pt) const {
    bool no = pt.x < (mPosition.x) ||
        pt.x > (mPosition.x + mDimensions.x) ||
        pt.y < (mPosition.y) ||
        pt.y > (mPosition.y + mDimensions.y);
    return !no;
}

void UIButton::onClick() {
    if (mOnClick) {
        mOnClick();
    }
}
