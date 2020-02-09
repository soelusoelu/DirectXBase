#include "PointLightComponent.h"
#include "../Actor/Actor.h"
#include "../Device/Renderer.h"

PointLightComponent::PointLightComponent(Actor* owner) :
    Component(owner),
    mDiffuseColor(Vector3::one),
    mInnerRadius(1.f),
    mOuterRadius(5.f) {
    owner->renderer()->addPointLight(this);
}

PointLightComponent::~PointLightComponent() {
    mOwner->renderer()->removePointLight(this);
}

void PointLightComponent::start() {
}

void PointLightComponent::update() {
}
