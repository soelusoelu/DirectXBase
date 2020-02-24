#pragma once

#include "Component.h"
#include <list>
#include <memory>

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
    void automation();
    void addHitCollider(Collider* hit);
    std::list<Collider*> onCollisionEnter();
    std::list<Collider*> onCollisionStay();
    std::list<Collider*> onCollisionExit();

    static void setPhysics(Physics* physics);

protected:
    Collider(std::shared_ptr<Actor> owner);
    virtual ~Collider();

protected:
    bool mIsAutoUpdate;

private:
    std::list<Collider*> mPreviousCollider;
    std::list<Collider*> mCurrentCollider;
    bool mEnable;

    static Physics* mPhysics;
};

