#pragma once

#include "Component.h"
#include "../GameObject/Pivot.h"
#include "../Math/Math.h"

class TextNumber : public Component {
public:
    TextNumber();
    ~TextNumber();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setNumber(int number);
    int number() const;
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void setScale(const Vector2& scale);
    const Vector2& getScale() const;
    void setAlpha(float alpha);
    float getAlpha() const;
    void setPivot(Pivot pivot);
    Pivot getPivot() const;
    void setActive(bool value);
    bool getActive() const;

private:
    int mNumber;
    Vector2 mPosition;
    Vector2 mScale;
    float mAlpha;
    Pivot mPivot;
    bool mIsActive;
};
