#pragma once

#include "Collider.h"
#include "../Utility/Math.h"
#include "../Utility/Collision.h"
#include <memory>

class Actor;
class MeshComponent;

class SphereCollisionComponent : public Collider {
public:
    SphereCollisionComponent(Actor* owner);
    ~SphereCollisionComponent();
    virtual void startCollider() override;
    virtual void updateCollider() override;
    virtual void onUpdateWorldTransformCollider() override;
    void set(const Vector3& center, float radius);
    void drawMesh(float alpha = 0.5f) const;
    std::shared_ptr<Sphere> getSphere() const;

private:
    std::shared_ptr<Sphere> mSphere;
    MeshComponent* mSphereMesh;
    float mRadius;
};
