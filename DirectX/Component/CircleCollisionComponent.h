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
    std::shared_ptr<Circle> getCircle() const;

private:
    std::shared_ptr<Circle> mCircle;
};
