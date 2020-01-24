#pragma once

#include "Collider.h"
#include "../Utility/Collision.h"
#include <memory>

class Actor;

class CircleCollisionComponent : public Collider {
public:
    CircleCollisionComponent(Actor* owner);
    ~CircleCollisionComponent();
    virtual void startCollider() override;
    virtual void updateCollider() override;
    virtual void onUpdateWorldTransformCollider() override;
    void set(const Vector2& center, float radius);
    std::shared_ptr<Circle> getCircle() const;
    void automation();

private:
    std::shared_ptr<Circle> mCircle;
    bool mIsAutoUpdate;
};
