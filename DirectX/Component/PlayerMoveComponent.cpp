#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/SoundComponent.h"
#include "../Device/Time.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"

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
    float h = Input::keyboard()->horizontal();
    float v = Input::keyboard()->vertical();
    if (!Math::nearZero(h) || !Math::nearZero(v)) {
        owner()->transform()->translate(Vector3(h, 0.f, v) * Time::deltaTime);
    }
}
