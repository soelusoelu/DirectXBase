#include "Physics.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Component/Collider.h"
#include "../Component/ComponentManager.h"
#include "../Utility/Collision.h"
#include <algorithm>

Physics::Physics() = default;
Physics::~Physics() = default;

void Physics::add(Collider* circle) {
    mColliders.emplace_back(circle);
}

void Physics::remove(Collider* circle) {
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

    //mCenter.x - mCenter.mRadiusが小さい順にソート
    //std::sort(mColliders.begin(), mColliders.end(), [](Collider* a, Collider* b) {
    //    auto circleA = dynamic_cast<CircleCollisionComponent*>(a);
    //    auto circleB = dynamic_cast<CircleCollisionComponent*>(b);
    //    return circleA->getCircle()->center.x - circleA->getCircle()->radius < circleB->getCircle()->center.x - circleB->getCircle()->radius;
    //});

    //for (size_t i = 0; i < mColliders.size(); i++) {
    //    auto a = dynamic_cast<CircleCollisionComponent*>(mColliders[i]);
    //    if (!a->getEnable()) {
    //        continue;
    //    }
    //    auto ac = a->getCircle();
    //    //mCircles[i]の中心+半径を取得
    //    float max = ac->center.x + ac->radius;
    //    for (size_t j = i + 1; j < mColliders.size(); j++) {
    //        auto b = dynamic_cast<CircleCollisionComponent*>(mColliders[j]);
    //        if (!b->getEnable()) {
    //            continue;
    //        }
    //        auto bc = b->getCircle();
    //        //もしmCircles[j]の中心-半径が、mCircles[i]の中心+半径を超えていたら、
    //        //mCircles[i]と交差する可能性があるボックスは存在しない
    //        if (bc->center.x - bc->radius > max) {
    //            break;
    //        } else if (intersect(*ac, *bc)) {
    //            a->addHitCollider(b);
    //            b->addHitCollider(a);
    //        }
    //    }
    //}
}
