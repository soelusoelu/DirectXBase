#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform2D.h"
#include "../Component/ComponentManager.h"
#include "../Component/CircleCollisionComponent.h"
#include "../Component/SoundComponent.h"
#include "../Device/Time.h"

PlayerMoveComponent::PlayerMoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    mCollider(nullptr),
    mSound(nullptr) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    mOwner->transform()->setPosition(Vector2(100.f, 200.f));
    mOwner->transform()->setPrimary(10);
    mCollider = mOwner->componentManager()->getComponent<CircleCollisionComponent>();
    mSound = mOwner->componentManager()->getComponent<SoundComponent>();
}

void PlayerMoveComponent::update() {
    move();
}

void PlayerMoveComponent::move() {
    float h = Input::horizontal();
    float v = Input::vertical();
    if (Math::nearZero(h) && Math::nearZero(v)) {
        h = Input::joyHorizontal();
        v = Input::joyVertical();
    }
    mOwner->transform()->translate(Vector2(h, -v) * 1200.f * Time::deltaTime);
}
