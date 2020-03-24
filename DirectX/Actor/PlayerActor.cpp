#include "PlayerActor.h"
#include "../Component/ComponentManager.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/SoundComponent.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Log.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer) :
    Actor(renderer, "Player"),
    mMoveComp(nullptr) {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::start() {
    mComponentManager->addComponent<SoundComponent>();
    mMoveComp = std::make_shared<PlayerMoveComponent>(shared_from_this());
    mComponentManager->addComponent(mMoveComp);

    Debug::log()->log("Player spawn.");
}

void PlayerActor::updateActor() {
}

bool PlayerActor::isJump() const {
    return mMoveComp->isJump();
}

void PlayerActor::setTargetPosition(const Vector3& pos) {
    mMoveComp->setTargetPosition(pos);
}
