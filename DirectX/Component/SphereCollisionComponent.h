#pragma once

#include "Collider.h"
#include "../Math/Math.h"
#include "../Utility/Collision.h"
#include <memory>

class SphereCollisionComponent : public Collider {
public:
    SphereCollisionComponent();
    ~SphereCollisionComponent();
    virtual void start() override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    virtual void onUpdateWorldTransform() override;
    void set(const Vector3& center, float radius);
    const Sphere& getSphere() const;

private:
    std::shared_ptr<Sphere> mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
};
