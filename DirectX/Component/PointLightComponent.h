#pragma once

#include "Component.h"
#include "../System/Game.h"
#include "../Utility/Math.h"
#include <memory>

class Actor;
class Camera;
class Mesh;
class Shader;

class PointLightComponent : public Component {
public:
    PointLightComponent(Actor* owner);
    ~PointLightComponent();
    virtual void start() override;
    virtual void update() override;
    void draw(std::shared_ptr<Shader> shader, Mesh* mesh, std::shared_ptr<Camera> camera) const;

    Vector3 mDiffuseColor; //色
    float mInnerRadius; //この半径以内だと完全な輝度で照らす
    float mOuterRadius; //光の届く半径
};
