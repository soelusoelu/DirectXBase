#include "PlayerActor.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/SoundComponent.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag),
    mSound(new SoundComponent(this)),
    mMove(new PlayerMoveComponent(this)) {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
}
