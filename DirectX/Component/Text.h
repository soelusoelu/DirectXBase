#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

enum class Pivot;

class Text : public Component {
public:
    Text();
    ~Text();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
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
    void setActive(bool value);
    bool getActive() const;

private:
    std::string mText;
    Vector2 mPosition;
    Vector2 mScale;
    Vector3 mColor;
    float mAlpha;
    Pivot mPivot;
    bool mIsActive;
};
