#include "PlayerActor.h"
#include "../Component/CircleCollisionComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/SpriteComponent.h"

PlayerActor::PlayerActor(std::shared_ptr<Renderer> renderer, const char* tag) :
    Actor(renderer, tag),
    mCollide(new CircleCollisionComponent(this)),
    mSound(new SoundComponent(this)),
    mSprite(new SpriteComponent(this, "circleP.png")),
    mMove(new PlayerMoveComponent(this)) {
}

PlayerActor::~PlayerActor() = default;

void PlayerActor::updateActor() {
}
