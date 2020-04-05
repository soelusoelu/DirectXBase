#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include "../System/Game.h"
#include <memory>

class Camera;
class GameObject;
struct PointLight;

class PointLightComponent : public Component, public std::enable_shared_from_this<PointLightComponent> {
public:
    PointLightComponent(std::shared_ptr<GameObject> owner);
    ~PointLightComponent();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    void draw(const PointLight& pointLight) const;
    void setColor(const Vector3& color);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setIntensity(float value);

private:
    std::shared_ptr<Camera> mCamera;
    Vector3 mColor; //色
    float mInnerRadius; //この半径以内だと完全な輝度で照らす
    float mOuterRadius; //光の届く半径
    float mIntensity; //光の強度
};
