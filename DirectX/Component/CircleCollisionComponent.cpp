#include "CircleCollisionComponent.h"
#include "ComponentManager.h"
#include "SpriteComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform2D.h"

CircleCollisionComponent::CircleCollisionComponent(Actor* onwer) :
    Collider(onwer),
    mCircle(nullptr),
    mSprite(nullptr) {
}

CircleCollisionComponent::~CircleCollisionComponent() = default;

void CircleCollisionComponent::startCollider() {
    mSprite = mOwner->getComponentManager()->getComponent<SpriteComponent>();

    mCircle = std::make_shared<Circle>(
        mOwner->getTransform()->getPosition() + mSprite->getPivot(),
        mSprite->getScreenTextureSize().x - mSprite->getPivot().x
    );
}

void CircleCollisionComponent::updateCollider() {
}

void CircleCollisionComponent::onUpdateWorldTransformCollider() {
    mCircle->set(
        mOwner->getTransform()->getPosition() + mSprite->getPivot(),
        mSprite->getScreenTextureSize().x - mSprite->getPivot().x
    );
}

std::shared_ptr<Circle> CircleCollisionComponent::getCircle() const {
    return mCircle;
}
