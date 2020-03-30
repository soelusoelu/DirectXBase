#pragma once

#include "Component.h"
#include <list>
#include <memory>
#include <string>

class GameObject;
class Physics;

class Collider : public Component, public std::enable_shared_from_this<Collider> {
    using CollPtr = std::shared_ptr<Collider>;
    using CollPtrList = std::list<CollPtr>;

protected:
    Collider(std::shared_ptr<GameObject> owner, const std::string& type);
    virtual ~Collider();

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
    void addHitCollider(CollPtr hit);
    CollPtrList onCollisionEnter();
    CollPtrList onCollisionStay();
    CollPtrList onCollisionExit();

    static void setPhysics(Physics* physics);

protected:
    bool mIsAutoUpdate;

private:
    CollPtrList mPreviousCollider;
    CollPtrList mCurrentCollider;
    bool mEnable;

    static Physics* mPhysics;
};

