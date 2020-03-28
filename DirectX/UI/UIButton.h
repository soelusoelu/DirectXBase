#pragma once

#include "../Math/Math.h"
#include <functional>

class UIButton {
public:
    //ボタン名称、コールバック関数、位置、寸法
    UIButton(std::function<void()> onClick, const Vector2& pos, const Vector2& dims);
    ~UIButton();

    void setClickFunc(std::function<void()> onClick);
    const Vector2& getPosition() const;
    void setHighlighted(bool set);
    bool getHighlighted() const;

    //ptがボタンの範囲内ならtrueを返す
    bool containsPoint(const Vector2& pt) const;
    //ボタンが押されたときに呼び出される
    void onClick();

private:
    std::function<void()> mOnClick;
    Vector2 mPosition;
    Vector2 mDimensions;
    bool mHighlighted;
};
