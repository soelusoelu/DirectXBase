﻿#pragma once

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
    virtual void update() override;
    virtual void onSetActive(bool value) override;
    //当たり判定を有効化
    void enabled();
    //当たり判定を無効化
    void disabled();
    //当たり判定が有効か
    bool getEnable() const;
    //衝突判定の自動化
    void automation();
    //コライダーを追加
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

