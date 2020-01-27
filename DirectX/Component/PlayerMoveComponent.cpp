#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/SoundComponent.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"

PlayerMoveComponent::PlayerMoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    mSound(nullptr) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    mOwner->transform()->translate(Vector3::up * 3.f);

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

    mOwner->transform()->translate(Vector3(h, 0.f, v) * Time::deltaTime);
}
