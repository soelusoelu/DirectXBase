#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/SoundComponent.h"
#include "../Device/Time.h"
#include "../Utility/Input.h"

PlayerMoveComponent::PlayerMoveComponent(std::shared_ptr<Actor> owner) :
    Component(owner, "PlayerMoveComponent", 10),
    mSound(nullptr) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    mSound = owner()->componentManager()->getComponent<SoundComponent>();
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

    owner()->transform()->translate(Vector3(h, 0.f, v) * Time::deltaTime);
}
