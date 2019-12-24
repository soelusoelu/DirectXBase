#pragma once

#include "Component.h"
#include <list>

class Actor;
class Physics;

class Collider : public Component {
public:
    virtual void start() override;
    virtual void startCollider() = 0;
    virtual void update() override;
    virtual void updateCollider() = 0;
    virtual void onUpdateWorldTransform() override;
    virtual void onUpdateWorldTransformCollider() = 0;
    void enabled();
    void disabled();
    bool getEnable() const;
    void addHitCollider(Collider* hit);
    std::list<Collider*> onCollisionEnter();
    std::list<Collider*> onCollisionStay();
    std::list<Collider*> onCollisionExit();

    static void setPhysics(Physics* physics);

protected:
    Collider(Actor* owner);
    virtual ~Collider();

private:
    bool mEnable;
    std::list<Collider*> mPreviousCollider;
    std::list<Collider*> mCurrentCollider;

    static Physics* mPhysics;
};

