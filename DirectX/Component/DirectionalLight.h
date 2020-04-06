#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class GameObject;

class DirectionalLight : public Component {
public:
    DirectionalLight(std::shared_ptr<GameObject> owner);
    ~DirectionalLight();
    virtual void onUpdateWorldTransform() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    const Vector3& getDirection() const;
    void setDirection(const Vector3& dir);
    const Vector3& getColor() const;
    void setColor(const Vector3& color);

private:
    Vector3 mDirection;
    Vector3 mColor;
};
