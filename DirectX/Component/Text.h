#pragma once

#include "Component.h"
#include "../GameObject/Transform2D.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

class Text : public Component {
public:
    Text(std::shared_ptr<GameObject> owner);
    ~Text();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    void setText(const std::string& text);
    void setText(int number);
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

private:
    std::string mText;
    Vector2 mPosition;
    Vector2 mScale;
    Vector3 mColor;
    float mAlpha;
    Pivot mPivot;
    bool mIsRightJustified;
};
