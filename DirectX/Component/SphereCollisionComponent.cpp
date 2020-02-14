#include "SphereCollisionComponent.h"
#include "MeshComponent.h"
#include "../Mesh/Mesh.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Renderer.h"

SphereCollisionComponent::SphereCollisionComponent(Actor* owner) :
    Collider(owner),
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
    auto mesh = mOwner->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        mesh->getMesh()->createSphere(&mSphere);
        mDefaultCenter = mSphere->center;
        mDefaultRadius = mSphere->radius;
#ifdef _DEBUG //デバッグ時のみ当たり判定表示
        //mSphereMesh = new Mesh(mOwner->renderer(), "Sphere.obj");
        //mTransform->setScale(mSphere->radius);
        //mSphereMesh->setTransform(mTransform);
#endif // _DEBUG
    }
}

void SphereCollisionComponent::updateCollider() {
}

void SphereCollisionComponent::onUpdateWorldTransformCollider() {
    auto center = mDefaultCenter + mOwner->transform()->getPosition();
    auto radius = mDefaultRadius * mOwner->transform()->getScale().y;

    mSphere->set(center, radius);

#ifdef _DEBUG
    //mTransform->setPosition(center);
    //mTransform->setScale(radius * 2.f);
    //mTransform->computeWorldTransform();
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
