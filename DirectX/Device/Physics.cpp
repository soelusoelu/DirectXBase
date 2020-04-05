#include "Physics.h"
#include "../Component/Collider.h"
#include "../Component/ComponentManager.h"
#include "../Component/SphereCollisionComponent.h"
#include "../Utility/Collision.h"
#include <algorithm>

Physics::Physics() = default;
Physics::~Physics() = default;

void Physics::add(const CollPtr& circle) {
    mColliders.emplace_back(circle);
}

void Physics::remove(const CollPtr& circle) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), circle);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }
}

void Physics::clear() {
    mColliders.clear();
}

void Physics::sweepAndPrune() {
    if (mColliders.empty()) {
        return;
    }

    //center.x - mCenter.radiusが小さい順にソート
    std::sort(mColliders.begin(), mColliders.end(), [](CollPtr a, CollPtr b) {
        auto circleA = std::dynamic_pointer_cast<SphereCollisionComponent>(a);
        auto circleB = std::dynamic_pointer_cast<SphereCollisionComponent>(b);
        return circleA->getSphere()->center.x - circleA->getSphere()->radius < circleB->getSphere()->center.x - circleB->getSphere()->radius;
    });

    for (size_t i = 0; i < mColliders.size(); i++) {
        auto a = std::dynamic_pointer_cast<SphereCollisionComponent>(mColliders[i]);
        if (!a->getEnable()) {
            continue;
        }
        auto as = a->getSphere();
        //mCircles[i]の中心+半径を取得
        float max = as->center.x + as->radius;
        for (size_t j = i + 1; j < mColliders.size(); j++) {
            auto b = std::dynamic_pointer_cast<SphereCollisionComponent>(mColliders[j]);
            if (!b->getEnable()) {
                continue;
            }
            auto bs = b->getSphere();
            //もしmCircles[j]の中心-半径が、mCircles[i]の中心+半径を超えていたら、
            //mCircles[i]と交差する可能性があるボックスは存在しない
            if (bs->center.x - bs->radius > max) {
                break;
            } else if (intersect(*as, *bs)) {
                a->addHitCollider(b);
                b->addHitCollider(a);
            }
        }
    }
}
