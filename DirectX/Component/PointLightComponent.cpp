#include "PointLightComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"

PointLightComponent::PointLightComponent(Actor* owner) :
    Component(owner),
    mDiffuseColor(Vector3::one),
    mInnerRadius(1.f),
    mOuterRadius(5.f),
    mShader(owner->renderer()->createShader(""))
{
    owner->renderer()->addPointLight(this);
}

PointLightComponent::~PointLightComponent() {
    mOwner->renderer()->removePointLight(this);
}

void PointLightComponent::start() {
}

void PointLightComponent::update() {
}

void PointLightComponent::draw() const {
    auto scale = Matrix4::createScale(mOwner->transform()->getScale() * mOuterRadius);
    auto trans = Matrix4::createTranslation(mOwner->transform()->getPosition());
    auto world = scale * trans;
}
