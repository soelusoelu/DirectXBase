#include "SphereCollisionComponent.h"
#include "../Mesh/Mesh.h"
#include "MeshComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Physics.h"

SphereCollisionComponent::SphereCollisionComponent(Actor* owner) :
    Collider(owner),
    mSphere(nullptr),
    mSphereMesh(nullptr),
    mRadius(0.f) {
}

SphereCollisionComponent::~SphereCollisionComponent() = default;

void SphereCollisionComponent::startCollider() {
    auto mesh = mOwner->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        //mesh->getMesh()->createSphere(&mSphere);
        //mSphere.mCenter = mOwner->getTransform()->getPosition();
        //mDefaultRadius = mSphere.mRadius;
        //mSphereMesh = new MeshComponent(mOwner, "Sphere.obj");
    }
}

void SphereCollisionComponent::updateCollider() {
}

void SphereCollisionComponent::onUpdateWorldTransformCollider() {
    auto center = mOwner->transform()->getPosition();
    auto radius = mRadius * mOwner->transform()->getScale().y;
    center.y += radius;

    mSphere->set(center, radius);
}

void SphereCollisionComponent::set(const Vector3& center, float radius) {
    mSphere->set(center, radius);
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

void SphereCollisionComponent::drawMesh(float alpha) const {
    //Matrix4 w = Matrix4::createScale(mSphere.mRadius * 2);
    //w *= Matrix4::createTranslation(mSphere.mCenter);
    //if (mSphereMesh) {
    //    mSphereMesh->draw(w, alpha);
    //}
}

std::shared_ptr<Sphere> SphereCollisionComponent::getSphere() const {
    return mSphere;
}
