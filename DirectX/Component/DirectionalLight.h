#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class GameObject;

//要修正
class DirectionalLight : public Component {
public:
    DirectionalLight();
    ~DirectionalLight();
    virtual void awake() override;
    virtual void onUpdateWorldTransform() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    const Vector3& getDirection() const;
    void setDirection(const Vector3& dir);
    const Vector3& getColor() const;
    void setColor(const Vector3& color);

private:
    Vector3 mDirection;
    Vector3 mColor;
};
