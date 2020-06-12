#pragma once

#include "Component.h"
#include "../GameObject/Pivot.h"
#include "../Math/Math.h"

class TextFloat : public Component {
public:
    TextFloat();
    ~TextFloat();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setNumber(float number);
    float number() const;
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void setScale(const Vector2& scale);
    const Vector2& getScale() const;
    void setAlpha(float alpha);
    float getAlpha() const;
    void setDecimalDigits(int decimalDigits);
    int getDecimalDigits() const;
    void setPivot(Pivot pivot);
    Pivot getPivot() const;
    void setActive(bool value);
    bool getActive() const;

private:
    float mNumber;
    Vector2 mPosition;
    Vector2 mScale;
    float mAlpha;
    int mDecimalDigits;
    Pivot mPivot;
    bool mIsActive;
};
