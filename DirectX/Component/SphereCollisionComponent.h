﻿#pragma once

#include "Collider.h"
#include "../Math/Math.h"
#include "../Utility/Collision.h"
#include <memory>

class GameObject;
class Mesh;
class Transform3D;

class SphereCollisionComponent : public Collider {
public:
    SphereCollisionComponent(std::shared_ptr<GameObject> owner);
    ~SphereCollisionComponent();
    virtual void startCollider() override;
    virtual void updateCollider() override;
    virtual void onUpdateWorldTransformCollider() override;
    void set(const Vector3& center, float radius);
    std::shared_ptr<Sphere> getSphere() const;

private:
    std::shared_ptr<Sphere> mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
    std::shared_ptr<Mesh> mSphereMesh;
    std::shared_ptr<Transform3D> mTransform;
};
