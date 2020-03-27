#pragma once

#include "UI.h"
#include "../Actor/Transform2D.h"
#include "../Utility/Math.h"
#include <memory>
#include <string>

class Renderer;

class Text : public UI {
public:
    Text(std::shared_ptr<Renderer> renderer);
    ~Text();
    virtual void updateUI() override;
    void setText(const std::string& text);
    const std::string& text() const;
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void setScale(const Vector2& scale);
    const Vector2& getScale() const;
    void setColor(const Vector3& color);
    const Vector3& getColor() const;
    void setAlpha(float alpha);
    float getAlpha() const;
    void setPivot(Pivot pivot);
    Pivot getPivot() const;
    //文字を右詰めにするか
    void setRightJustified(bool value);
    bool getRightJustified() const;

protected:
    std::string mText;
    Vector2 mPosition;
    Vector2 mScale;
    Vector3 mColor;
    float mAlpha;
    Pivot mPivot;
    bool mIsRightJustified;
};
