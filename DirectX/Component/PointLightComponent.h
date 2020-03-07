#pragma once

#include "Component.h"
#include "../System/Game.h"
#include "../Utility/Math.h"
#include <memory>

class Actor;
class Camera;
struct PointLight;

class PointLightComponent : public Component, public std::enable_shared_from_this<PointLightComponent> {
public:
    PointLightComponent(std::shared_ptr<Actor> owner);
    ~PointLightComponent();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void draw(std::shared_ptr<PointLight> pointLight, std::shared_ptr<Camera> camera) const;
    void setColor(const Vector3& color);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setIntensity(float value);

private:
    Vector3 mColor; //色
    float mInnerRadius; //この半径以内だと完全な輝度で照らす
    float mOuterRadius; //光の届く半径
    float mIntensity; //光の強度
};
