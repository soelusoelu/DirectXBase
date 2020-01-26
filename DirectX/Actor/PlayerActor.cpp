#include "PlayerActor.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/SoundComponent.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag),
    mMesh(new MeshComponent(this, "Chips.obj")),
    mSound(new SoundComponent(this)),
    mMove(new PlayerMoveComponent(this)) {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
}
