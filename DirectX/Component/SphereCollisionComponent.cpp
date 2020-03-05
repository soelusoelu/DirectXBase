#include "SphereCollisionComponent.h"
#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Renderer.h"

SphereCollisionComponent::SphereCollisionComponent(std::shared_ptr<Actor> owner) :
    Collider(owner, "SphereCollisionComponent"),
    mSphere(std::make_shared<Sphere>(Vector3::zero, 0.f)),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f),
    mSphereMesh(nullptr),
    mTransform(std::make_shared<Transform3D>()) {
}

SphereCollisionComponent::~SphereCollisionComponent() {
    if (mSphereMesh) {
        mSphereMesh->destroy();
    }
}

void SphereCollisionComponent::startCollider() {
    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        mesh->getMesh()->createSphere(&mSphere);
        mDefaultCenter = mSphere->center;
        mDefaultRadius = mSphere->radius;
#ifdef _DEBUG //デバッグ時のみ当たり判定表示
        mSphereMesh = std::make_shared<Mesh>(owner()->renderer(), "Shape/Sphere.obj");
        mSphereMesh->addToManager();
        mTransform->setScale(mSphere->radius);
        mSphereMesh->setTransform(mTransform);
#endif // _DEBUG
    }
}

void SphereCollisionComponent::updateCollider() {
}

void SphereCollisionComponent::onUpdateWorldTransformCollider() {
    auto center = mDefaultCenter + owner()->transform()->getPosition();
    auto radius = mDefaultRadius * owner()->transform()->getScale().y;

    mSphere->set(center, radius);

#ifdef _DEBUG
    mTransform->setPosition(center);
    mTransform->setScale(radius * 2.f);
    mTransform->computeWorldTransform();
#endif // _DEBUG
}

void SphereCollisionComponent::set(const Vector3 & center, float radius) {
    mSphere->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

std::shared_ptr<Sphere> SphereCollisionComponent::getSphere() const {
    return mSphere;
}
