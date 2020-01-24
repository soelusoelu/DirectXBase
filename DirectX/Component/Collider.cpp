#include "Collider.h"
#include "../Device/Physics.h"
#include <algorithm>

Collider::Collider(Actor* owner) :
    Component(owner, 1000),
    mEnable(false) {
}

Collider::~Collider() {
    if (mPhysics) {
        mPhysics->remove(this);
    }
}

void Collider::start() {
    startCollider();
    if (mPhysics) {
        mPhysics->add(this);
        mEnable = true;
    }
}

void Collider::update() {
    updateCollider();

    mPreviousCollider.resize(mCurrentCollider.size());
    std::copy(mCurrentCollider.begin(), mCurrentCollider.end(), mPreviousCollider.begin());
    mCurrentCollider.clear();
}

void Collider::onUpdateWorldTransform() {
    onUpdateWorldTransformCollider();
}

void Collider::enabled() {
    mEnable = true;
}

void Collider::disabled() {
    mEnable = false;
}

bool Collider::getEnable() const {
    return mEnable;
}

void Collider::addHitCollider(Collider* hit) {
    mCurrentCollider.emplace_back(hit);
}

std::list<Collider*> Collider::onCollisionEnter() {
    std::list<Collider*> temp;
    for (const auto& c : mCurrentCollider) {
        auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), c);
        if (itr == mPreviousCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

std::list<Collider*> Collider::onCollisionStay() {
    std::list<Collider*> temp;
    for (const auto& c : mCurrentCollider) {
        auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), c);
        if (itr != mPreviousCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

std::list<Collider*> Collider::onCollisionExit() {
    std::list<Collider*> temp;
    for (const auto& c : mPreviousCollider) {
        auto itr = std::find(mCurrentCollider.begin(), mCurrentCollider.end(), c);
        if (itr == mCurrentCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

void Collider::setPhysics(Physics* physics) {
    mPhysics = physics;
}

Physics* Collider::mPhysics = nullptr;
