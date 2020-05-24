#include "SphereCollisionComponent.h"
#include "MeshComponent.h"
#include "../Component/ComponentManager.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

SphereCollisionComponent::SphereCollisionComponent() :
    Collider(),
    mSphere(std::make_shared<Sphere>()),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f) {
}

SphereCollisionComponent::~SphereCollisionComponent() = default;

void SphereCollisionComponent::start() {
    Collider::start();

    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        mSphere->center = mesh->getCenter();
        mSphere->radius = mesh->getRadius();
        mDefaultCenter = mSphere->center;
        mDefaultRadius = mSphere->radius;
    }
}

void SphereCollisionComponent::onUpdateWorldTransform() {
    Collider::onUpdateWorldTransform();

    if (!mIsAutoUpdate) {
        return;
    }

    auto center = mDefaultCenter + owner()->transform()->getPosition();
    auto radius = mDefaultRadius * owner()->transform()->getScale().y;

    mSphere->set(center, radius);
}

void SphereCollisionComponent::set(const Vector3 & center, float radius) {
    mSphere->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Sphere& SphereCollisionComponent::getSphere() const {
    return *mSphere;
}
