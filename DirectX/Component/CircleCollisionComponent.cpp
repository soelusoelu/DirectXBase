#include "CircleCollisionComponent.h"
#include "ComponentManager.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform2D.h"

CircleCollisionComponent::CircleCollisionComponent(Actor* onwer) :
    Collider(onwer),
    mCircle(nullptr) {
}

CircleCollisionComponent::~CircleCollisionComponent() = default;

void CircleCollisionComponent::startCollider() {
    mCircle = std::make_shared<Circle>(
        mOwner->getTransform()->getPosition() + mOwner->getTransform()->getPivot(),
        mOwner->getTransform()->getPivot().x
    );
}

void CircleCollisionComponent::updateCollider() {
}

void CircleCollisionComponent::onUpdateWorldTransformCollider() {
    mCircle->set(
        mOwner->getTransform()->getPosition() + mOwner->getTransform()->getPivot(),
        mOwner->getTransform()->getPivot().x
    );
}

std::shared_ptr<Circle> CircleCollisionComponent::getCircle() const {
    return mCircle;
}
